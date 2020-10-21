#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus
{
  unsigned short SigSize;
  char virusName[16];
  char *sig;
} virus;

typedef struct link link;
struct link
{
  link *nextVirus;
  virus *vir;
};

struct fun_desc
{
  char *name;
  void (*fun)();
};

virus *readVirus(FILE *inputfile)
{
  if (inputfile == NULL)
  {
    fputs("File error", stderr);
    exit(1);
  }

  int i;
  char *buffer = (char *)malloc(sizeof(char) * sizeof(virus));
  fread(buffer, sizeof(char), (unsigned long)2, inputfile);
  virus *newvirus = calloc(1, sizeof(virus));
  newvirus->SigSize = buffer[0] + 256 * buffer[1];
  newvirus->sig = calloc((newvirus->SigSize), sizeof(char));

  fread(newvirus->virusName, sizeof(char), (unsigned long)16, inputfile);
  fread(newvirus->sig, sizeof(char), newvirus->SigSize, inputfile);

  free(buffer);
  return newvirus;
}
void printVirus(virus *virus, FILE *output)
{
  fprintf(output, "Virus name: %s\n", virus->virusName);

  fprintf(output, "Virus size: %d\n", virus->SigSize);

  int i;
  fprintf(output, "%s\n", "signature:");

  for (i = 0; i < virus->SigSize; i++)
    fprintf(output, "%02X ", (unsigned char)virus->sig[i]);
  fprintf(output, "\n");
  fprintf(output, "\n");
}

void printVirusToScreen(virus *virus)
{
  fprintf(stdout, "Virus name: %s\n", virus->virusName);

  fprintf(stdout, "Virus size: %d\n", virus->SigSize);

  int i;
  fprintf(stdout, "%s\n", "signature:");

  for (i = 0; i < virus->SigSize; i++)
    fprintf(stdout, "%02X ", (unsigned char)virus->sig[i]);
  fprintf(stdout, "\n");
  fprintf(stdout, "\n");
}

void list_print(link *virus_list, FILE *file)
{
  /* Print the data of every link in list to the given stream. Each item followed by a newline character. */
  if (virus_list != NULL)
  {
    printVirus(virus_list->vir, file);
    list_print(virus_list->nextVirus, file);
  }
}

link *list_append(link *virus_list, virus *data)
{
  if (virus_list == NULL)
  {
    link *newFirst = calloc(1, sizeof(link));
    newFirst->nextVirus = NULL;
    newFirst->vir = data;
    return newFirst;
  }
  else
  {
     /*T0 ADD LINK IN FIRST PLACE*/
        link *newFirst=calloc(1, sizeof(link));
        newFirst->nextVirus=virus_list;
        newFirst->vir=data;
        return newFirst;

    /* T0 ADD LINK IN LAST PLACE
    link *last = virus_list;
    while (last->nextVirus != NULL)
      last = last->nextVirus;
    last->nextVirus = calloc(1, sizeof(link));
    last->nextVirus->nextVirus = NULL;
    last->nextVirus->vir = data;
    return virus_list;*/
  }
}

void list_free(link *virus_list)
{ /* Free the memory allocated by the list. */
  if (virus_list != NULL)
  {
    list_free(virus_list->nextVirus);
    free(virus_list->vir->sig);
    free(virus_list->vir);
  }
  free(virus_list);
}

virus *load(char *filename)
{

  long lSize;
  FILE *inputfile;
  inputfile = fopen(filename, "rb");
  fseek(inputfile, 0, SEEK_END);
  lSize = ftell(inputfile);
  rewind(inputfile);

  link *virusLinks = calloc(1, sizeof(link));
  virusLinks->vir = readVirus(inputfile);
  /*link *cur = virusLinks;*/
  lSize = lSize - (18 + (virusLinks->vir->SigSize));
  while (lSize > 0)
  {
    virusLinks = list_append(virusLinks, readVirus(inputfile));
    /*cur = cur->nextVirus;*/
    lSize = lSize - (18 + (virusLinks->vir->SigSize));
  }
  fclose(inputfile);
  return virusLinks;
}

