#include "LineParser.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <wait.h>
#include <time.h>

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;
//#include <sys⁄types.h>
typedef struct node
{
  char *name;
  char *value;
} node;

typedef struct link linknodes;
struct link
{
  linknodes *next;
  node *nod;
};

#define STDERR 2

char *findPath()
{
  long size;
  char *path;
  size = pathconf(".", _PC_PATH_MAX);
  path = (char *)malloc((size_t)size);
  getcwd(path, (size_t)size);
  return path;
}

int execute(cmdLine *pCmdLine)
{
  //int rtn =execvp(pCmdLine->arguments[0], pCmdLine->arguments);
  int rtn = fork();
  if (rtn == 0)
  {
    printf("Child\n");
    if (execvp(pCmdLine->arguments[0], pCmdLine->arguments) < 0)
      perror("Error In Execution");

    //freeCmdLines(pCmdLine);
    printf("Child close\n");

    exit(1);
  }
  else
    printf("Parent\n");

  //freeCmdLines(pCmdLine);
  return rtn;
}
/*
void cdPrev()
{
  int err = chdir("..");
  if (err != 0)
  {
    perror("chdir() failed!!! with error: ");
    perror(err);
  }
}
*/
linknodes *addValue(linknodes *myList, char *name, char *value)
{
  if (myList == NULL)
  {
    linknodes *newNode = calloc(1, sizeof(linknodes));
    newNode->nod = calloc(1, sizeof(node));
    newNode->nod->name = calloc(1, sizeof(name));
    newNode->nod->value = calloc(1, sizeof(value));
    strcpy(newNode->nod->name, name);
    strcpy(newNode->nod->value, value);
    newNode->next = NULL;
    return newNode;
  }
  else
  {
    linknodes *cur = myList;
    while (cur->next != NULL)
      cur = cur->next;
    linknodes *newNode = calloc(1, sizeof(linknodes));
    newNode->nod = calloc(1, sizeof(node));
    newNode->nod->name = calloc(1, sizeof(name));
    newNode->nod->value = calloc(1, sizeof(value));
    strcpy(newNode->nod->name, name);
    strcpy(newNode->nod->value, value);
    newNode->next = NULL;
    cur->next = newNode;
    return myList;
  }
}

int isExisted(linknodes *myList, char *name)
{
  linknodes *cur = myList;
  char test[1024];
  while (cur != NULL)
  {
    strcpy(test, cur->nod->name);
    if (strcmp(test, name) == 0)
      return 1;
    cur = cur->next;
  }
  return 0;
}
linknodes *changeExisted(linknodes *myList, char *name, char *value)
{
  linknodes *cur = myList;
  char test[1024];
  while (cur != NULL)
  {
    strcpy(test, cur->nod->name);
    if (strcmp(test, name) == 0)
    {
      cur->nod->value = value;
      return myList;
    }
    cur = cur->next;
  }
  return myList;
}

void printAll(linknodes *myList)
{
  int i = 1;
  linknodes *cur = myList;
  while (cur != NULL)
  {
    printf("Node Number: %d\n", i);
    printf("Node Name: %s\n", cur->nod->name);
    printf("Node Value: %s\n\n", cur->nod->value);
    i++;
    cur = cur->next;
  }
}

char *nameToReplace(char *fname, linknodes *myList)
{

  char newname[1024];
  strcpy(newname, fname + 1);
  linknodes *cur = myList;
  char test[1024];
  while (cur != NULL)
  {
    if ((strcmp(newname, cur->nod->name)) == 0)
      return cur->nod->value;
    cur = cur->next;
  }
  return fname;
}

