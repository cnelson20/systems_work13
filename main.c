#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

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

  //printf("%s\n",file_contents);
  char *pt = file_contents;
  int rows = -1;
  while (pt = strchr(pt+1,'\n')) {
    rows++;
  }
  printf("%d\n",rows);

  struct pop_entry *list = malloc(sizeof(struct pop_entry)*5*rows);
  int i = 0;
  
  pt = file_contents;
  while (pt = strchr(pt+1,'\n')) {
    char *newline = strchr(pt+1,'\n');
	char *carriage = strchr(pt+1,'\r');
    if (newline != NULL) {*newline = '\0';}
	if (carriage != NULL) {*carriage = '\0';}

    strncpy(list[i].boro,"Manhattan",14);
    strncpy(list[i+1].boro,"Brooklyn",14);
    strncpy(list[i+2].boro,"Queens",14);
    strncpy(list[i+3].boro,"Bronx",14);
    strncpy(list[i+4].boro,"Staten Island",14);
    int year = 0;
    
	sscanf(pt,"%d,%d,%d,%d,%d,%d",&year,list[i].population,list[i+1].population,list[i+2].population,list[i+3].population,list[i+4].population);
	list[i].year = year;
	list[i+1].year = year;
	list[i+2].year = year;
	list[i+3].year = year;
	list[i+4].year = year;
	
    printf("%s",pt);

    if (newline != NULL) {*newline = '\n';}
	if (carriage != NULL) {*carriage = '\r';}
    i += 5;
  }
  //for (i = 0; i < sizeof(struct pop_entry)*5*rows; i++) {
	//printf("%c",*(((char *)list)+i));
  //}
  printf("\n");
  fd = open("nyc_data.bin",O_WRONLY);
  if (fd == -1) {printf("%s\n",strerror(errno)); return -1;}
  write(fd,list,sizeof(struct pop_entry)*5*rows);
  close(fd);
  

  free(file_contents);
  return 0;

}
