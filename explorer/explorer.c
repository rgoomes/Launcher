#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

char search[256];
int cdir;
int steps=0;

void dfs(char* dir_name, int depth){
   //printf("%d\n", steps++);
   if(depth > cdir){
      return;
   }

   char name[256];
   DIR *cur_dir;
   struct dirent *file;
   struct stat mystat;
   cur_dir = opendir(dir_name);
   while( cur_dir != NULL && (file = readdir(cur_dir)) != NULL){
      if(strcmp(file->d_name, "..") != 0 && strcmp(file->d_name, ".") != 0){
         sprintf(name, "%s/%s", dir_name, file->d_name);
         stat(name, &mystat);
         if(S_ISDIR(mystat.st_mode)){
            //printf("%s\n", myfile->d_name);
            dfs(name, depth+1);
         }else{
            if(strstr(file->d_name, search) != NULL && cdir == depth){
               printf("%s/%s\n", name, file->d_name);
            }
         }
      }
   }
   closedir(cur_dir);


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

   /*strcpy(queue[qsize], argv[1]);
   qsize++;
   strcpy(search, argv[2]);
   bfs();*/

   strcpy(search, argv[2]);
   for(cdir=1; cdir<=10; cdir++){
      printf("cdir: %d\n", cdir);
      dfs(argv[1], 0);
   }
   printf("END\n");

}