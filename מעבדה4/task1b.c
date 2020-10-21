#include "util.h"

#define SYS_WRITE 4
#define SYS_READ 3
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_LSEEK 19
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define RDO 0
#define O_WRONLY 1
#define O_RDRW 2
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#define error_exit_code 0x55
#define PREMI 0777

int main(int argc, char* argv[])
{
  int deg;
  int to = STDOUT;
  int from = STDIN;
  int filefrom;
  int fileto;
  char *source;
  int i;
  for (i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-D") == 0)
      deg = 1;
    if (strncmp(argv[i], "-o", 2)==0)
    {
      to = 1;
      char *nameto = argv[i] + 2;
      fileto=system_call(SYS_OPEN,nameto,64|1|512, PREMI);
    }
    if (strncmp(argv[i], "-i", 2)==0)
    {
      from = 5;
      char *namefrom = argv[i] + 2;
      filefrom=system_call(SYS_OPEN, namefrom, RDO, 100);
    }
  }

  char *rtn;
  char buf[1];
  if (from == STDIN)
  {
    if (deg == 1)
    { /*system_call(SYS_WRITE,STDOUT,"please enter \n", 14)*/
      rtn = itoa(system_call(SYS_WRITE, STDOUT, "please enter \n", 14));
      system_call(SYS_WRITE, STDERR, itoa(SYS_WRITE), 1);
      system_call(SYS_WRITE, STDERR, "-", 1);
      system_call(SYS_WRITE, STDERR, rtn, 1);
      system_call(SYS_WRITE, STDERR, "\n", 1);
    }
    else
      system_call(SYS_WRITE, STDOUT, "please enter \n", 14);

    if (deg == 1)
    { /*system_call(SYS_READ,STDIN,buf,1)*/
      rtn = itoa(system_call(SYS_READ, from, buf, 1));
      system_call(SYS_WRITE, STDERR, itoa(SYS_READ), 1);
      system_call(SYS_WRITE, STDERR, "-", 1);
      system_call(SYS_WRITE, STDERR, rtn, 1);
      system_call(SYS_WRITE, STDERR, "\n", 1);
    }
    else
      system_call(SYS_READ, from, buf, 1);

    while (buf[0] != '\n')
    {
      if ((buf[0] <= 122) & (buf[0] >= 97))
        buf[0] = buf[0] - ('a' - 'A');

      if (deg == 1)
      { /*system_call(SYS_WRITE,STDOUT,buf,1)*/
        rtn = itoa(system_call(SYS_WRITE, fileto, buf, 1));
        system_call(SYS_WRITE, STDERR, itoa(SYS_WRITE), 1);
        system_call(SYS_WRITE, STDERR, "-", 1);
        system_call(SYS_WRITE, STDERR, rtn, 1);
        system_call(SYS_WRITE, STDERR, "\n", 1);
      }
      else
        system_call(SYS_WRITE, fileto, buf, 1);

      if (deg == 1)
      { /*system_call(SYS_READ,STDIN,buf,1)*/
        rtn = itoa(system_call(SYS_READ, from, buf, 1));
        system_call(SYS_WRITE, STDERR, itoa(SYS_READ), 1);
        system_call(SYS_WRITE, STDERR, "-", 1);
        system_call(SYS_WRITE, STDERR, rtn, 1);
        system_call(SYS_WRITE, STDERR, "\n", 1);
      }
      else
        system_call(SYS_READ, from, buf, 1);
    }
  }
  else /*if from file*/
  {
    
    while (system_call(SYS_READ, filefrom, buf, 1)!= 0)
    {
      if (deg == 1)
      { /*system_call(SYS_READ,STDIN,buf,1)*/
        system_call(SYS_WRITE, STDERR, itoa(SYS_READ), 1);
        system_call(SYS_WRITE, STDERR, "-", 1);
        system_call(SYS_WRITE, STDERR, rtn, 1);
        system_call(SYS_WRITE, STDERR, "\n", 1);
      }
      if ((buf[0] <= 122) & (buf[0] >= 97))
        buf[0] = buf[0] - ('a' - 'A');

      if (deg == 1)
      { /*system_call(SYS_WRITE,STDOUT,buf,1)*/
        rtn = itoa(system_call(SYS_WRITE, fileto, buf, 1));
        system_call(SYS_WRITE, STDERR, itoa(SYS_WRITE), 1);
        system_call(SYS_WRITE, STDERR, "-", 1);
        system_call(SYS_WRITE, STDERR, rtn, 1);
        system_call(SYS_WRITE, STDERR, "\n", 1);
      }
      else
        system_call(SYS_WRITE, fileto, buf, 1);
    }
    system_call(SYS_CLOSE,filefrom);
  }

  return 0;
}
