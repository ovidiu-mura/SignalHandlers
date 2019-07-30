// Author: Ovidiu Mura
// CS510 - ALSP
// Date: July 27, 2019

// ALARM signal handler example

#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

long main_count = 0; // counter to be incremented inside main method
int acount = 0; // counter to be incremented inside alarm handler
int stop = 0; // flag to terminate the execution

time_t t; // timer to measure the time between handler execution


// ALARM Signal Handler
void
alarm_handler(int sig)
{
  signal(SIGALRM, SIG_IGN);  // set the ALARM signal to be ignored
  time(&t);
  printf("\nTime: %s", ctime(&t));  // print the time the handler is run
  acount++;                         // increment the alarm counter, number the alarm handler to run is 5
  printf("SIGALRM received: %s. Signal number is %d.\n", strsignal(sig), sig); // identify the signal
  printf("SIGALRM is received %d time(s).\n", acount);  // count the number the alarm handler is executed
  printf("Counter in 'main' function is: %ld\n", main_count); // print counter from main method
  if(acount == 5) // the alarm handler is executed 5 times, then it stops
    stop=1;
  if((main_count % 2) == 0) // reset the alarm for the next run
  {
    printf("Alarm is reset to 1 second.\n");
    alarm(1); // set an alarm clock for delivery of a signal
  }
  else
  {
    printf("Alarm is reset to 2 seconds.\n");
    alarm(2); // set an alarm clock for delivery of a signal
  }
  signal(SIGALRM, alarm_handler);  // reinstall the handler
}


int
test_sigalrm(void)
{
  main_count = 0;
  signal(SIGALRM, alarm_handler);
  alarm(1); // set an alarm clock for delivery of a signal
  while(stop == 0){ // increment the counter until stop == 1, then it terminates
    main_count++;
  }
  printf("------ done ------\n");
  if(acount == 5)
    return 0; // On Success return 0, test passed successfully.
  else
    return -1; // On Failure return -1, test failed.
}
