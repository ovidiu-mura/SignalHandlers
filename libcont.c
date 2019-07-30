// Tacy Bechtel

// SIGCONT

#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>


void
sigcontt(int signo)
{
  printf("SIGCONT received\n");
}

int
test_sigcont(void)
{
  int count = 0;
  int pid;
  struct sigaction sa;

  sigfillset(&sa.sa_mask);
  sigdelset(&sa.sa_mask, SIGTSTP);
  sigdelset(&sa.sa_mask, SIGCONT);

  sa.sa_handler = &sigcontt;
  if ((pid = fork()) < 0) 
  {
    printf("Fork failure\n");
    return -1;
  }
  else if (pid == 0)
  {
    sigaction(SIGCONT, &sa, NULL);
    while (1)
    {
      printf("Child running\n");
      sleep(1);
    }
  }
  else
  {
    printf("Parent running\n");
    while (count < 3) {
      sleep(5);
      printf("Sending SIGTSTP to child\n");
      kill(pid, SIGTSTP);
      sleep(3);
      printf("Sending child Continue\n");
      kill(pid, SIGCONT);
      count++;
    }
    sleep(3);
    printf("Exiting\n");
    kill(pid, SIGKILL);
    wait(NULL);
  }
  return 0;
}
