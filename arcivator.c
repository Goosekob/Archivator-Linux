#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

#define PATHSIZE  512
#define SIZE  1024

void arch(char *dir, char *afile)
{
  char path[PATHSIZE], doppath1[PATHSIZE], doppath2[PATHSIZE], doppath3[PATHSIZE], buffer[SIZE];
  FILE *file, *archive, *dopname, *dopsize, *dopnamesize;
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
      if (strcmp(".", in->d_name) == 0 || strcmp("..", in->d_name) == 0) 
        continue;
      arch(path, afile);
    }
    else 
    {
      snprintf(path, sizeof path, "%s/%s", dir, in->d_name);
      snprintf(doppath1, sizeof doppath1, "%sdopname", afile);
      snprintf(doppath2, sizeof doppath2, "%sdopsize", afile);
      snprintf(doppath3, sizeof doppath3, "%sdopnamesize", afile);

      if((file = fopen(path, "r")) == NULL)
      {
        printf("Не удалось открыть файл.\n");
        return;
      }

      if((archive = fopen(afile, "a")) == NULL || afile[0] != '/')
      {
        printf("Не удалось открыть архив.\n");
        return;
      }

      if((dopname = fopen(doppath1, "a")) == NULL )
      {
        printf("Не удалось открыть доп.архив.\n");
        return;
      }

      if((dopsize = fopen(doppath2, "a")) == NULL )
      {
        printf("Не удалось открыть доп.архив.\n");
        return;
      }

      if((dopnamesize = fopen(doppath3, "a")) == NULL )
      {
        printf("Не удалось открыть доп.архив.\n");
        return;
      }
      
      file = fopen(path, "r");
      archive = fopen(afile, "a"); 
      dopname = fopen(doppath1, "a");
      dopsize = fopen(doppath2, "a");
      dopnamesize = fopen(doppath3, "a");

      while((symbols = fread(buffer, sizeof(char), SIZE, file)) > 0)
      {
        fwrite(buffer, sizeof(char), symbols, archive);
        fprintf(dopname, "%s\n", in->d_name);
        fprintf(dopsize, "%ld\n", symbols);
        char *b = in->d_name;
        fprintf(dopnamesize, "%ld\n", strlen(b)+1);
      }
      fclose(dopsize);
      fclose(dopname);
      fclose(dopnamesize);
      fclose(archive);
      fclose(file);
    }
  }
  chdir("..");
  closedir(mydir);
}

void dearch(char *dir, char *afile)
{
  char path[PATHSIZE], doppath1[PATHSIZE], doppath2[PATHSIZE], doppath3[PATHSIZE];
  FILE *file, *archive, *names, *dopsize, *dopnamesize;
  DIR *mydir;

  snprintf(doppath1, sizeof doppath1, "%sdopname", afile);
  snprintf(doppath2, sizeof doppath2, "%sdopsize", afile);
  snprintf(doppath3, sizeof doppath3, "%sdopnamesize", afile);

  if((archive = fopen(afile, "r")) == NULL || afile[0] != '/')
  {
    printf("Не удалось открыть архив.\n");
    return;
  }

  if((names = fopen(doppath1, "r")) == NULL )
  {
    printf("Не удалось открыть доп.архив.\n");
    return;
  }

  if((dopnamesize = fopen(doppath3, "r")) == NULL )
  {
    printf("Не удалось открыть доп.архив.\n");
    return;
  }

  if((dopsize = fopen(doppath2, "r")) == NULL )
  {
    printf("Не удалось открыть доп.архив.\n");
    return;
  }

  if ((mydir = opendir(dir)) == NULL)
  {
    printf("Невозможно открыть директорию.\n");
    return;
  }
  chdir(dir);

  char *data[SIZE], buff[SIZE], c, *name[SIZE];
  size_t size[SIZE], namesize[SIZE], length = 0;
  int elements = 0;

  dopsize = fopen(doppath2, "r");
  for (int i = 0; i < SIZE; i++)
  {
    fscanf(dopsize, "%ld", &size[i]);
    if (size[i] > 0)
      elements++;
    else 
      break;
  }
  fclose(dopsize);

  dopnamesize = fopen(doppath3, "r");
  for (int i = 0; i < SIZE; i++)
    fscanf(dopnamesize, "%ld", &namesize[i]);

  fclose(dopnamesize);

  archive = fopen(afile, "r");
  for (int i = 0; i < elements; i++)
  {
    data[i] = malloc(size[i] + 1);
    for (length = 0; length < size[i]; length++)
    {
      c = fgetc(archive);
      buff[length] = c;
    }
    buff[length] = '\0';
    strcpy(data[i], buff);
  }

  names = fopen(doppath1, "r");
  for (int i = 0; i < elements; i++)
  {
    name[i] = malloc(namesize[i] + 1);
    for (length = 0; length < namesize[i]; length++)
    {
      c = fgetc(names);
      buff[length] = c;
    }
    buff[length] = '\0';
    strcpy(name[i], buff); 
  }

    for (int i = 0; i < elements; i++)
  {
    snprintf(path, sizeof path, "%s/%s", dir, name[i]);
    //snprintf(path, sizeof path, "%s/%s.txt", dir, name[i]);
    if((file = fopen(path, "w")) == NULL)
    {
      printf("Не удалось открыть файл.\n");
      fclose(file);
      return;
    }
    fwrite(data[i], size[i], 1, file);
    fclose(file);
  }
}

int main() 
{
  char afile[50] = {'0'};
  char directory[50] = {'0'};
  int number;

  while(1) 
  {
    printf("\n1 - Архивация\n2 - Разархивация\n3 - Выход\nВведите число: ");
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
        break;

    case 2:
      printf ("Разархивация\n");
      printf ("Введите путь к архиву:\n");
      scanf("%s", afile);

      printf ("Введите путь к директории для разархивации:\n");
      scanf("%s", directory);

      dearch(directory, afile);
      break;

    case 3:
      return 0;

    default:
      printf ("Введено неверное число!\n");
      break;
    }
  }
}