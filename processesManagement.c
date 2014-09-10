# include <unistd.h>
# include <signal.h>
# include <sys/wait.h>


void childTermination()
{
		
		union wait wstat;
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

int foregroundProcess(char * executableCommandFilePath, char* allArgs[100]){

	pid_t childForExecutingCommands, terminatedChild;
	int terminationStatus;
	
	
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

	}
}

int backgroundProcess(char * executableCommandFilePath, char* allArgs[100]){

	pid_t childForExecutingCommands, terminatedChild;
	int terminationStatus;
	
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
}