#ifndef __TESTSIGNAL_H__
#define __TESTSIGNAL_H__

//SIGALRM
int	test_sigalrm(void);

//SIGCHLD
int 	test_sigchld(void);

//SIGXFSZ
void	test_sigxfsz(void);

//SIGPIPE
int		test_sigpipe(void);

//SIGFPE
int 	test_sigfpe(void);

//SIGSEGV
int	  test_sigsegv(void);

//SIGCONT
int		test_sigcont(void);

//SIGSTOP
int		test_sigtstp(void);

//SIGUSR1, SIGUSR2
int		test_userdefsignals(void);

//SIGINT
void	test_sigint(void);

//SIGWINCH
void	test_sigwinch(void);

//SIGTTIN
void	test_sigttin(void);

//SIGTTOU
void	test_sigttou(void);

#endif
