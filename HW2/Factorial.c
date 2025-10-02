#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>

/*
Factorial Criteria:
Prints the factorial of the number, and returns the factorial value if 
it is ≤ 255; otherwise returns n % 255.

*/

int main (int argc, char *argv[]){
    if (argc != 2){
        fprintf(stderr, "Factorial: Usage: %s <n>\n", argv[0]);
        return 1; //Not two args? Error!!!
    }

    int n = atoi(argv[1]); //convert string to num 
    pid_t pid = getpid();

    unsigned long long factorial = 1; 
     
    for(int i = 1; i <= n; ++i){
        factorial *= i;
    }

    printf("Factorial: %d! = %llu (PID: %d)\n ", n , factorial, pid);

    if(factorial <= 255ULL){ // Little confusing, still need to tweak..
        return (int)factorial & 0xFF;
    }
    else{
        return n % 255;
    }

}