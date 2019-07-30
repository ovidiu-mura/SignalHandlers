// Matthew Spohrer
// SIGCHLD
// 
// Gets child process to send SIGCHLD from exiting normally
// or from being termed by a signal (SIGKILL). 
//
// I also showed a child process exiting with SIGCHLD being
// ignored as is the default.
//
// I'm also not totally sure this is what was wanted from us.
#include <errno.h>
#include <error.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

pid_t
Fork()
{
  pid_t pid;
  if ((pid = fork()) < 0)
    error(EXIT_FAILURE, errno, "fork error");
  return pid;
}
  
void
sig_hnd(int signo)
{
  int errno_saved = errno;
  switch (signo) {
    case SIGCHLD:
      printf("----->SIGCHLD being received and being handled\n");
      sleep(1);
      break;
    default:
      printf("You caught the wrong thing!\n");
  }
  errno = errno_saved;
}

// child terminates via being sent SIGKILL
int
sig_term(void)
{
  int status, result, signo;
  pid_t pid = 0;


  printf("\nSignal Termination (via SIGKILL):\n");

  if((pid = Fork()) == 0) {
    // if child, spin-wait until kill signal sent from parent
    printf("In child, pid = %d, spin-waiting for SIGKILL\n", getpid());
    while(1);
    exit(EXIT_SUCCESS);
  }
  else {
    // if parent, send SIGKILL to child and report on status of child term
    sleep(1);
    printf("Sending child (%d) SIGKILL\n", pid);
    kill(pid, SIGKILL);
    waitpid(pid, &status, 0);

    // Macro used to show the child was termed by a signal
    if((result = WIFSIGNALED(status)) != 0) { 
      // Defines the signal doing the killing
      signo = WTERMSIG(status);

      printf("WIFSIGNALED(status) =  %d (1 means termed by a "
        "signal, otherwise 0)\nChild \"%s\" by signal number %d\n",
        result, sys_siglist[signo],signo);

      } else{ 
        printf("Uh-oh, you didn't do it right!\n");
        return -1;
      }
  }
  return 0;
}

// child terminates normally via a call to exit()
int
normal_term(void)
{
  int status = 0;
  char *result;
  pid_t pid = 0;

  printf("\nNormal Termination (SIGCHLD sent via exit()):\n");

  if((pid = Fork()) == 0) {
    // if child exit()
    printf("In child. PID = %d, Exiting Now\n", getpid());
    exit(EXIT_SUCCESS);
  }
  else {
    // if parent, wait for child exit and  comment on the status
    // of the child's termination
    printf("In parent waiting via waitpid for child: %d\n", pid);
    sleep(1);
    waitpid(pid, &status, 0);
    printf("Checking if child terminated via a call to exit()\n");
    
    // Macro to test if the child terminated via a call to exit
    if(WIFEXITED(status)) { 
      // if termed via exit, returns the exit status
      if(WEXITSTATUS(status) == 0)
        result = "EXIT_SUCCESS";
      else
        result = "EXIT_FAILURE";
      printf("Child Terminated via a call to exit():\n");
      printf("Exit status according to WEXITSTATUS(status): %s\n",
          result);
    } else{ 
      printf("Uh-oh, you didn't do it right!\n");
      return -1;
    }
  }
  return 0;
}

int
test_sigchld(void)
{
  struct sigaction sa, old;

  // block all signals except fot SIGCHLD cuz why not?!
  sigfillset(&sa.sa_mask);
  sigdelset(&sa.sa_mask, SIGCHLD);

  sa.sa_handler = &sig_hnd;
  if(sigaction(SIGCHLD, &sa, &old) == -1)
      perror("sigaction()");

  // child calls exit
  normal_term();
  // child killed by a signal
  sig_term();

  printf("\nReset handling SIGCHLD to the default of being "
      "ignored by the parent (or, at least, that was my "
      "parents' default).\n\n");

  // resets the signal mask to default
  if(sigaction(SIGCHLD, &old,NULL) == -1)
      perror("sigaction()");

  // child calls exit while the SIGCHLD is being ignored
  normal_term();

  printf("\nNote the process terminated normally but the "
      "signal was ignored.\n\nSIGCHLD was successfully handled"
      "when expected to do so!\n");
  return 0;
}
