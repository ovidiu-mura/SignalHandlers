/****************************
 * Jeff Lund
 * ALSP - SIGSEGV testing
 * **************************/

#include <stdio.h>
#include <signal.h>
#include <setjmp.h>

static volatile sig_atomic_t addrtest = 1;
static volatile sig_atomic_t acctest = 1;
static sigjmp_buf jmpbuf;

void
sigsegv(int signo, siginfo_t *info, void *context)
{
    printf("SIGSEGV recieved");
    if(info->si_code & SEGV_MAPERR) {
        printf(" - Address 0x%lx not mapped to object", (long)info->si_addr);
        addrtest = 0;
    }
    if(info->si_code & SEGV_ACCERR) {
        printf(" - Invalid access permissions");
        acctest = 0;
    }
    printf(".\n"); 
    siglongjmp(jmpbuf, 1);
}

int
test_sigsegv(void)
{
    int *deref = NULL; 
    char *access = "access";
    sigset_t mask;
    struct sigaction sa, old;

    sigfillset(&mask); 
    sa.sa_sigaction = &sigsegv;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_mask = mask;

    if(sigaction(SIGSEGV, &sa, &old) < 0) {
        return -1;
    }
    sigsetjmp(jmpbuf, 1);
    // Test address mapping error, dereferencing a null pointer.
    if(addrtest) {
        printf("%d\n", *deref);
    }
    // Test access permission error. char * is read-only, trying to write to the array will trigger a SIGSEGV.
    if(acctest) {
        *access = 'A';
    }

    if(sigaction(SIGSEGV, &old, NULL) < 0) {
        return -1;
    }

    return 0;
}
