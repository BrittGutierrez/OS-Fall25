#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     
#include <sys/mman.h>   
#include <sys/stat.h>  
#include <fcntl.h>      
#include <sys/wait.h>   
#include <errno.h>

#define VERBOSE 1
#if VERBOSE
  #define LOGF(...) do { printf(__VA_ARGS__); fflush(stdout); } while (0)
#else
  #define LOGF(...) do {} while (0)
#endif


#define MAX_WORD 256
#define MAX_NAME 128
#define MAX_PROC 128

// Structure stored in each shared memory segment
struct shared_data {
    char word[MAX_WORD]; // input word
    int  result;         // 1 = palindrome, 0 = not, -1 = not set yet
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <word1> <word2> ... <wordN>\n", argv[0]);
        return 1;
    }

    int n = argc - 1;
    if (n > MAX_PROC) {
        fprintf(stderr, "Too many words (max %d)\n", MAX_PROC);
        return 1;
    }

    int pipes[MAX_PROC][2];               // one pipe per child (Manager -> Child)
    char shm_names[MAX_PROC][MAX_NAME];   // one shm name per child
    int shm_fds[MAX_PROC];                // shm fds
    struct shared_data *maps[MAX_PROC];   // mapped shared memory pointers
    pid_t child_pids[MAX_PROC];           // for wait()

    pid_t parent_pid = getpid();

    // Create (pipe + shm + mapping)/ child
    for (int i = 0; i < n; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return 1;
        }

        // POSIX shm names must start with '/'
        if (snprintf(shm_names[i], MAX_NAME, "/pal_shm_%ld_%d",
                     (long)parent_pid, i) >= MAX_NAME) {
            fprintf(stderr, "shm name too long\n");
            return 1;
        }

        shm_fds[i] = shm_open(shm_names[i], O_CREAT | O_RDWR, 0666);
        if (shm_fds[i] == -1) {
            perror("shm_open");
            return 1;
        }

        if (ftruncate(shm_fds[i], sizeof(struct shared_data)) == -1) {
            perror("ftruncate");
            return 1;
        }

        maps[i] = mmap(NULL, sizeof(struct shared_data),
                       PROT_READ | PROT_WRITE, MAP_SHARED, shm_fds[i], 0);
        if (maps[i] == MAP_FAILED) {
            perror("mmap");
            return 1;
        }

        // Initialize contents
        memset(maps[i], 0, sizeof(struct shared_data));
        strncpy(maps[i]->word, argv[i + 1], MAX_WORD - 1);
        maps[i]->word[MAX_WORD - 1] = '\0';
        maps[i]->result = -1;
    }

    // Fork ALL children first (parallel)
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return 1;
        }

        if (pid == 0) {
            // Child... 
            for (int k = 0; k < n; k++) {
                if (k == i) {
                    close(pipes[k][1]); // keep read end pipes[i][0]
                } else {
                    close(pipes[k][0]);
                    close(pipes[k][1]);
                }
            }

            char fd_str[16];
            snprintf(fd_str, sizeof(fd_str), "%d", pipes[i][0]);

            execlp("./Palindrome", "Palindrome", fd_str, (char *)NULL);
            perror("execlp");
            _exit(1);
        } else {
            // Parent...
            child_pids[i] = pid;
            LOGF("Manager: forked process with ID %ld.\n", (long)pid);

            // Manager writes shm name to child's pipe, then closes write end
            close(pipes[i][0]); // parent writes only
            size_t len = strnlen(shm_names[i], MAX_NAME);
            ssize_t bw = write(pipes[i][1], shm_names[i], len + 1); // include '\0'
            if (bw == -1) {
                perror("write");
                return 1;
            }
            LOGF("Manager: wrote shm name to pipe (%zd bytes)\n", bw);
            close(pipes[i][1]);
        }
    }

    // Wait for all children
    for (int i = 0; i < n; i++) {
        LOGF("Manager: waiting on child process ID %ld...\n", (long)child_pids[i]);
        int status = 0;
        if (waitpid(child_pids[i], &status, 0) == -1) {
            perror("waitpid");
        }
    }

    // Read results and print verbose summary lines
    for (int i = 0; i < n; i++) {
        const char *w = maps[i]->word;
        int r = maps[i]->result;
        if (r == 1) {
            LOGF("Manager: result %d read from shared memory. \"%s\" is a palindrome.\n", r, w);
        } else if (r == 0) {
            LOGF("Manager: result %d read from shared memory. \"%s\" is not a palindrome.\n", r, w);
        } else {
            LOGF("Manager: result %d read from shared memory. \"%s\" has no result.\n", r, w);
        }
    }

    LOGF("Manager: exiting.\n");

    // Cleanup shared memory objects
    for (int i = 0; i < n; i++) {
        if (maps[i] && maps[i] != MAP_FAILED) {
            munmap(maps[i], sizeof(struct shared_data));
        }
        if (shm_fds[i] != -1) {
            close(shm_fds[i]);
        }
        if (shm_unlink(shm_names[i]) == -1) {
            fprintf(stderr, "Warning: shm_unlink(%s): %s\n",
                    shm_names[i], strerror(errno));
        }
    }

    return 0;
}
