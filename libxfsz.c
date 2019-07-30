/*SIGXFSZ This signal is generated if the process exceeds its soft file size limit; */
/*generating the signal is more interesting then handling it. */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFSIZE 100

/*signal handler for XFSZ */
void xfsz_handler()
{
     signal(SIGXFSZ,SIG_IGN);
     fprintf(stderr, "Caught SIGXFSZ: soft file size limit exceeded.\n");
     signal(SIGXFSZ, xfsz_handler); //reinstall the signal
}
void test_sigxfsz(void)
{
    //int n;
    //char buf[BUFFSIZE];
    struct rlimit fsizeLimit;
    char *data = "Hello,World!\n";
    FILE* fp = fopen("file.txt","w");
 
    fsizeLimit.rlim_cur=8; //soft-limit
    fsizeLimit.rlim_max=16; //hard-limit

/*RLIMIT_FSIZE The maximum size in bytes of a file that may be created.
When the soft limit is exceeded, the process is sent the
SIGXFSZ signal. */

    if(setrlimit(RLIMIT_FSIZE, &fsizeLimit) < 0)
    {
        perror("setrlimit error");
    }
    signal(SIGXFSZ, xfsz_handler);

    /* Write buffer to file. */
    if (fp){
    fwrite(data, strlen(data), 1, fp);
    	puts("Writting to file!");
    }
    else{  
        perror("write error");
    }
    fclose(fp);
    printf("done...\n");
}

