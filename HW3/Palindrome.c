#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>      
#include <sys/types.h>   
#include <sys/mman.h>   
#include <sys/stat.h>
#include <fcntl.h>     


#define VERBOSE 1
#if VERBOSE
  #define LOGF(...) do { printf(__VA_ARGS__); fflush(stdout); } while (0)
#else
  #define LOGF(...) do {} while (0)
#endif

#define MAX_WORD 256
#define MAX_NAME 128

struct shared_data {
    char word[MAX_WORD];
    int  result; // 1 = palindrome, 0 = not
};

static int is_palindrome(const char *s) {
    size_t len = strlen(s);
    if (len == 0) return 1; // empty string is palindrome
    size_t l = 0, r = len - 1;
    while (l < r) {
        if (s[l] != s[r]) return 0;
        l++; r--;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage (child): %s <pipe_read_fd>\n", argv[0]);
        return 1;
    }

    // Read-end FD passed from Manager
    int read_fd = atoi(argv[1]);
    if (read_fd < 0) {
        fprintf(stderr, "Invalid read fd: %s\n", argv[1]);
        return 1;
    }

    LOGF("Palindrome process [%ld]: starting.\n", (long)getpid());

    // Read shm name (C-string) from pipe
    char shm_name[MAX_NAME];
    size_t pos = 0;
    while (pos < MAX_NAME) {
        char c;
        ssize_t n = read(read_fd, &c, 1);
        if (n == -1) {
            perror("read");
            close(read_fd);
            return 1;
        }
        if (n == 0) { // EOF
            shm_name[pos < MAX_NAME ? pos : (MAX_NAME - 1)] = '\0';
            break;
        }
        shm_name[pos++] = c;
        if (c == '\0') break;
    }
    if (pos == MAX_NAME) shm_name[MAX_NAME - 1] = '\0';
    close(read_fd);

    // Open and map shared memory
    int shm_fd = shm_open(shm_name, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open (child)");
        return 1;
    }
    struct shared_data *mem = mmap(NULL, sizeof(struct shared_data),
                                   PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (mem == MAP_FAILED) {
        perror("mmap (child)");
        close(shm_fd);
        return 1;
    }
    close(shm_fd);

    // Log the word about to be check
    LOGF("Palindrome process [%ld]: read word \"%s\" from shared memory\n",
         (long)getpid(), mem->word);

    // Compute and log decision
    int pal = is_palindrome(mem->word);
    if (pal) {
        LOGF("Palindrome process [%ld]: %s *IS* a palindrome\n",
             (long)getpid(), mem->word);
    } else {
        LOGF("Palindrome process [%ld]: %s *IS NOT* a palindrome\n",
             (long)getpid(), mem->word);
    }

    // Write result back and log it
    mem->result = pal;
    LOGF("Palindrome process [%ld]: wrote result (%d) to shared memory.\n",
         (long)getpid(), pal);

    // Cleanup!!
    if (munmap(mem, sizeof(struct shared_data)) == -1) {
        perror("munmap");
        return 1;
    }

    return 0;
}
