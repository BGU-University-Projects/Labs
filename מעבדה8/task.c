#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <elf.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SYS_OPEN 5
#define RDO 0

int debug;
void *map_start;
Elf32_Ehdr *myELF;
int currentfd;

struct fun_desc
{
  char *name;
  void (*fun)();
};

void Examine()
{
  int test;
  char newpara[100];
  printf("%s\n", "pls enter an new ELF File Name and smile");
  /*fgets(newpara, 60, stdin);  */
  scanf("%s", newpara);
  test=fgetc(stdin);


  /*close all opened filesss*/
  /*strcpy(newpara, "ntsc");*/
  int fd;
  void *map_start2;    /*will point to the start of the memory mapped file */
  struct stat fd_stat; /* this is needed to  the size of the file */
  int num_of_section_headers;

  if (currentfd > 0)
  {
    close(currentfd);
  }

  if ((fd = open(newpara, O_RDWR)) < 0)
  {
    perror("error in open");
    exit(-1);
  }

  if (fstat(fd, &fd_stat) != 0)
  {
    perror("stat failed");
    exit(-1);
  }

  if ((map_start2 = mmap(0, fd_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
  {
    perror("mmap failed");
    exit(-4);
  }

  /* now, the file is mapped starting at map_start.
    * all we need to do is tell *header to point at the same address:
    */

  Elf32_Ehdr *myELF2 = (Elf32_Ehdr *)map_start2;
  myELF = myELF2;
  /* now we can do whatever we want with header!!!!
    * for example, the number of section header can be obtained like this:*/

  printf("THE ENTRY POINT IS: %x\n", (int)myELF->e_entry);

  if ((myELF->e_ident[1] != 'E') || (myELF->e_ident[2] != 'L') || (myELF->e_ident[3] != 'F'))
  {
    perror("Wrong Magic");
    exit(-1);
  }
  /**/
  Elf32_Shdr *MainSection = (Elf32_Shdr *)(map_start + (myELF->e_shoff));
  Elf32_Shdr *program = (Elf32_Shdr *)(map_start + (myELF->e_phoff));

  printf("THE Magic Number IS: %s\n", "69,76,70");
  printf("The data encoding sheme: %d\n", myELF->e_machine);
  printf("The file offset in which the section header table resides: %d\n", myELF->e_shoff);
  printf("The number of section header entries: %d\n", myELF->e_shnum);
  printf("The size of each section header entry:%d\n",myELF->e_ehsize);
  printf("The file offset in which the program header table resides: %d\n", myELF->e_phoff);
  printf("The number of program header entries.: %d\n", myELF->e_phnum);
  printf("The size of each program header entry:\n");
  printf("The size of each program header entry:%d\n",myELF->e_phentsize);
  /* now, we unmap the file */
  /*munmap(map_start, fd_stat.st_size);*/
  map_start = map_start2;
}

void PrintThoseNames()
{
  Elf32_Shdr *MainSection = (Elf32_Shdr *)(map_start + (myELF->e_shoff));
  Elf32_Shdr *StringTable = &MainSection[myELF->e_shstrndx];
  char *SectionsNames = map_start + StringTable->sh_offset;
  if (debug == 1)
  {
    printf("shstrndx: %d\n", myELF->e_shstrndx);

  }

  printf("%s\n", "Index\tName\t\tAddress\tOffset\tSize\tType");
  int i;
  for (i = 0; i < myELF->e_shnum; i++)
  {
    printf("%2d\t", i);
    if(debug==1)
      /*printf("%d\n", MainSection[i].sh_name);*/
    printf("%-15s", SectionsNames + MainSection[i].sh_name);
    printf("%8X\t", MainSection[i].sh_addr);
    printf("%X\t", MainSection[i].sh_offset);
    printf("%X\t", MainSection[i].sh_size);
    printf("%d\n", MainSection[i].sh_type);
  }
}

void PrintThoseSymbols()
{
  Elf32_Shdr *MainSection = (Elf32_Shdr *)(map_start + (myELF->e_shoff));
  Elf32_Shdr *StringTable = &MainSection[myELF->e_shstrndx];
  char *SectionsHeadersNames = map_start + StringTable->sh_offset;
  int i = 0;
  while (strcmp(SectionsHeadersNames + MainSection[i].sh_name, ".strtab") != 0)
    i++;
  char *myStringTable = map_start + MainSection[i].sh_offset;
  i = 0;
  while (strcmp(SectionsHeadersNames + MainSection[i].sh_name, ".symtab") != 0)
    i++;
  Elf32_Sym *SymbolTable = (Elf32_Sym *)(map_start + (MainSection[i].sh_offset));
  Elf32_Sym CurSymbolTable ;
  int sizeOf = MainSection[i].sh_size;
  int sizeOf2 = sizeof(Elf32_Sym);
  int totalSize = sizeOf / sizeOf2;
  if (debug == 1)
  {
    printf("number of sybmols therein: %d\n", totalSize);
    printf("Size of SYMB Table: %d\n", sizeOf);
  }
  printf("%s\n", "Index\tValue\t\tSection_Index\tSectionName\tName");
  for (i = 25; i < totalSize; i++)
  {
    CurSymbolTable=SymbolTable[i];
    printf("%2d\t", i);
    printf("%X\t\t", CurSymbolTable.st_value);
    printf("%d\t\t", CurSymbolTable.st_shndx);
    if((int)CurSymbolTable.st_shndx==65521)
        printf("%s\t","SHN_ABS");
    else
    printf("%s\t", SectionsHeadersNames + (MainSection[(int)CurSymbolTable.st_shndx].sh_name));
  
    printf("%-15s\n", myStringTable + CurSymbolTable.st_name);
  }
}

void Relocation_Tables()
{
  Elf32_Shdr *CurTable;
  Elf32_Rel *CurRel;
  int i=0;
  int totalsize;
  int sizeunit = sizeof(Elf32_Rel);
  int totalunits;
  Elf32_Shdr *MainSection = (Elf32_Shdr *)(map_start + (myELF->e_shoff));
  Elf32_Shdr *StringTable = &MainSection[myELF->e_shstrndx];
  char *SectionsHeadersNames = map_start + StringTable->sh_offset;
  while (strcmp(SectionsHeadersNames + MainSection[i].sh_name, ".dynsym") != 0)
    i++;
  /*char *dynsymTable = map_start + MainSection[i].sh_offset;*/
  Elf32_Sym *thisSym=(Elf32_Sym *)(map_start + MainSection[i].sh_offset);
  i=0;
  while (strcmp(SectionsHeadersNames + MainSection[i].sh_name, ".dynstr") != 0)
    i++;
  char *dynsymTable = map_start + MainSection[i].sh_offset;
  i = 0;
  while (strcmp(SectionsHeadersNames + MainSection[i].sh_name, ".strtab") != 0)
    i++;
  char *myStringTable = map_start + MainSection[i].sh_offset;
  i=0;
  int j;
  int test=sizeof(Elf32_Sym);
  printf("Offset\tInfo\tType\tValue\tName\n");
  for (i = 0; i < myELF->e_shnum; i++)
  {
    if (MainSection[i].sh_type == 9)
    { /*so its type REL*/
      totalsize = MainSection[i].sh_size;
      totalunits = totalsize / sizeunit;
      for (j = 0; j < totalunits; j++)
      {/*for each unit the the rel*/
        Elf32_Rel *CurRel = (Elf32_Rel *)((j*sizeunit)+map_start + (MainSection[i].sh_offset));
        printf("%x\t",CurRel->r_offset);
        printf("%x\t",CurRel->r_info);
        printf("%d\t",ELF32_R_TYPE(CurRel->r_info));
        int indexindyn=ELF32_R_SYM(CurRel->r_info);
        Elf32_Sym thisSym2=thisSym[indexindyn];
        printf("%x\t",thisSym2.st_value);
        printf("%s\n", dynsymTable + thisSym2.st_name);

      }
      
    }

    /*while (strcmp(SectionsHeadersNames + MainSection[i].sh_type, ".strtab") != 0)
    i++;
  char *myStringTable = map_start + MainSection[i].sh_offset;
  i = 0;*/
  }
}

int DebugMode()
{
  if (debug == 0)
  {
    printf("%s\n", "Debug Is Now On!!!");
    return 1;
  }
  else
  {
    printf("%s\n", "Debug Is Now Off!!!");
    return 0;
  }
}

void quit()
{
  /*unmap allllllll*/
  exit(0);
}

int main(int argc, char **argv)
{
  int currentfd = 0;
  char carray[5];
  strcpy(carray, "");
  struct fun_desc menu[] = {{"Toggle Debug Mode", DebugMode},
                            {"Examine ELF File", Examine},
                            {"Print Section Names", PrintThoseNames},
                            {"Print Section Symbols", PrintThoseSymbols},
                            {"Relocation Tables", Relocation_Tables},
                            {"Quit", quit},
                            {NULL, NULL}};
  int i;
  int sizeMenu = sizeof(menu) / sizeof(double);
  int input = 0;
  debug = 1;
  /*int debug=1;*/
  while (1)
  {

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
        debug = DebugMode();
        break;
      case 1:
        Examine();
        break;
      case 2:
        PrintThoseNames();
        break;
      case 3:
        PrintThoseSymbols();
        break;
      case 4:
        Relocation_Tables();
        break;
      case 5:
        quit();
        break;
      }
    }
  }
}