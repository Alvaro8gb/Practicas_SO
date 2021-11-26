#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int my_system(char * comand){
	
	pid_t pid = fork();
	int ret;

	if (pid == 0) 
		ret = execl("/bin/bash", "/bin/bash","-c",comand,NULL);
		
	else
		wait(NULL);

	return ret; 
}


int main(int argc, char* argv[])
{
	if (argc!=2){
		fprintf(stderr, "Usage: %s <command>\n", argv[0]);
		exit(1);
	}

	return my_system(argv[1]);

	//return system(argv[1]);
}

