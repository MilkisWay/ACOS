#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <error.h>
#include <errno.h>

int count = 0;

void funct(int nsig)
{
  pid_t pid;
  while ((pid = waitpid(-1,NULL,0)) > 0)
    {
      count--;
      printf("Process %d terminated\n", pid);
    }
}


int main(int argc,char* argv[])
{
  int maxx;

  if (argc != 2)
  {    printf("You need to write just !one! number \n");
       return -1;
  }
  else
  {
    maxx  = atoi(argv[1]);
  }

  char* arg[10];
  char buffer[100];

  signal(SIGCHLD,funct);

  while(fgets(buffer, 99, stdin))
  {
    if (count+1 > maxx)
  printf("maximum number of running programs has exceeded\n");
    else
    {
      int  i = 1;
      arg[0] = strtok(buffer, "\n");
      do
        {
          arg[i] = strtok(NULL, "\n");
          i++;
        }
      while(arg[i-1] != NULL);

      count++;
      if(fork() == 0)
      {
       execvp(arg[0],arg);
       printf("Execution error!\n");
       return 0;
      }
     }
     }
 return 0;
}
