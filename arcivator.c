#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>


#define SIZE  512

int main() 
{
    char afile[50];
    char directory[50];
    char buffer[SIZE];
    FILE *file, *archive;
    size_t symbols;
    DIR *mydir;
    struct dirent *in;

    printf ("Введите путь к архиву: ");
    scanf("%s", afile);

    printf ("Введите путь к директории для архивации: \n");
    scanf("%s", directory);

    mydir = opendir(directory);
      if (!mydir) 
      {
        printf("Не удалось открыть директорию.\n");
        return 3;
      };

    while ( (in = readdir(mydir)) != NULL)
  {
    char path[SIZE];
    snprintf(path, sizeof path, "%s/%s", directory, in->d_name);

    if (strcmp(".", in->d_name) == 0 || strcmp("..", in->d_name) == 0) continue;

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
}