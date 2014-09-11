# include <stdio.h>
# include <stdlib.h>
# include <dirent.h>

# include "processesManagement.c"
# include "addHistory.c"

# define MAX_LENGTH 1000


int main(){
	
	char choice[1000], *getEnvironments, secondCharTest, exclaimationString[100], *tempString;
	char *cmdLineArgs, *allArgs[100], *historyPrint;
	int allArgsLength = 0, historyArrayLength = 0, historyArrayStartPosition = 0, historyInt;
	char history[MAX_LENGTH][1000], *redirectToFilePath;
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
		redirectToFilePath = NULL;
		allArgsLength = 0;
		signal(SIGCHLD, SIG_DFL);
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

		for(int i = 0; i<allArgsLength-1; i++){
			if(strncmp(allArgs[i], ">", strlen(allArgs[i])) == 0){
				printf("Redirect to file.\n");

				if(allArgs[i+1]!=NULL){
					redirectToFilePath = (char *)malloc(sizeof(allArgs[i+1]));	
					strcpy(redirectToFilePath, allArgs[i+1]);
				}
				printf("%s", allArgs[i]);	
				allArgsLength = allArgsLength - 2;
				allArgs[allArgsLength-1] = NULL;
				for(int i=0; i<allArgsLength; i++){
					printf("%s\n", allArgs[i]);
				}
				
			}
			else if(strncmp(allArgs[i], "<", strlen(allArgs[i])) == 0){
				printf("Redirect from file.\n");
				
			}
			else if(strncmp(allArgs[i], ">>", strlen(allArgs[i])) == 0){
				printf("Append to File\n");
				
			}
		}
		

		if((strncmp(allArgs[0],"!",1)==0)){
			
			secondCharTest = allArgs[0][1];
			
			if(secondCharTest == '!'){
				strcpy(choice, history[historyArrayLength-1]);
				goto rebuildCommand;
				
			}

			else if(isalpha(secondCharTest)){
				sscanf(allArgs[0], "!%s", &exclaimationString);
				if((tempString = searchHistory(history, historyArrayLength, historyArrayStartPosition, exclaimationString))!=NULL){
					strcpy(choice, tempString);
					goto rebuildCommand;
				}
				else{
					printf("Command not found.\n");
					continue;
				}
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

		//get the command's executable path from the paths in env
		executableCommandFilePath = searchInPath(allArgs[0], getEnvironments);
		if(executableCommandFilePath != NULL){
			executableCommandFilePath = trimwhitespace(executableCommandFilePath);

		}

		if(executableCommandFilePath == NULL)
			printf("Command not found %s", allArgs[0]);
		else{

			if(strncmp(allArgs[allArgsLength-2], "&", 1)==0){
				
				printf("Background process!!\n");
				allArgs[allArgsLength-2] = NULL;
				--allArgsLength;


				backgroundProcess(executableCommandFilePath, allArgs, redirectToFilePath);
				
			}
			else{
				
				foregroundProcess(executableCommandFilePath, allArgs, redirectToFilePath);
			}
			
		}


	}
	return 0;
}
