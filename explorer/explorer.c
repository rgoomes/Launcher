#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

char queue[1000000][256];
int qsize=0;
char search[256];

void bfs(){

   DIR *mydir;
   struct dirent *myfile;
   struct stat mystat;
   int sd=0;
   char name[256];

   while(sd < qsize){
      mydir = opendir(queue[sd]);
      //printf("dir: %s\n", argv[1]);
      while((myfile = readdir(mydir)) != NULL){
         if(strcmp(myfile->d_name, "..") != 0 && strcmp(myfile->d_name, ".") != 0){
            sprintf(name, "%s/%s", queue[sd], myfile->d_name);
            //printf("%d\n", qsize*256);
            //printf("%s\n", name);
            stat(name, &mystat);
            if(S_ISDIR(mystat.st_mode)){
               //printf("%s\n", myfile->d_name);
               strcpy(queue[qsize], name);
               qsize++;
            }else{
               if(strstr(myfile->d_name, search) != NULL){
                  printf("%s\n", myfile->d_name);
               }
            }
         }
      }
      closedir(mydir);
      sd++;
   }
}

int main(int argc, char* argv[])
{
   /*DIR *mydir;
   struct dirent *myfile;
   struct stat mystat;

   char name[100];
   mydir = opendir(argv[1]);
   printf("dir: %s\n", argv[1]);
   while((myfile = readdir(mydir)) != NULL){

      sprintf(name, "%s/%s", argv[1], myfile->d_name);
      stat(name, &mystat);
      printf("%d",mystat.st_size);
      printf(" %s %d\n", myfile->d_name, S_ISDIR(mystat.st_mode) );
   }
   closedir(mydir);*/

   strcpy(queue[qsize], argv[1]);
   qsize++;
   strcpy(search, argv[2]);
   bfs();
}