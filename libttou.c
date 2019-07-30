#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <wait.h>

struct sigaction sa;
struct termios tc;

void 
ttou_hdlr(int signo)
{
  const char* out = "SIGTTOU received\n";
  signal(SIGTTOU, SIG_IGN); // Resets signal disposition so SIGTTOU not received for following write
  write(STDOUT_FILENO, out, strlen(out));
}

int
test_sigttou(void)
{
  const char* msg = "Attempting to write to terminal from background process";
  pid_t pid; 

  sa.sa_handler = &ttou_hdlr;
  sa.sa_flags = 0;
  sa.sa_flags |= SA_INTERRUPT;
  sigemptyset(&sa.sa_mask);

  if(sigaction(SIGTTOU, &sa, NULL) < 0){
      perror("Error setting sigaction");
      return -1;
  }
  if((pid = fork()) < 0){
    perror("fork error");
    return -1;
  }
  else if(!pid){
    setpgid(0,0);   // Child running as background process
    tcgetattr(STDOUT_FILENO, &tc);
    if(tc.c_lflag != TOSTOP){
      tc.c_lflag = TOSTOP;
      tcsetattr(STDOUT_FILENO, 0, &tc);
    }
    printf("%s\n", msg); // Will print after returning from signal handler
    return 0;
  }
  
  wait(&pid);
  return 0;
}
