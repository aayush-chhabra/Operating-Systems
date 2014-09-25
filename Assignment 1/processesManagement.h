/*

    processManagement.h v0.0.1 
    (c) 2014 Aayush Chhabra
	
	has the methods related to all the processes in the shell.
    
*/

# include <unistd.h>
# include <signal.h>
# include <sys/wait.h>
# include <fcntl.h>

void childTermination();
int foregroundProcess(char * executableCommandFilePath, char* allArgs[100], char * redirectToFilePath, int append);
int backgroundProcess(char * executableCommandFilePath, char* allArgs[100], char * redirectToFilePath, int append);