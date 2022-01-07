/* 
* Class: SYST44288, Fall 2021
* Assignment: Assignment #1 - Process Communication
* Date: Sept 21, 2021
* Program: Assignment 1, Q1.2: process_communication.c
* Description: Communication between processes
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#define SIZE 1024

int main(void) {
    int pfd[2], nread;
    char buf[SIZE];
    if (pipe(pfd) == -1) {
        perror("pipe failed");
        exit(1);
    }
    int parent = fork(); //creating a parent process
    if (parent < 0) {     //fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    }  
    if (parent == 0) { 
        int child = fork(); //creating grandchild process 
        if (child < 0) {     //fork failed; exit
            fprintf(stderr, "fork failed\n");
            exit(1);
        }     
        if (child == 0) { //child code
            nread = read(pfd[0], buf, SIZE); //reading message from parent
            printf("Child Reads: %s\n\n", buf);

            strcpy(buf, "Yes, I would love that."); 
            write(pfd[1], buf, strlen(buf)+1); //writing to parent
            printf("Child %d --> Parent\n", getpid());
        } else { //parent code
            nread = read(pfd[0], buf, SIZE); //reading grandfather's message 
            printf("Parent Reads: %s\n\n", buf);
            strcpy(buf, "Grandfather will pick you up today, you will bake cakes! Would you like that?");
            write(pfd[1], buf, strlen(buf)+1); //writing to child
            
            printf("Parent %d --> Child\n", getpid());
            wait(NULL); //waiting for child process to write back
            nread = read(pfd[0], buf, SIZE); //reading message from child
            printf("Parent Reads: %s\n\n", buf);
            strcpy(buf, "She says she would love to!"); 
            write(pfd[1], buf, strlen(buf)+1); //writing child message to Grandfather
            printf("Parent %d --> Grandfather\n", getpid());
            
        }
    } else {   //grand parent goes into this path
        printf("Grandfather %d --> Parent\n", getpid());
        strcpy(buf, "Tell my beautiful granddaughter that I will pick her up for us to bake a cake");
        write(pfd[1], buf, strlen(buf)+1); //writing message to parent to pass onto child
        wait(NULL); //waiting for parent process to write back

        nread = read(pfd[0], buf, SIZE); //reading what parent sent from child
        printf("Grandfather Reads: %s\n", buf);
    }
}
