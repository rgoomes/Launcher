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

const char* exclude_prefix[] = {"//proc", "//dev", "//usr/lib", "//usr/lib64", "//usr/share", "."};

int check_dir(char *dir){
   int l = strlen(dir), i;
   for(i=0; i<sizeof(exclude_prefix)/sizeof(char*); i++){
      if(strlen(exclude_prefix[i]) <= l && strncmp(dir, exclude_prefix[i], l) == 0)
         return false;

   }
   return true;
}

void dfs(char* dir_name, int depth){
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
         if(S_ISDIR(mystat.st_mode) && check_dir(name)){
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

int main(int argc, char* argv[]){
   if(argc != 3){
      printf("Usage: ./explorer root_search_directory file\n");
      return 0;
   }

   strcpy(search, argv[2]);
   for(cdir=0; cdir<=10; cdir++){
      //printf("cdir: %d\n", cdir);
      dfs(argv[1], 0);
   }
   printf("END\n");

}