# include <string.h>

void addHistory(char history[][1000], char* choice, int *historyArrayLength, int *historyArrayStartPosition);
char *trimwhitespace(char *str);
char * searchInPath(char *command, char* environments);
char * searchHistory(char history[][1000], int historyArrayLength, int historyArrayStartPosition, char exclaimationString[100]);