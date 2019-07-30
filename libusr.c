/* Sam Shippey
 * SIGUSR1, SIGUSR2
 *
 * The user defined signals.
 * Shows how to catch them and how to send them.
 *
 * Written based off Jeff Lund's SIGSEGV, Matthew Spohrer's SIGCHLD tests.
 */

#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <signal.h>

void
usrdefsignal_handler(int signo)
{
  int errno_saved = errno;
  if (signo == SIGUSR1)
      printf("We've caught the first user-defined signal, SIGUSR1.\n");
  else if (signo == SIGUSR2)
      printf("We've caught the second user-defined signal, SIGUSR2.\n");
  errno = errno_saved;
}

// Purely for testing purposes
int 
test_userdefsignals()
{
  struct sigaction sa, saved;
  sa.sa_handler = &usrdefsignal_handler;
  
  if (sigaction(SIGUSR1, &sa, &saved) < 0)
    return 1;
  if (sigaction(SIGUSR2, &sa, &saved) < 0)
    return 1;
  // Note that the way to actually cause these to happen is to raise them or send them with
  // kill. They aren't generated from anything else.
  raise(SIGUSR1);
  raise(SIGUSR2);
  return 0;
}
