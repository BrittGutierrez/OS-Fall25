Task Description 

You will be developing the three CPU scheduling methods listed below in this project. The first 
command line argument indicates the name of the file containing the list of processes to be used 
by your scheduling methods. The next command line argument specifies the time quantum (for 
Round Robin). You will use Python 3 for the programming language to implement these three 
scheduling techniques shown below (you are free to use Pandas and NumPy libraries if needed). 
Assume that all jobs are CPU bound (no I/O blocking) and that the context-switching time is 
minimal. Assume that in Round Robin, if a new process arrives at the same time as a process is 
switched out, the new process is placed first in the ready queue. 

● First Come First Serve (FCFS) 

● Shortest Job First with pre-emption (SJF-P) 

● Round Robin (RR) with the specified quantum. 

You must produce a Gantt chart for each scheduling algorithm to visualize the sequence of 
execution for each process (See the example output below). You will analyze the performance of 
these scheduling algorithms by tracking the turnaround time and wait time for each process by 
printing: the average waiting time, the average turnaround time, and the throughput after all 
processes have completed. 

● The turnaround time for a process is the difference between a job's submission and 
completion times. The average turnaround time reports the average for the set of processes 
that were scheduled. 

● The waiting time for a process reflects the total amount of time spent by that process in the 
ready queue. The average waiting time reports the average for the set of processes that were 
scheduled. 

● The throughput for a scheduling algorithm measures the total number of tasks processes 
per unit time.