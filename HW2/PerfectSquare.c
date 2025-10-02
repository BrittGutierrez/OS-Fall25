#include <stdio.h>    
#include <stdlib.h>   
#include <math.h>     
#include <sys/types.h>
#include <unistd.h> 

/*
Perfect Squares Criteria:
Prints whether the number is a perfect square, 
then returns the number itself.
*/

int main (int argc, char *argv[]){
    if (argc != 2){
        fprintf(stderr, "PerfectSquare: Usage: %s <n>\n", argv[0]);
        return 1; // Not two args? Error!!
    }

    int n = atoi(argv[1]); // convert string to num 
    pid_t pid = getpid(); 

    //Perfect square??
    int root = (int)(sqrt((double)n) + 0.5); // rount the squareroot

    if(root * root == n){
        //Perfect Square
        printf("PerfectSquare: %d is a Perfect Square (PID: %d)\n", n, pid);
    }
    else {
        //Not Perfect Square
        printf("PerfectSquare: %d is Not a Perfect Square (PID: %d)\n", n, pid);
    }

    //Return number itself, "8 bit only"
    return (n & 0xFF); 
}
