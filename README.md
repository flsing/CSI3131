# CSI3131
Operating Systems

Winter 2018

Languages: C, Java

Felix Singerman

Labs, and assignments for this course. This is for educational purposes only. Beware that there may contain mistakes.

## Assignment1
### Process Creation and Inter-Process Communication
Practice process creation and communication in Linux using fork() and exec() system calls. Wrote a C program dp (standing for double pipe) with the following command syntax: dp program1 <arglist1> : program2 <arglist2> : program3 <arglist3>. 
1. program2 <arglist2> is executed with the standard input coming from the standard output of program1 <arglist1>
2. program3 <arglist3> is executed with the standard input coming from the standard output of program1 <arglist1>
3. program2 and program3 do not divide the output of program1 among themselves, they both get the full output of program1

## Assignment 2
### Semaphore Usage
Several synchronization constraints apply. Simulated the work being completed by the professor, TA, and student, using the Java Semaphore Class. 

## Assignment 3
### Memory Management- Paging Algorithms
Java based simulator of a memory management system. Based on the simulator from http://www.ontko.com/moss/ - memory. Implemented LRU and CLOCK algorithms. Paging.java (programming part) and Discussion.docx (explanation of experimental results)