void printListToScreen(link *virus_list)
{
  /* Print the data of every link in list to the given stream. Each item followed by a newline character. */
  if (virus_list != NULL)
  {
    printVirusToScreen(virus_list->vir);
    printListToScreen(virus_list->nextVirus);
  }
}

void quit()
{
  exit(0);
}

void detect_virus(char *buffer, unsigned int size, link *virus_list)
{
  char *toTest;
  int sigSize;
  int resultCmp;
  int i;
  while (virus_list != NULL)
  {
    toTest = virus_list->vir->sig;
    for (i = 0; i + virus_list->vir->SigSize < size; i++)
    {
      sigSize = virus_list->vir->SigSize;
      resultCmp = memcmp(buffer + i, toTest, sigSize);
      if (resultCmp == 0)
      {
        printf("The starting byte location is: ");
        printf("%d\n", i);
        printf("The virus name is: ");
        printf("%s\n", virus_list->vir->virusName);
        printf("The size of the virus signature is: ");
        printf("%d\n\n", virus_list->vir->SigSize);
      }
    }
    virus_list = virus_list->nextVirus;
  }
}

void kill_virus(char *fileName, int signitureOffset, int signitureSize)
{
  FILE * infile;
  infile = fopen(fileName, "rb+");

  char newSig[signitureSize];
  int i;
  for (i = 0; i < signitureSize; i++)
    newSig[i] = 0x90;

  fseek(infile , signitureOffset, SEEK_SET);
  fwrite(newSig, sizeof(char), sizeof(newSig), infile);
  fclose(infile);
}

int main(int argc, char **argv)
{
  int byteloc;
  int sizeloc;
  long lSize;
  FILE *inputfile;
  char *toDetect;
  char *buf = calloc(1, 10000);
  char filename[1024];
  link *newLinkedViruses = NULL;
  struct fun_desc menu[] = {{"Load signatures", load}, {"Print signatures",
   printListToScreen}, {"Detect viruses", detect_virus}, {"Fix file", kill_virus}, {"Quit", quit}, {NULL, NULL}};
  int i;

  int size = sizeof(menu) / sizeof(double);
  int input = 0;
  
  while (1)
  {
    for (i = 0; ((i < size) & (menu[i].name != NULL)); i++)
  {
    printf("%d", i + 1);
    printf("%s", ") ");
    printf("%s\n", menu[i].name);
  }
    while ((input = fgetc(stdin)) != '\n')
    {

      input = input - '0';
      /*printf("%d\n",input);*/

      if (!((input >= 0) & (input <= (size - 1))))
      {
        printf("%s\n", "Option Not within bounds");
        exit(0);
      }
      switch (input)
      {

      case 1:
        fgets(filename, 1024, stdin);
        fgets(filename, 1024, stdin);
        sscanf(filename, "%s", filename);
        newLinkedViruses = load(filename);
        break;

      case 2:
        printListToScreen(newLinkedViruses);
        break;

      case 3:

        inputfile = fopen(argv[1], "rb");
        fseek(inputfile, 0, SEEK_END);
        lSize = ftell(inputfile);
        rewind(inputfile);
        fread(buf, sizeof(char), lSize, inputfile);
        detect_virus(buf, lSize, newLinkedViruses);

        fclose(inputfile);
        break;

      case 4:
        printf("Enter the the starting byte location: ");
        scanf("%d", &byteloc);
        printf("Enter the size of the virus signature: ");
        scanf("%d", &sizeloc);
        toDetect=argv[1];

        kill_virus(toDetect, byteloc, sizeloc);
        printf("Fixed\n");

        break;

      case 5:
        free(buf);
        list_free(newLinkedViruses);
        quit();

        break;
      default:
        break;
      }
    }
  }
  return 0;
}