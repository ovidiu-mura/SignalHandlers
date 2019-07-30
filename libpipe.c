/* Micah Burnett, CS410 ALSP, Prof. Mark Morrissey, 7/29/19
 * 
 * This routine demonstrates how to install your own signal handler
 * for the SIGPIPE signal and catch the signal yourself. A
 * SIGPIPE signal is raised when a process tries writing to a pipe
 * when all of the readers have been closed. So, to achieve this 
 * behavior, we wait for the child process to exit so that it has
 * no more readers open. Then we close the parents reader and write
 * to the pipe.
*/

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void
sigpipe(int signo)
{
  if (signo == SIGPIPE) {
    printf("received signal SIGPIPE\n");
  } else {
    fprintf(stderr, "received signal %d\n", signo);
  }
	//longjmp(sigpipe_buf, sigpipe_val);
}

int test_sigpipe(void) {
  struct sigaction sig;
 
	sig.sa_handler = sigpipe; 
  sigemptyset(&sig.sa_mask);
  sig.sa_flags = 0;
 
  // Install sigpipe signal handler
  if (sigaction(SIGPIPE, &sig, NULL) == -1) {
    perror("can't catch SIGPIPE");
    return -1;
  }
  
	int     fd[2];
	pid_t   pid;

	if (pipe(fd) < 0) {
		fprintf(stderr, "pipe error");
		return -1;
	}
	if ((pid = fork()) < 0) {
		fprintf(stderr, "fork error");
		return -1;

	// Parent
	} else if (pid > 0) {
			// Wait for child to exit and close our
			// own reader so that no more readers are open
			waitpid(0, NULL, 0);
			close(fd[0]);
			write(fd[1], "foo", 3);

	// Child
	} else { 
			return 0;
	}

  printf("process has returned from the signal handler\n");
  
	return 0;
}
