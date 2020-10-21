#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <wait.h>
#include <time.h>

int main(int argc, char *argv[], char *envp[])
{
  int fd[2];
  int child1;
  int child2;
  int copy_desc2;
  int statusWait;
  int copy_desc1;
  int deg = 0;
  int i;
  char *cmdArray[1024];
  for (i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-d") == 0)
      deg = 1;
  }

  if (pipe(fd) == -1)
  {
    fprintf(stderr, "Pipe Failed");
    return 1;
  }
  if (deg == 1)
    fprintf(stderr, "parent_process>forking…\n");
               // copy_desc1 = dup(fd[1]);

  child1 = fork();
  if (deg == 1)
    fprintf(stderr, "parent_process>created process with id: \n", child1);
              if (child1 == 0)
              {
                if (deg == 1)
                  fprintf(stderr, "child1>redirecting stdout to the write end of the pipe…\n");
                close(1);
                copy_desc1 = dup(fd[1]);
                close(fd[1]); //what to  close
                char *argnew[1];
                argnew[0] = "ls";
                argnew[1] = "-l";
                if (deg == 1)
                  fprintf(stderr, "(child1>going to execute cmd: …)\n");
                execvp(argnew[0], argnew);
              }
  
    if (deg == 1)
      fprintf(stderr, "parent_process>closing the write end of the pipe…\n");
    close(fd[1]);
    child2 = fork();
              if (child2 == 0)
              {

                if (deg == 1)
                  fprintf(stderr, "child2>redirecting stdin to the read end of the pipe…\n");
                close(0);
                copy_desc2 = dup(fd[0]);

                close(fd[0]);   //what to  close
                char *argnew[2];
                argnew[0] = "tail";
                argnew[1] = "-n";
                argnew[2] = "2";
                if (deg == 1)
                  fprintf(stderr, "(child2>going to execute cmd: …)\n");

                execvp(argnew[0], argnew);
              }
    if (deg == 1)
      fprintf(stderr, "parent_process>closing the read end of the pipe…\n");
    close(fd[0]);
    if (deg == 1)
      fprintf(stderr, "parent_process>waiting for child processes to terminate…\n");

    waitpid(child1, statusWait, WEXITED);
    waitpid(child2, statusWait, WEXITED);
    if (deg == 1)
      fprintf(stderr, "parent_process>exiting…\n");
    exit(1);
  return 0;
}