# include <stdio.h>
# include <unistd.h>
# include <sys/wait.h>

int main(){
	pid_t pid, pid_terminate;
	int status;
	char *args[] = {"ls", "-al"};
	pid = fork();

	if(pid<0){
		printf("Error forking a child!!\n");
	}

	if(pid==0){
		printf("I am in Child!\n");
		execv("/bin/ls", args);
		return 12;
	}
	else{
		printf("In Parent, child's PID %d !\n", pid);

		pid_terminate = wait( &status );
		printf("Child Terminated %d \n", pid_terminate);

	}
}