cmdLine *supportPipe(cmdLine *cmd)
{
  int j;
  int PID2;
  int PID1;
  cmdLine *newcmd;
  int statusWait;
  int copy_desc1;
  int copy_desc0;
  int rtn;
  int fd[2];

  newcmd = cmd->next;
  rtn = open(cmd->next->arguments[0]);
  if (pipe(fd) == -1) //check if
  {
    fprintf(stderr, "Pipe Failed");
    return 1;
  }

  //replaceCmdArg(newcmd, 0, cmd->arguments[0]);
  PID2 = fork();
  if (PID2 == 0)
  {
    close(1);
    //rtn = open(cmd->next->arguments[0]);
    copy_desc1 = dup(fd[1]);
    close(fd[1]); //what to  close
    printf(stderr, "%d\n", copy_desc1);
    printf(stderr, "%s\n", "child1\n");
  }

  close(fd[1]);
  PID1 = fork();
  if (PID1 == 0)
  {
    close(0);
    copy_desc0 = dup(fd[0]);
    close(fd[0]); //what to  close
    fprintf(stderr, "%d\n", copy_desc0);
    printf(stderr, "%s\n", "child111\n");
    execvp(newcmd->arguments[0], newcmd->arguments);
    printf(stderr, "%s\n", "child222\n");
  }

  close(fd[0]);
  //close(fd[1]);

  waitpid(PID2, statusWait, WEXITED);
  waitpid(PID1, statusWait, WEXITED);
  printf(stderr, "%s\n", "parent2\n");
}

int main(int argc, char *argv[], char *envp[])
{

  //task 4

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
  /*link *myList=(linked_list *)malloc((size_t)(sizeof(linked_list)));*/
  linknodes *myList;
  char *name3;
  char *path;
  long size;
  size = pathconf(".", _PC_PATH_MAX);
  path = (char *)malloc((size_t)size);
  /*
  for (i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-D") == 0)
      deg = 1;
    if (strncmp(argv[i], "-a", 2)==0)
    {
      changes = 1;
      prename = argv[i] + 2;
    }
  }
  */

  printf("%s\n", "pls enter an input and smile");
  fgets(inputUser, 2048, stdin);

  while (strncmp(inputUser, "quit", 4) != 0)
  {

    if (strncmp(inputUser, "set", 3) == 0)
    {
      strcpy(name, strchr(inputUser, ' ') + 1);
      strcpy(value, strchr(name, ' ') + 1);
      int lenValue = strlen(value);
      int lenName = strlen(name);
      strncpy(name2, name, lenName - lenValue - 1);
      strncpy(value2, value, lenValue - 1);
      if (isExisted(myList, name2) == 1)
        myList = changeExisted(myList, name2, value2);
      else
        myList = addValue(myList, name2, value2);
    }
    else if (strncmp(inputUser, "vars", 4) == 0)
      printAll(myList);
    else
    {
      cmdUser = parseCmdLines(inputUser);

      if (strstr(inputUser, "|") != NULL)
        cmdUser = supportPipe(cmdUser);
      else
      {
        int j;
        for (j = 0; j < cmdUser->argCount; j++)
          if (strncmp(cmdUser->arguments[j], "$", 1) == 0)
            if (isExisted(myList, cmdUser->arguments[j] + 1) == 1)
            {
              name3 = nameToReplace(cmdUser->arguments[j], myList);
              replaceCmdArg(cmdUser, j, name3);
            }
        if (strncmp(cmdUser->arguments[0], "cd", 2) == 0)
        {
          if (strncmp(cmdUser->arguments[1], "~", 1) == 0)
          {
            int err = chdir(getenv("HOME"));
            if (err != 0)
            {
              perror("chdir() failed!!! with error: ");
              perror(err);
            }
          }
        }
        else
        {
          PID = execute(cmdUser);
          if (deg == 1)
          {
            fprintf(stderr, "PID: %d\n", PID);
            fprintf(stderr, "Executing command: %s\n", inputUser);
          }
          if (cmdUser->blocking == 1)
          {
            if ((rslWait = waitpid(PID, statusWait, WUNTRACED)) == -1)
              perror("wait() error");
            else if (rslWait == 0)
            {
              time(&t);
              printf("child is still running at %s", ctime(&t));
              sleep(1);
            }
          }
        }
      }
    }

    getcwd(path, (size_t)size);
    printf("The path is: ");
    printf(path);
    printf("\n");

    printf("%s\n", "pls enter an input and smile");
    fgets(inputUser, 2048, stdin);
  }
  return 0;
}