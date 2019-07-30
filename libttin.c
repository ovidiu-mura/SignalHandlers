#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <wait.h>
#define SIZE 25

struct sigaction sa;
struct termios tc;

void
ttin_hdlr(int signo)
{
  const char* in = "SIGTTIN received\n";
  write(STDOUT_FILENO, in, strlen(in));
}

int
test_sigttin(void)
{
  char *input = NULL;
  pid_t pid;

  sa.sa_handler = &ttin_hdlr;
  sa.sa_flags = 0;
  sa.sa_flags |= SA_INTERRUPT;
  sigemptyset(&sa.sa_mask);

  if(sigaction(SIGTTIN, &sa, NULL) < 0){
    perror("sigaction error");
    return -1;
  }
  if((pid = fork()) < 0){
    perror("fork error");
    return -1;
  }
  else if(!pid){
    setpgid(0,0); // Child is now running as a background process
    read(STDIN_FILENO, input, SIZE); // Will cause SIGTTIN to be sent
    return 0;
  }

  wait(&pid);
  return 0;
}

