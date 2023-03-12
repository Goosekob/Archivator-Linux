#include <stdio.h>
#include <dirent.h>
#include <string.h>


#define SIZE  512

int main() 
{
    char afile[50];
    char directory[50];
    FILE *file;
   // char buffer[100];

    size_t num;
   // char   buffer[10]; //буффер для записи

    char   buffer[SIZE]; //буффер для записи

    printf ("Введите путь к архиву: ");
    scanf("%s", afile);
    FILE *archive;  

    printf ("Введите путь к директории для архивации: \n");
    scanf("%s", directory);

    //DIR *mydir = opendir(directory);

    //struct dirent *entry;
   // while(entry = readdir(mydir)) 

    DIR *mydir;
    struct dirent *entry;
    mydir = opendir(directory);
 /*     if (!mydir) {
        perror("diropen");
        exit(1);
    };
  */
  //  archive = fopen (afile, "wb");
    while ( (entry = readdir(mydir)) != NULL)
  {
    printf("%s", entry->d_name);
    if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0) continue;
    if((file = fopen(entry->d_name, "r")) == NULL)   //обработка ошибки открытия файлов
    {
        printf("Could not open file.\n");
        return 1;
    }
 
    if((archive = fopen(afile, "a+")) == NULL)
    {
        printf("Could not open afile.\n");
        return 2;
        fclose(archive);
    }

    archive = fopen(afile, "a+");  
    while((num = fread(buffer, sizeof(char), SIZE, file)) > 0)  //чтение копируемого файла до конца
    {
      fwrite(buffer, sizeof(char), num, archive);//запись копии в новый файл
    }
    fclose(archive);
    fclose(file);
  }
    closedir(mydir);
}