// Tacy Bechtel

// SIGTSTP

#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>


/***
* Uses a random number generator to determine behavior.
* If the number is even, the TSTP is ignored. Otherwise,
* Default TSTP behavior is used, and the process stops
* until its parent wakes it back up.
***/

void
sigtstp(int signo)
{
  printf("SIGTSTP received\n");
  int rando = rand();
  if (rando % 2 == 0)
  {
    printf("Ignoring the SIGTSTP\n");
  }
  else
  {
    printf("Stopping process %i\n", getpid());
    signal(SIGTSTP, SIG_DFL);
    raise(SIGTSTP);
  }
}

// restoring the correct signal handler for tstp
// in case it was reset to the default behavior.
void
sigcont(int signo)
{
  struct sigaction sa;
  sigfillset(&sa.sa_mask);
  sigdelset(&sa.sa_mask, SIGTSTP);
  sigdelset(&sa.sa_mask, SIGCONT);
  sa.sa_handler = &sigtstp;
  sigaction(SIGTSTP, &sa, NULL);
}

int
test_sigtstp(void)
{
  int count = 0;
  int pid;
  struct sigaction sa;

  sigfillset(&sa.sa_mask);
  sigdelset(&sa.sa_mask, SIGTSTP);
  sigdelset(&sa.sa_mask, SIGCONT);

  // first, setting the sigcont handler. This handler will not
  // be changed again, so I resused the struct.
  sa.sa_handler = &sigcont;
  sigaction(SIGCONT, &sa, NULL);

  // Make sure it's ready for the test
  sa.sa_handler = &sigtstp;

  if ((pid = fork()) < 0)
  {
    printf("Fork failure\n");
    return -1;
  }
  else if (pid == 0)
  {
    //set the TSTP handler the first time
    sigaction(SIGTSTP, &sa, NULL);
    while (1)
    {
      printf("Child running\n");
      sleep(1);
    }
  }
  else
  {
    printf("Parent running\n");
    while (count < 5) {
      sleep(5);
      printf("Sending SIGTSTP to child\n");
      kill(pid, SIGTSTP);
      sleep(3);
      kill(pid, SIGCONT);
      count++;
    }
    printf("Exiting\n");
    kill(pid, SIGKILL);
    wait(NULL);
  }
  return 0;
}
