#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <wait.h>
#include <time.h>
#include "LineParser.h"

int main(int argc, char *argv[], char *envp[])
{
  char inputUser[1024];
  char name2[1024];
  char value2[1024];
  char *name = calloc(1, 1024);
  char *value = calloc(1, 1024);
  cmdLine *cmdUser;
  int i = 0;
  int deg = 0;
  int PID;
  int statusWait;
  time_t t;
  int rslWait = 0;
  char *name3;
  int child1;
  int child2;

  int fd[2];

  printf("%s\n", "pls enter an input and smile");
  fgets(inputUser, 2048, stdin);

  cmdUser = parseCmdLines(inputUser);
  child1 = fork();
  if (child1 == 0)
  {
    close(0);
    if (open(cmdUser->inputRedirect) == -1)
      fprintf(stderr, "Cannot open input end\n");
    close(1);
    if (open(cmdUser->outputRedirect) == -1)
      fprintf(stderr, "Cannot output end\n");
  }

  return 0;
}