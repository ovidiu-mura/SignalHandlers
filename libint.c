/*
  Author:  Marie Escapita
  Date:    29 July 2019
  Class:   cs410_023 - Advanced Linux Systems Programming
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void
interupt_handler(int sig)
{
  signal(SIGINT, SIG_IGN);
  printf("\n^C Signal Caught\n");  // message can be omitted
  // Causes program to exit without success, like an error
  exit(EXIT_FAILURE);
}

void
test_sigint()
{
  signal(SIGINT, interupt_handler);
  int pid;
  pid = fork();

  // testing if it properly handles child programs
  if (pid == 0) {
    // child proc will spam the screen until the parent ends
    do {
      printf("x");
    } while(1);
  } else {
    // parent proc will sleep for a bit, then "recieve" the sigint
    // signal and run the handler.
    sleep(4);
    kill(pid, SIGINT);
  }
}
