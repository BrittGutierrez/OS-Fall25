
#include <stdio.h>     
#include <stdlib.h>    
#include <string.h>    
#include <unistd.h>    
#include <sys/types.h> 
#include <sys/wait.h>  
#include <errno.h>     
#define BUF_SZ 256 // constant val for max size of line


/* 
1. Execute fork() to create child process
2. Child process runs exec() to run helper programs (OddEven, PerfectSquare, Factorial)
3. wait() used to wait for child and get return val from WIFEXITED(status)
*/
int launch_and_wait(const char *prog_path, const char *progname, const char *arg){
    pid_t pid = fork(); // copy of process

    //parent returns PID of child, child returns 0, -# is error
    if(pid < 0){
        perror("fork");
        return -1;
    }

    int status;
    if(pid == 0){ //Child Process  
        execlp(prog_path, progname, arg, (char *)NULL);
        perror("execlp"); // if exelp fails...
        _exit(1);
    }
    else{//Parent process
        printf("Waiting for the Child Process: (PID: %d)\n", pid);
        pid_t w = waitpid(pid, &status, 0); 

        if( w == -1){
            perror("waitpaid");
            return -1;
        }
    }

    if(WIFEXITED(status)){ //child ended normally?
        int returnVal = WEXITSTATUS(status); //childs return value
        printf("./%s Process finished (PID: %d). Returned %d\n", progname, pid, returnVal);
        return returnVal;
    }
    
    return -1; // if child failed to exit normally
}

int main(int argc, char *argv[]){
    if (argc != 2){
        fprintf(stderr, "Usage: %s <input.txt>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *fp = fopen(argv[1], "r"); // read file
    char line[BUF_SZ];
    char *endPointer;
    char argbuf[32];

    while(fgets(line, sizeof(line), fp) != NULL){
        line[strcspn(line, "\r\n")] = '\0'; //Read each line, remove \n characters
        if(line[0] == '\0') continue; // skip empty lines

        //assigment printing outline
        printf("===========================================\n");
        printf("Generator Process: Processing line \"%s\"\n", line);
        printf("===========================================\n");

        long n = strtol(line, &endPointer, 10);
        if (n < 1 || n > 20) {
            fprintf(stderr, "Generator: Number %ld out of allowed range [1,20]. Skipping.\n", n);
            continue;
        }

        // Run Oddeven w/ num 
        snprintf(argbuf, sizeof(argbuf), "%ld",n);
        int return1 = launch_and_wait("./OddEven", "OddEven", argbuf);
        // Uses OddEven return for PerfectSquare
        snprintf(argbuf, sizeof(argbuf), "%d",return1);
        int return2 = launch_and_wait("./PerfectSquare", "PerfectSquare", argbuf);
        // Uses PerfectSquare return for Factorial
        snprintf(argbuf, sizeof(argbuf), "%d",return2);
        launch_and_wait("./Factorial", "Factorial",argbuf);
    }

    fclose(fp);
    return EXIT_SUCCESS;
}


