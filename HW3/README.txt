CS 370: Operating Systems Fall 25 

Homework 3: Programming Assignment 

Description of Task:

This assignment builds on the concepts of HW2. In this assignment, however, you will be tasked with 
creating processes that execute in parallel using both shared memory and pipes. To do so, you will 
create two files:  
1. Manager.c 
2. Palindrome.c 

The Manager (the parent) coordinates the tasks performed by Palindrome (the children). You will 
be asked to take n words from the command line and distribute the words to the children where 
they will each perform the same task with their own word. The words provided at the command line 
will be all lowercase for simplicity. In each child (Palindrome), you will determine if the word received 
is a valid palindrome and write the result to the Manager.   



The Manager behaves similarly to Generator in the previous assignment, but has the following 
new capabilities: 
1. Creation of unique shared memory segments for the Manager to share an individual word with 
each Palindrome instance and for each Palindrome instance to store results in.

2. Creation of a pipe for each Palindrome instance that provides it with the name of the shared 
memory segment created in step (1). The file descriptor (FD) of the pipe is passed as an 
additional argument to Palindrome. 

3. Palindrome processes run concurrently rather than sequentially. This means that the 
Manager will launch all the child processes and then start waiting for results. 
As in the previous assignment, each instance of the Palindrome will receive different arguments. 
As discussed above; to facilitate this, the Manager will take n command line arguments (the 
words to be validated). For instance, 
> ./Manager racecar kayak apple abcba hello 

Would create 5 child processes that would check each word in parallel. 



Palindrome requirements are below: 

1. It will take a command line argument that gives the FD of the pipe to read from.

2. Using the pipe FD, the Palindrome instance determines the name of the shared memory 
segment to read its given word from and store its result. 

3. Rather than returning the result of the check, the result is stored in the shared memory 
segment.


What to Submit:

Assignments should be submitted through Canvas. E-mailing the codes to the Professor, GTA, or the class 
accounts will result in an automatic 1 point deduction. 
Use the CS370 Canvas to submit a single .zip file that contains: 
• Manager.c and Palindrome.c (please document your code) 
• A Makefile that performs both a make clean as well as a make all (this will be provided in 
Teams) 

Filename Convention: Your manager and palindrome must be named Manager.c and Palindrome.c 
respectively; you can name additional .c and .h files anything you want. The archive file should be 
named as <LastName>-<FirstName>-HW3.zip. E.g. if you are Cameron Doe and submitting for 
HW3, then the zip file should be named Doe-Cameron-HW3.zip. 