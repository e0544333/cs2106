#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

int main()
{

    printf("Be patient, the program will take around 7 seconds to run.\n");
    printf("At the end you can do \"cat results.out\" to see the result.\n");

    //
    // Add code here to pipe from ./slow 5 to ./talk and redirect
    // output of ./talk to results.out
    // I.e. your program should do the equivalent of ./slow 5 | talk > results.out
    // WITHOUT using | and > from the shell.
    //

    // pipefd[0] Read end: Child is using the read end.
    // pipefd[1] Write end: Parent is using the write end.
    int pipefd[2];

    if (pipe(pipefd) < 0)
    {
        // The "perror" call prints the cause of the error to the screen
        perror("lab2p2f: ");
    }
    // Parent
    if (fork() != 0)
    {
        wait(NULL); // Wait for child
        int fp_out = open("./results.out", O_CREAT | O_WRONLY);
        close(pipefd[1]); // The end we are not using.
        dup2(pipefd[0], STDIN_FILENO);
        dup2(fp_out, STDOUT_FILENO);
        close(pipefd[0]);
        execlp("./talk", "talk", (char *)0);
        close(fp_out);
    }
    // Child
    else
    {
        close(pipefd[0]); // The the end we are not using.
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execl("./slow", "slow", "5", NULL);
    }
}
