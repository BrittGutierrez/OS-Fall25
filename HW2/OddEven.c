#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

/*
OddEven Criteria:
Prints whether the number(arg) is odd or even, 
then returns the number itself as exit status
*/

int main(int argc, char *argv[]){
    if(argc != 2){
        fprintf(stderr, "OddEven: Usage: %s <n>\n", argv[0]);
        return 1; // Not two args? Error!
    }

    int n = atoi(argv[1]); // convert string to int
    pid_t pid = getpid(); 

    if(n % 2 == 0){
        // EVEN
        printf("OddEven: %d is Even (PID: %d)\n", n, pid);
    }
    else{
        // ODD 
        printf("OddEven: %d is Odd (PID: %d)\n", n, pid);
    }

    //Return number itself, "8 bit only"
    return (n & 0xFF);
}