# include <stdio.h>
# include <stdlib.h>
# include <dirent.h>
# include <unistd.h>
# include <sys/wait.h>

# include "addHistory.c"

# define MAX_LENGTH 1000

char* searchInPath(char* command, char* environments);
// Syntax for Add History : addHistory(history, choice, &historyArrayLength, &historyArrayStartPosition);

int main(){
	
	char choice[1000], *getEnvironments, secondCharTest, exclaimationString[100], *tempString;
	char *cmdLineArgs, *allArgs[100], *historyPrint;
	int allArgsLength = 0, historyArrayLength = 0, historyArrayStartPosition = 0, historyInt;
	char history[MAX_LENGTH][1000];
	char * executableCommandFilePath;
	pid_t childForExecutingCommands, terminatedChild;
	int terminationStatus, errorCheck;

	//gets the environment from the environment variable $MYPATH
	getEnvironments = getenv("MYPATH");

	while(1){
		
		//gets the comands from the user.
		printf(">>> ");
		fgets(choice, 1000, stdin);
		rebuildCommand: 
		allArgsLength = 0;

		//store the command in the history and increment the length of the array by 1. If the length reaches 1000, increment the start
		//counter by 1.
		if(strncmp(choice,"!",1)!=0){
			addHistory(history, choice, &historyArrayLength, &historyArrayStartPosition);
		}

		//tokenize the string into an array and get the length of the args, also seperate the commands with the arguments.
		cmdLineArgs = strtok(choice," ");
		while(cmdLineArgs != NULL){
			allArgs[allArgsLength] = cmdLineArgs;
			cmdLineArgs = strtok(NULL, " ");
			allArgsLength++;
		}
		allArgs[allArgsLength-1] = trimwhitespace(allArgs[allArgsLength-1]);
		allArgs[allArgsLength] = NULL;
		allArgsLength++;
		
		//printf("%s\n",allArgs[0]);
		//checks if the user types in exit ot quit
		if(((strncmp(allArgs[0],"quit",4)==0)||(strncmp(allArgs[0],"exit",4)==0))&&(strlen(allArgs[0])==4)){
			printf("bye\n");
			exit(0);
		}

		

		if((strncmp(allArgs[0],"!",1)==0)){
			secondCharTest = allArgs[0][1];
			if(secondCharTest == '!'){
				//allArgs[0] = trimwhitespace(history[historyArrayLength-1]);
				//strcat(history[historyArrayLength-1], "\n");
				//strcpy(allArgs[0], history[historyArrayLength-1]);
				//addHistory(history, history[historyArrayLength-1], &historyArrayLength, &historyArrayStartPosition);
				strcpy(choice, history[historyArrayLength-1]);
				goto rebuildCommand;
				
			}
			else if(isalpha(secondCharTest)){
				//printf("Alphabet found\n");
				sscanf(allArgs[0], "!%s", &exclaimationString);
				if((tempString = searchHistory(history, historyArrayLength, historyArrayStartPosition, exclaimationString))!=NULL){
					strcpy(choice, tempString);
					goto rebuildCommand;
				}
				else{
					printf("Command not found.\n");
					continue;
				}

				//allArgs[0] = trimwhitespace(exclaimationString);
				//addHistory(history, history[historyArrayLength-1], &historyArrayLength, &historyArrayStartPosition);
				//printf("%s\n", exclaimationString);

				//allArgs[0] = trimwhitespace(history[historyArrayLength-1]);
				//strcat(history[historyArrayLength-1], "\n");
				//strcpy(allArgs[0], history[historyArrayLength-1]);
				//addHistory(history, history[historyArrayLength-1], &historyArrayLength, &historyArrayStartPosition);
				
			}
			else if(isdigit(secondCharTest)){
				sscanf(allArgs[0], "!%d", &historyInt);
				if(historyInt<historyArrayLength)
					strcpy(choice, history[historyInt]);
				else{
					printf("Out of bound.\n");
					continue;
				}
				goto rebuildCommand;
			}
		}
		

		if((strncmp(allArgs[0],"history",7)==0)&&(strlen(allArgs[0])==7)){
			
			if(historyArrayStartPosition == 1){
				for(int i=historyArrayLength; i<MAX_LENGTH; i++){
					historyPrint = trimwhitespace(history[i]);
					printf("%d %s\n",i, history[i]);
				}
				for(int i=0; i<historyArrayLength; i++){
					historyPrint = trimwhitespace(history[i]);
					printf("%d %s\n",i, history[i]);		
				}
			}
			else{
				for(int i=0; i<historyArrayLength; i++){
					historyPrint = trimwhitespace(history[i]);
					printf("%d %s\n",i, history[i]);	
				}
			}
			continue;
		}


		if((strncmp(allArgs[0],"cd",2)==0)&&(strlen(allArgs[0])==2)&&(allArgsLength-1 == 2)){
			errorCheck = chdir(allArgs[1]);
			if(errorCheck<0){
				perror("");
			}
			else{	
			}
			
			continue;	
		}
		if((strncmp(allArgs[0],"cd",2)==0)&&(strlen(allArgs[0])==2)&&(allArgsLength-1 != 2)){
			printf("Invalid number of arguments.\n");
			continue;
		}

		
		//printf("before !! %s ------------------------>>>>>\n", allArgs[0]);
		//get the command's executable path from the paths in env
		executableCommandFilePath = searchInPath(allArgs[0], getEnvironments);
		if(executableCommandFilePath != NULL){
			executableCommandFilePath = trimwhitespace(executableCommandFilePath);
			//printf("In here!! %s -----------------\n", allArgs[0]);
		}

		if(executableCommandFilePath == NULL)
			printf("Command not found %s", allArgs[0]);
		else{
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


	}
	return 0;
}
