
You will have with two C files: MemoryManager.c and the Driver.c. The program will 
dynamically allocate and deallocate random sized arrays with no memory leaks.

Driver.c: It is responsible for: 

1. Setting the seed, whose value is passed as an argument, using srand(). 
 
2. Invoking functions in MemoryManager.c 


MemoryManager.c: It is responsible for implementing the following: 

1. Dynamically allocate and deallocate a random sized array for each iteration. 

2. Populate elements in the array with random integers. 

3. For each iteration, check all the elements in the array and determine whether each element 
is square or not, and if so, count it.  

4. Calculate the ratio of perfect squares to non-perfect squares given by  
Ratio = count of squares / count of non-perfect squares  

5. At the end, print the iteration number with maximum count of perfect squares. 

6. Return the average value of the ratio for all iterations to Driver.  