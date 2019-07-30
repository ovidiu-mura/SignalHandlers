/* Micah Burnett, CS410 ALSP, Prof. Mark Morrissey, 7/29/19
 * 
 * This routine demonstrates how to install your own signal handler
 * for the SIGFPE (Floating-Point Exception) signal and catch
 * the signal yourself.
 *
 * NOTE: The setjmp / longjmp calls are needed in order to recover
 * from the error. Otherwise, the behavior is undefined and will
 * generally result in an infinite loop!
*/

#include <signal.h>
#include <stdio.h>
#include <setjmp.h>

jmp_buf sigfpe_buf;
int sigfpe_val, sigfpe_flag = 0;

void
sigfpe(int signo, siginfo_t *siginfo, void *context)
{
  if (signo == SIGFPE) {
    printf("received signal SIGFPE: ");
  
		// All possible reasons that SIGFPE was raised
    switch(siginfo->si_code) {
      case FPE_INTDIV:
        printf("integer divide by zero\n");
        break;
      case FPE_INTOVF:
        printf("integer overflow\n");
        break;
      case FPE_FLTDIV:
        printf("floating-point divide by zero\n");
        break;
      case FPE_FLTOVF:
        printf("floating-point overflow\n");
        break;
      case FPE_FLTUND:
        printf("floating-point underflow\n");
        break;
      case FPE_FLTRES:
       printf("floating-point	inexact	result\n");
       break;
      case FPE_FLTINV:
        printf("invalid floating-point operation\n");
        break;
      case FPE_FLTSUB: 
        printf("subscript out of range\n");
      default:
        printf("unknown si_code.\n");
        break;
    }
  } else {
    fprintf(stderr, "received signal %d\n", signo);
  }
 	sigfpe_flag = 1; 
	longjmp(sigfpe_buf, sigfpe_val);
}


int test_sigfpe(void) {
  struct sigaction sig;
  
  sig.sa_sigaction = (void *)sigfpe;
  sigemptyset(&sig.sa_mask);
  sig.sa_flags = SA_RESTART | SA_SIGINFO;
 
  // Install sigfpe signal handler
  if (sigaction(SIGFPE, &sig, NULL) == -1) {
    perror("can't catch SIGFPE");
    return -1;
  }
  sigfpe_val = setjmp(sigfpe_buf);
 
	int val1 = 5, val2 = 0;
 	int retval = 0; 
  // Trigger divide-by-zero signal
	if (!sigfpe_flag) {
		printf("executing: 5/0...\n"); 
    retval = val1/val2;
	}

  printf("process has returned from the signal handler\n");
  
	return 0;
}
