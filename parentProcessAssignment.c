# include <stdio.h>
# include <stdlib.h>
# include <dirent.h>
# include <unistd.h>
# include <sys/wait.h>

# include "addHistory.c"

# define MAX_LENGTH 1000

char* trimwhitespace(char *str);
char* searchInPath(char* command, char* environments);
// Syntax for Add History : addHistory(history, choice, &historyArrayLength, &historyArrayStartPosition);

int main(){
	
	char choice[1000], *getEnvironments;
	char *cmdLineArgs, *allArgs[100];
	int allArgsLength = 0, historyArrayLength = 0, historyArrayStartPosition = 0, historyInt;
	char history[MAX_LENGTH][1000];
	char * executableCommandFilePath;
	pid_t childForExecutingCommands, terminatedChild;
	int terminationStatus;

	//gets the environment from the environment variable $MYPATH
	getEnvironments = getenv("MYPATH");

	while(1){
		
		//gets the comands from the user.
		printf(">>> ");
		fgets(choice, 1000, stdin); 
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
		if(((strncmp(allArgs[0],"quit",4)==0)||(strncmp(allArgs[0],"exit",4)==0))&&(strlen(allArgs[0])==5)){
			printf("bye\n");
			exit(0);
		}

		if((strncmp(allArgs[0],"history",7)==0)&&(strlen(allArgs[0])==8)){
			
			if(historyArrayStartPosition == 1){
				for(int i=historyArrayLength; i<MAX_LENGTH; i++){
					printf("%d %s",i, history[i]);
				}
				for(int i=0; i<historyArrayLength; i++){
					printf("%d %s",i, history[i]);		
				}
			}
			else{
				for(int i=0; i<historyArrayLength; i++){
					printf("%d %s",i, history[i]);	
				}
			}
			continue;
		}

		if(strncmp(allArgs[0],"!",1)==0){
			sscanf(allArgs[0], "!%d", &historyInt);
			strcpy(allArgs[0], history[historyInt]);
			addHistory(history, history[historyInt], &historyArrayLength, &historyArrayStartPosition);
		}


		for(int i=0; i<allArgsLength; i++){
			printf("%d %s\n", i, allArgs[i]);
		}

		//get the command's executable from the path if it exists.
		executableCommandFilePath = searchInPath(allArgs[0], getEnvironments);
		if(executableCommandFilePath != NULL)
			executableCommandFilePath = trimwhitespace(executableCommandFilePath);

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



char * searchInPath(char *command, char* environments){
	
	char *environmentsTokenized[100], tempToReturnPath[1000];   //stores all the paths in a char *, and array of all the paths in the MYPATH variable
	char *environmentsSeperate;			//intermediate variable, which is of no use after the parsing of paths is done.
	int environmentsLength = 0;			//The length of the paths array. The total number of paths in MYPATH.
	DIR * dir;
	
	//get all the paths into an array.
	environmentsSeperate = strtok(environments, ":");
	while(environmentsSeperate != NULL){
		environmentsTokenized[environmentsLength] = environmentsSeperate;
		environmentsSeperate = strtok(NULL, ":");
		environmentsLength++;
	}


	for(int i=0; i<environmentsLength; i++){
		
		//open every directory mentioned in the MYPATH environment variable. 
		dir = opendir( environmentsTokenized[i] );

		//if opendir is not able to open the directory.
		if ( dir == NULL )
		{
			perror( "Cannot Open the directory mentioned in the Environment Variable" );
			return NULL;
		}

		struct dirent * file;
		
		while ( ( file = readdir( dir ) ) != NULL ){
			//printf("%lu %lu\n", strlen(command), strlen(file->d_name) );
			if((strncmp(command, file->d_name, strlen(command)-1)==0)&&(strlen(command) == strlen(file->d_name))){
				//if it finds the executable of the command in any of the folders, break.
				break;
			}
		}
		if(file == NULL){
			//printf("Command not found %s", command);
			return NULL;
		}
		else{
			
			//return file;
			strcpy(tempToReturnPath, environmentsTokenized[i]);
			strcat(tempToReturnPath, "/");
			strcat(tempToReturnPath, command);
			return tempToReturnPath;
		}

	}
	return NULL;
}

char *trimwhitespace(char *str)
{
	char *end;

	// Trim leading space
	while(isspace(*str)) str++;

	if(*str == 0)  // All spaces?
		return str;

	// Trim trailing space
	end = str + strlen(str) - 1;
	while(end > str && isspace(*end)) end--;

	// Write new null terminator
	*(end+1) = 0;

	return str;
}

