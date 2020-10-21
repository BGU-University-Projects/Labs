/*#include "units.c"*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <elf.h>

#define SYS_OPEN 5
#define RDO 0

void substring(char s[], char sub[], int p, int l)
{
  int c = 0;

  while (c < l)
  {
    sub[c] = s[p + c - 1];
    c++;
  }
  sub[c] = '\0';
}

void print_units(FILE *output, char *buffer, int count, int unit_size, char show)
{
  int unitS = unit_size;
  int decSHOW;
  char *strdec = calloc(1, 1000);
  char *end = buffer + unitS * count;
  static char *formats1[] = {"%#hhd\n", "%#hd\n", "No such unit", "%#d\n"};
  static char *formats2[] = {"%#hhx\n", "%#hx\n", "No such unit", "%#x\n"};

  char *format;
  if (show == '0')
    format = formats1[unit_size - 1];
  else
    format = formats2[unit_size - 1];

  while (buffer < end)
  {
    int var = *((int *)(buffer));
    fprintf(output, format, var);

    buffer += unitS;
  }
  free(strdec);
}

struct fun_desc
{
  char *name;
  void (*fun)(state);
};

typedef struct
{
  char debug_mode;
  char file_name[128];
  int unit_size;
  unsigned char mem_buf[10000];
  size_t mem_count;
  char display_mode;
  /*
   .
   .
   Any additional fields you deem necessary
  */
} state;

void ToggleDebugMode(state *s)
{
  if (s->debug_mode == '0') /*if its off*/
  {
    s->debug_mode = '1';
    printf("%s\n", "Debug flag now on");
  }
  else
  {
    s->debug_mode = '0';
    printf("%s\n", "Debug flag now off");
  }
}

void ToggleDisplayMode(state *s)
{
  if (s->display_mode == '0') /*if its off*/
  {
    s->display_mode = '1';
    printf("%s\n", "Display flag now on, hexadecimal representation");
  }
  else
  {
    s->display_mode = '0';
    printf("%s\n", "Display flag now off, decimal representation");
  }
}

void setFileName(state *s)
{
  char newName[100];
  printf("%s\n", "pls enter an new File Name and smile");
  /*fgets(newName, 2048, stdin);*/
  scanf("%s", newName);

  strcpy(s->file_name, newName);

  if (s->debug_mode == '1')
    printf("Debug: file name set to: %s\n", s->file_name);
}

void setUnitSize(state *s)
{
  printf("%s\n", "pls enter an new Unit Size and smile");
  int newS;
  char strnewS[100];
  scanf("%s", strnewS);
  newS = atoi(strnewS);
  /*newS = newS - '0';*/
  if ((newS == 1) | (newS == 2) | (newS == 4))
  {
    s->unit_size = newS;
    if (s->debug_mode == '1')
      printf("Debug: set Size to: %d\n", s->unit_size);
  }
  else
    printf("%s\n", "not vaild size");
}

void quit(state *s)
{
  exit(0);
}

void Load(state *s)
{
  int location;
  char *strlocation;
  int length;
  char *strlength;

  if (s->file_name == NULL)
  {
    printf("name is null :(");
    return;
  }

  FILE *pFile;
  pFile = fopen(s->file_name, "rb");
  if (pFile == NULL)
  {
    printf("%s\n", "cannot open file :(");
    /*fclose(pFile);*/
    return;
  }
  char newpara[100];
  printf("%s\n", "Please enter <location> <length>");

  fgets(newpara, 60, stdin);
  /*seperate the args*/
  strcpy(strlocation, newpara);
  strlength = strchr(strlocation, ' ') + 1;
  /*strcpy(strlength, strchr(strlocation, ' ') + 1);*/
  length = atoi(strlength);
  newpara[strlen(strlocation) - strlen(strlength) - 1] = '\n';
  strcpy(strlocation, newpara);
  location = (int)strtol(strlocation, NULL, 16);
  if (s->debug_mode == '1')
  {
    printf("file_name: %s\n", s->file_name);
    printf("Location: %d\n", location);
    printf("Length: %d\n", length);
  }

  fseek(pFile, location, SEEK_SET);

  fread(s->mem_buf, s->unit_size, length, pFile);

  printf("Loaded %d", length);
  printf("%s\n", " units into memory");

  fclose(pFile);
}

