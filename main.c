#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

struct pop_entry {
  int year;
  int population;
  char boro[15];
};

int main();
int read_csv();

int main() {
  read_csv();
  return 0;
}
int read_csv() {
  int fd = open("nyc_pop.csv",O_RDONLY);
  struct stat stp;
  stat("nyc_pop.csv",&stp);
  char *file_contents = malloc(stp.st_size);
  read(fd,file_contents,stp.st_size);
  close(fd);

  printf("%s\n",file_contents);
  char *pt = file_contents;
  int rows = -1;
  while (pt = strchr(pt+1,'\n')) {
    rows++;
  }
  printf("%d\n",rows);

  struct pop_entry *list = malloc(sizeof(struct pop_entry)*5*rows);
  int i;
  
  pt = file_contents;
  while (pt = strchr(pt+1,'\n')) {
    char *newline = strchr(pt+1,'\n');
    if (newline != NULL) {*newline = '\0';}

    strncpy(list[i].boro,"Manhattan",14);
    strncpy(list[i+1].boro,"Brooklyn",14);
    strncpy(list[i+2].boro,"Queens",14);
    strncpy(list[i+3].boro,"Bronx",14);
    strncpy(list[i+4].boro,"Staten Island",14);
    int year;
    sscanf(pt,"%d,%d,%d,%d,%d,%d",&year,list[i].population,list[i+1].population,list[i+2].population,list[i+3].population,list[i+4].population);
    printf("%s\n",pt);


    if (newline != NULL) {*newline = '\n';}
    i += 5;
  }

  free(file_contents);
  return 0;

}
