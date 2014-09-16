#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

#define STDIN 0
#define STDOUT 1

int main()
{
  int fd[2];
  int pid;
  char *lschar[20]={"ls",NULL};
  char *morechar[20]={"less", NULL};
  pid = fork();
  if (pid == 0) {
    /* child */
    int cpid;
    pipe(fd);
    cpid = fork();
    if(cpid == 0) {
      //printf("\n in ls \n");
      dup2(fd[1], STDOUT);
      close(fd[0]);
      close (fd[1]);
      execvp("ls",lschar);
    } else if(cpid>0) {
      dup2(fd[0],STDIN);
      close(fd[0]);
      close(fd[1]);
      execvp("less", morechar);
    }
  } else if (pid > 0) {
    /* Parent */
    waitpid(pid, NULL,0);
  }
  return 0;
}