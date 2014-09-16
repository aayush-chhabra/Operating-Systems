# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

int main(){

	int p[2], status;
	pid_t childTerm;
	char *lschar[20]={"ls",NULL};
  	char *morechar[20]={"less", NULL};

	pid_t child = fork();

	if(child<0){
		printf("Error!!\n");
	}

	if(child == 0){
		printf("In child\n");
		pipe(p);

		pid_t childInter = fork();
		if(childInter<0) printf("Error!!\n");

		if(childInter == 0){
			printf("Child's Child\n");
			dup2(p[1], 1);
			close(p[1]);
			close(p[0]);
			execvp("ls", lschar);
		}
		else{
			printf("Child II \n");
			pid_t a = wait(&status);
			dup2(p[0], 0);
			close(p[1]);
			close(p[0]);
			execvp("less", morechar);

		}
		return 0;

	}

	waitpid(childTerm, &status, WUNTRACED);

}