# include "addHistory.h"


void addHistory(char history[][1000], char* choice, int *historyArrayLength, int *historyArrayStartPosition){
	
	if(*historyArrayLength >= 1000){
		*historyArrayStartPosition = 1;
		if((*historyArrayLength) % 1000 == 0){
			*historyArrayLength = 0;
		}
		strcpy(history[*historyArrayLength], choice);
		*historyArrayLength = *historyArrayLength + 1;		
	}
	
	else
	{
		strcpy(history[*historyArrayLength], choice);
		*historyArrayLength = *historyArrayLength + 1;	
	}
}

char * searchHistory(char history[][1000], int historyArrayLength, int historyArrayStartPosition, char exclaimationString[100]){
	int tempVar = -1;
	if (historyArrayStartPosition == 1) {
	    for (int i = historyArrayLength; i < 1000; i++) {
	        if(strncmp(exclaimationString, history[i], strlen(exclaimationString)) == 0){
	        	tempVar = i;
	        }
	    }
	    for (int i = 0; i < historyArrayLength; i++) {
	        if(strncmp(exclaimationString, history[i], strlen(exclaimationString)) == 0){
	        	tempVar = i;
	        }
	    }
	} 
	else {
	    for (int i = 0; i < historyArrayLength; i++) {
	        if(strncmp(exclaimationString, history[i], strlen(exclaimationString)) == 0){
	        	tempVar = i;
	        }
	    }
	}
	if(tempVar != -1)
		return history[tempVar];
	else
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

char * searchInPath(char *command, char* environments){
	

	
	char *environmentsTokenized[100], tempToReturnPath[1000];   //stores all the paths in a char *, and array of all the paths in the MYPATH variable
	char *environmentsSeperate;			//intermediate variable, which is of no use after the parsing of paths is done.
	int environmentsLength = 0;			//The length of the paths array. The total number of paths in MYPATH.
	DIR * dir;
	
	//get all the paths into an array.
	char tempEnvironment[1000];
	strcpy(tempEnvironment, environments);
	environmentsSeperate = strtok(environments, ":");
	while(environmentsSeperate != NULL){
		environmentsTokenized[environmentsLength] = environmentsSeperate;
		environmentsSeperate = strtok(NULL, ":");
		environmentsLength++;
	}

	// for(int i=0; i<environmentsLength; i++){
	// 	printf("%s\n", environmentsTokenized[i]);
	// }

	for(int i=0; i<environmentsLength; i++){
		
		//open every directory mentioned in the MYPATH environment variable. 
		dir = opendir( environmentsTokenized[i] );
		//printf("%s\n", environmentsTokenized[i]);

		//if opendir is not able to open the directory.
		if ( dir == NULL )
		{
			perror( "Cannot Open the directory mentioned in the Environment Variable" );
			return NULL;
		}

		struct dirent * file;
		
		while ( ( file = readdir( dir ) ) != NULL ){
			//printf("%lu %lu\n", strlen(command), strlen(file->d_name) );
			if((strncmp(command, file->d_name, strlen(command))==0)&&(strlen(command) == strlen(file->d_name))){
				//if it finds the executable of the command in any of the folders, break.
				break;
			}
		}
		if(file == NULL){
			//printf("back !! %s ------------------------>>>>>\n", command);
			//strcpy(environments, tempEnvironment);
			continue;
		}
		else{
			
			//return file;
			strcpy(tempToReturnPath, environmentsTokenized[i]);
			strcat(tempToReturnPath, "/");
			strcat(tempToReturnPath, command);
			strcpy(environments, tempEnvironment);
			return tempToReturnPath;
		}

	}
	strcpy(environments, tempEnvironment);
	return NULL;
}