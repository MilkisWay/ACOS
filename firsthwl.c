#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char* argv[], char* envp[])
{
	if (argc !=2) {
		printf("Incorrect number of arguments!\n");
		return 1;
	}

	FILE *f = fopen(argv[1],"r");
	if (f == NULL) {
		perror("Incorrect imput or empty file!\n");
		return 2;
	}


	char str[FILENAME_MAX]={};
	int time = 0;
	while ( fscanf(f,"%d%s", &time,str) == 2 )
	{
		if(time < 0)
		{
			printf("Incorrect time!\n");
			return 3;
		}
		
		pid_t pid = fork();
		if (pid==-1){
			perror("Problem in fork!\n");
			return 4;
		}
		else if(pid==0){
			sleep(time);
			execlp(str,str,NULL);
		}
	}

	while(wait(NULL)!=-1){};

	if(!feof(f))
	{
		perror("File is unreadable!\n");
		return 5;
	}
	fclose(f);
	return 0;
}
