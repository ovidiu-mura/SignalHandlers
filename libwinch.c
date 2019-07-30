//Bradley Thompson
//CS410 prof. Mark Morrissey

//Handles SIGWINCH -- occurs when the window size changes.
//Found: http://man7.org/tlpi/code/online/dist/tty/demo_SIGWINCH.c.html
//  -> Used to help figure out how to test for the signal.


#include <errno.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <unistd.h>

void
sigwinch(int signo)
{
  if (signo == SIGWINCH)
    printf("SIGWINCH received.\n");
}

int
test_sigwinch()
{
  struct sigaction sa;
  struct winsize ws;

  sa.sa_handler = sigwinch;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  if (sigaction(SIGWINCH, &sa, NULL) == -1)
  {
    printf("Can't catch SIGWINCH\n");
    exit(errno);
  }
  
  printf("Waiting for SIGWINCH...\n");
  pause();

  if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == -1)
  {
    printf("ioctl couldn't get new window size.\n");
    exit(errno);
  } 
/* Looked up TIOCGWINSZ command for ioctl, result: 

    TIOCGWINSZ
    Fill in the winsize structure pointed to by the third argument 
    with the screen width and height. The winsize structure is 
    defined in `sys/ioctl.h' as follows:
 	
    struct winsize
    {
      unsigned short ws_row;	   rows, in characters 
      unsigned short ws_col;	   columns, in characters 
      unsigned short ws_xpixel;	 horizontal size, pixels 
      unsigned short ws_ypixel;	 vertical size, pixels 
    };
*/ 
  printf("SIGWINCH success, window resized to: %u(chars)x%u(chars)\n",
          ws.ws_row, ws.ws_col); 
  exit(EXIT_SUCCESS);
}
