#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#define SIZE  512

void arch(char *dir, char *afile)
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
      arch(path, afile);
    }
    else 
    {
      snprintf(path, sizeof path, "%s/%s", dir, in->d_name);

      if((file = fopen(path, "r")) == NULL)
      {
        printf("Не удалось открыть файл.\n");
        return;
      }
 
      if((archive = fopen(afile, "a+")) == NULL || afile[0] != '/')
      {
        printf("Не удалось открыть архив.\n");
        fclose(archive);
        return;
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
  char afile[50] = {'a'};
  char directory[50] = {'a'};
  int number;

  while(1) 
  {
    printf("1 - Архивация\n2 - Разархивация\n3 - Выход\nВведите число: ");
    scanf("%d", &number);
    while(getchar()!='\n')
      continue;

    switch(number) 
    {
      case 1: 
        printf ("Архивация\n");
        printf ("Введите путь к архиву:\n");
        scanf("%s", afile);

        printf ("Введите путь к директории для архивации:\n");
        scanf("%s", directory);

      arch(directory, afile);
      return 0;

    case 2:
      printf ("Разархивация\n");
      return 0;

    case 3:
      return 0;

    default:
      printf ("Введено неверное число!\n");
      break;
    }
  }
}