void Memory_Display(state *s)
{
  int u;
  char *stru = calloc(1, 100);
  char *strr = calloc(1, 100);
  int addr;
  char *straddr = calloc(1, 100);
  char newpara[100];
  printf("%s\n", "Please enter <u> <addr>");
  /*fgets(newpara, 60, stdin);*/
  fgets(newpara, 60, stdin);
  /*seperate the args*/
  strcpy(stru, newpara);
  straddr = strchr(newpara, ' ') + 1;
  newpara[strlen(stru) - strlen(straddr) - 1] = '\n';
  strcpy(stru, newpara);
  addr = (int)strtol(straddr, NULL, 16);
  u = atoi(stru);

  if (addr == 0)
  {
    print_units(stdout, s->mem_buf, u, s->unit_size, s->display_mode);
  }
  else
  {
    char *ptr;
    sscanf(straddr, "%X", &ptr);
    print_units(stdout, ptr, u, s->unit_size, s->display_mode);
  }

  /*free(straddr);
  free(stru);
  free(strr);*/
}

int getIndex(char input[], int num)
{ /*return the byte[num] from the input*/

  char j[2];
  j[0] = input[(2 * num) - 2];
  j[1] = input[(2 * num) - 1];
  int first = (int)strtol(j, NULL, 16);
  return first;
}

void Memory_Modify(state *s)
{
  char *strr = calloc(1, 100);
  int location;
  char test[8];
  char *strlocation = calloc(1, 100);
  int val;
  int err;
  char *strval = calloc(1, 100);
  char newpara[100];
  printf("%s\n", "Please enter <location> <val>");

  fgets(newpara, 60, stdin);
  /*seperate the args*/
  strcpy(strlocation, newpara);
  strval = strchr(strlocation, ' ') + 1;
  /*strcpy(strlength, strchr(strlocation, ' ') + 1);*/
  val = (int)strtol(strval, NULL, 16);
  newpara[strlen(strlocation) - strlen(strval) - 1] = '\n';
  strcpy(strlocation, newpara);
  location = (int)strtol(strlocation, NULL, 16);
  if (s->debug_mode == '1')
    printf("%s\n", strlocation);
  /*check input somehow*/
  /*s->unit_size=4;*/
  int i;
  switch (s->unit_size)
  {
  case 1:
    test[6] = strval[0];
    test[7] = strval[1];
    break;
  case 2:
    test[4] = strval[0];
    test[5] = strval[1];
    test[6] = strval[2];
    test[7] = strval[3];
    break;
  case 4:
    strcpy(test, strval);
    if (test[7] == 10)
    {
      for (i = strlen(test) - 1; i > 0; i--)
        test[i] = test[i - 1];
      test[0] = 48;
    }
    break;
  }
  if (location == 0)
  {
    switch (s->unit_size)
    {
    case 1:
      s->mem_buf[location] = getIndex(test, 1);
      break;
    case 2:
      s->mem_buf[location] = getIndex(test, 2);
      s->mem_buf[location + 1] = getIndex(test, 1);
      break;
    case 4:
      s->mem_buf[location] = getIndex(test, 4);
      s->mem_buf[location + 1] = getIndex(test, 3);
      s->mem_buf[location + 2] = getIndex(test, 2);
      s->mem_buf[location + 3] = getIndex(test, 1);
      break;
    }
  }
  else
  {
    char *ptr;
    int i = 0;
    ptr = (char *)location;
    /*sscanf(strlocation,"%X",&ptr);*/
    /*write(strval, s->unit_size, 1, ptr);*/
    for (i = 0; i < s->unit_size; i++)
      ptr[i] = getIndex(test, 4 - i);

    printf("%s\n", "------------------------");
  }
}

