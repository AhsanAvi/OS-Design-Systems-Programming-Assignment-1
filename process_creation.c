/* 
* Class: SYST44288, Fall 2021
* Assignment: Assignment #1 - Process Creation
* Date: Sept 21, 2021
* Program: process_creation.c
* Description: Processes identify themselves.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(void) {
    int parent = fork(); //creating a new parent process
    if (parent < 0) {     //fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    }  
    if (parent == 0) { //parent 
        int child = fork(); //creating new child process 
        if (child < 0) {     //fork failed; exit
            fprintf(stderr, "fork failed\n");
            exit(1);
        }     
        if (child == 0) { //child code
            //nothing needed to be done
        } else { //parent code
            printf("I am the parent, %d, parent of the %d child process\n", getpid(), child);
        }
    } else {   //grandparent goes into this path
        printf("I am grandparent, %d, parent of %d parent process. \n", getpid(), parent);
        wait(NULL); //waiting for child process to terminate
    } 
}