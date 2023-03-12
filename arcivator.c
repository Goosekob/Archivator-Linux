#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>


#define SIZE  512

void printdir(char *dir, int depth, char *afile)
    {
      char path[SIZE];
      char buffer[SIZE];
      FILE *file, *archive;
      size_t symbols;
      DIR *mydir;
      struct dirent *in;
      struct stat statbuf;

      if ((mydir = opendir(dir)) == NULL)
      {
        printf("Невозможно открыть директорию.\n");
        return;
      }
      chdir(dir);

      while((in = readdir(mydir)) != NULL )
      {
        lstat (in->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode))
        {
          snprintf(path, sizeof path, "%s/%s", dir, in->d_name);
          if (strcmp(".", in->d_name) == 0 || strcmp("..", in->d_name) == 0) continue;
          printf("%*s%s\n", depth, "", in->d_name);
          printdir(path, depth+4, afile);
        }
        else 
        {
          printf("%*s%s\n", depth, " ", in->d_name);
          snprintf(path, sizeof path, "%s/%s", dir, in->d_name);

          if((file = fopen(path, "r")) == NULL)
          {
          printf("Не удалось открыть файл.\n");
          return;
          }
 
          if((archive = fopen(afile, "a+")) == NULL)
          {
          printf("Не удалось открыть архив.\n");
          return;
          fclose(archive);
          }

          file = fopen(path, "r");
          archive = fopen(afile, "a+"); 

          while((symbols = fread(buffer, sizeof(char), SIZE, file)) > 0)
          {
            fwrite(buffer, sizeof(char), symbols, archive);
          }
          fclose(archive);
          fclose(file);
        }

      }
      chdir("..");
      closedir(mydir);

    }

int main() 
{
    char afile[50];
    char directory[50];
    char path[SIZE];
    char buffer[SIZE];
    FILE *file, *archive;
    size_t symbols;
    /*DIR *mydir;
    struct dirent *in;
    struct statbuf;
    */
    printf ("Введите путь к архиву: ");
    scanf("%s", afile);

    printf ("Введите путь к директории для архивации: \n");
    scanf("%s", directory);

    printdir(directory, 0, afile);

  /*
    mydir = opendir(directory);
      if (!mydir) 
      {
        printf("Не удалось открыть директорию.\n");
        return 3;
      };

    while ( (in = readdir(mydir)) != NULL)
  {
    snprintf(path, sizeof path, "%s/%s", directory, in->d_name);

    if((file = fopen(path, "r")) == NULL)
    {
        printf("Не удалось открыть файл.\n");
        return 1;
    }
 
    if((archive = fopen(afile, "a+")) == NULL)
    {
        printf("Не удалось открыть архив.\n");
        return 2;
        fclose(archive);
    }

    file = fopen(path, "r");
    archive = fopen(afile, "a+"); 

    while((symbols = fread(buffer, sizeof(char), SIZE, file)) > 0)
    {
      fwrite(buffer, sizeof(char), symbols, archive);
    }

    fclose(archive);
    fclose(file);
  }
    closedir(mydir);
    */
}