void Save_Into_File(state *s)
{
  int source;
  int target;
  int length;
  int sz;
  int err = 0;
  char *strsource = calloc(1, 100);
  char *strtarget = calloc(1, 100);
  char *strlength = calloc(1, 100);
  char newpara[100];
  printf("%s\n", "Please enter <source-address> <target-location> <length>");
  /*fgets(newpara, 60, stdin);*/
  fgets(newpara, 60, stdin);
  /*seperate the args*/
  strcpy(strsource, newpara);
  strtarget = strchr(newpara, ' ') + 1;
  strlength = strchr(strtarget, ' ') + 1;
  length = atoi(strlength);

  newpara[strlen(strsource) - strlen(strlength) - 1] = '\n';
  strcpy(strsource, newpara);
  strtarget = strchr(strsource, ' ') + 1; /*targer is at hexa*/
  target = (int)strtol(strtarget, NULL, 16);

  newpara[strlen(strsource) - strlen(strtarget) - 1] = '\n';
  strcpy(strsource, newpara); /*source is at hexa*/
  /*addr = (int)strtol(straddr, NULL, 16);*/
  source = (int)strtol(strsource, NULL, 16);

  char newSig[(length) * (s->unit_size)];
  int i;
  for (i = 0; i < (length) * (s->unit_size); i++)
    newSig[i] = 0;
  /*char* buffer=calloc(length,s->unit_size);*/
  char *buffer;
  FILE *pFile;
  pFile = fopen(s->file_name, "rb+");
  if (pFile == NULL)
  {
    printf("%s\n", "cannot open file :(");
    return;
  }
  if (s->debug_mode == '1')
    printf("%s\n", "Debug: file was opened !");

  fseek(pFile, 0L, SEEK_END);
  sz = ftell(pFile); /*get full size of file*/
  if (target > sz)
    printf("%s\n", "ERROR: target-location> is greater than the size of the file");

  else
  {
    fseek(pFile, target, SEEK_SET); /*set offset of file*/
    if (source == 0)
    {
      err = fwrite(s->mem_buf, s->unit_size, length, pFile);
    }
    else
    {
      fread(buffer, s->unit_size, length, strsource);
      /*buffer="NADAV";
    putchar(*buffer);*/
      err = fwrite(buffer, s->unit_size, length, pFile);
    }
  }
  fclose(pFile);
}

void showEntry(state *s)
{
  FILE *file = fopen(s->file_name, "rb");
  Elf32_Ehdr *myELF = calloc(1, sizeof(Elf32_Ehdr));
  fread(myELF, sizeof(Elf32_Ehdr), 1, file);
  printf("THE ENTRY POINT IS: %x\n", (int)myELF->e_entry);
  fclose(file);
  free(myELF);
}

void showCompiled(state *s)
{
  FILE *file = fopen(s->file_name, "rb");
  Elf32_Ehdr *myELF = calloc(1, sizeof(Elf32_Ehdr));
  fread(myELF, sizeof(Elf32_Ehdr), 1, file);
  printf("THE ENTRY POINT IS: %x\n", (int)myELF->e_entry);
  fclose(file);
  free(myELF);
}

int main(int argc, char **argv)
{
  char carray[5];
  char *toDel;
  state *curstate = calloc(1, sizeof(state));
  char *ttt = calloc(1, sizeof(int));

  curstate->debug_mode = '1';
  curstate->display_mode = '1';
  curstate->unit_size = 1;

  strcpy(carray, "");
  struct fun_desc menu[] = {{"Toggle Debug Mode", ToggleDebugMode},
   {"Set File Name", setFileName}, {"Set Unit Size", setUnitSize}, 
   {"Load Into Memory", Load},
    {"Toggle Display Mode", ToggleDisplayMode}, 
    {"Memory Display", Memory_Display}, 
    {"Save Into File", Save_Into_File},
     {"Memory Modify", Memory_Modify}, 
     {"SHOW ENTRY", showEntry}, 
     {"Quit", quit}, {NULL, NULL}};
  int i;
  int sizeMenu = sizeof(menu) / sizeof(double);
  int input = 0;

  while (1)
  {

    if (curstate->debug_mode == '1')
    {
      printf("file_name: %s\n", curstate->file_name);
      printf("unit_size: %d\n", curstate->unit_size);
      printf("mem_count: %d\n", curstate->mem_count);
    }

    printf("%s\n", "Please choose a function:");
    for (i = 0; ((i < sizeMenu) & (menu[i].name != NULL)); i++)
    {
      printf("%d", i);
      printf("%s", ") ");
      printf("%s\n", menu[i].name);
    }

    /*while ((input = fgetc(stdin)) != '\n')*/
    /*scanf("%c", input);*/
    input = fgetc(stdin);
    fgetc(stdin);
    {
      input = input - '0';

      if ((input < 0) & (input > (sizeMenu - 1)))
      {
        printf("%s\n", "Not within bounds");
        exit(0);
      }
      /*to change when can*/
      switch ((input))
      {
      case 0:
        ToggleDebugMode(curstate);
        break;
      case 1:
        setFileName(curstate);
        fgetc(stdin);
        break;
      case 2:
        setUnitSize(curstate);
        fgetc(stdin);
        break;
      case 3:
        Load(curstate);
        break;
      case 4:
        ToggleDisplayMode(curstate);
        break;
      case 5:
        Memory_Display(curstate);
        break;
      case 6:
        Save_Into_File(curstate);
        break;
      case 7:
        Memory_Modify(curstate);
        break;
      case 8:
        showEntry(curstate);
        break;
      case 9:
        quit(curstate);
        break;
      }
    }
  }
}