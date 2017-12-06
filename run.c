/*Name : Sourav Pal
  Roll No. : 14CS10062
  Assignment No. : 2
  Question No. : 2
  Date : 30/01/2017
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
	int pid, status;
	pid = fork();
	if(pid==0)
	{
		//Child Process
		//simply execs using ./shell
		char * childarg[2];
		childarg[0] = (char *)malloc(strlen("./shell")+1);
		strcpy(childarg[0],"./shell");
		childarg[1] = NULL;
		execvp("./shell",childarg);
		perror("execvp failed : ");
		printf("\n");
		exit(-1);
	}
	else
	{
		//Parent Process
		waitpid(pid, &status, 0);
	}
	return 0;
}
