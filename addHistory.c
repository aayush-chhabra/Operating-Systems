# include <string.h>

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