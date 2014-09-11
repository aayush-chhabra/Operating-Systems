# include <unistd.h>
# include <signal.h>
# include <sys/wait.h>

void childTermination();
int foregroundProcess(char * executableCommandFilePath, char* allArgs[100], char * redirectToFilePath);
int backgroundProcess(char * executableCommandFilePath, char* allArgs[100], char * redirectToFilePath);