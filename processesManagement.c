# include "processesManagement.h"

void childTermination()
{
		int wstat;
		//union wait wstat;
		pid_t	pid;

		while (1) {
			pid = wait3 (&wstat, WNOHANG, (struct rusage *)NULL );
			if (pid == 0)
				return;
			else if (pid == -1)
				return;
			else
				printf ("[process %d completed]\n", pid);
		}

}

int foregroundProcess(char * executableCommandFilePath, char* allArgs[100], char * redirectToFilePath, int append){

	pid_t childForExecutingCommands, terminatedChild;
	int terminationStatus, file = 0, stdoutCopy = 0;
	
	if(redirectToFilePath != NULL){
		printf("append was %d\n", append);
		stdoutCopy = dup(1);
		close(1);
		if(append == 0){
			printf("Write your code here..\n");
			file = open( redirectToFilePath, O_RDWR | O_CREAT, 0660);	
		}
		else{
			file = open( redirectToFilePath, O_APPEND | O_WRONLY, 0660);
		}
		
	}
	
	printf( "found %s \n", executableCommandFilePath );

	childForExecutingCommands = fork();
				
	if(childForExecutingCommands<0){
		printf("Error, couldn't fork a child.\n");
	}

	if(childForExecutingCommands==0){
		printf("I am in Child!\n");
		execv(executableCommandFilePath, allArgs);
		printf("Exec troubles\n");
		return 12;
	}
	
	else{
		printf("In Parent, child's PID %d !\n", childForExecutingCommands);
		terminatedChild = wait( &terminationStatus );
		printf("Child Terminated %d \n", terminatedChild);
		if(file)
			close(file);	
		if (stdoutCopy)
			dup2(stdoutCopy, 1);
	}
	return 0;
}

int backgroundProcess(char * executableCommandFilePath, char* allArgs[100], char * redirectToFilePath, int append){

	pid_t childForExecutingCommands, terminatedChild;
	int terminationStatus;
	
	if(redirectToFilePath != NULL){
		printf("Write your code here..\n");


		// to do, all the redirection and stuff
	}
	


	signal (SIGCHLD, childTermination);
	printf( "found %s \n", executableCommandFilePath );

	childForExecutingCommands = fork();
				
	if(childForExecutingCommands<0){
		printf("Error, couldn't fork a child.\n");
	}

	if(childForExecutingCommands==0){
		printf("I am in Child!\n");
		execv(executableCommandFilePath, allArgs);
		printf("Exec troubles\n");
		return 12;
	}
	else{
		printf("[process running in background with pid %d]\n", childForExecutingCommands);
	}
	return 0;
}