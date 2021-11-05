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

int main(int argc, char *argv[]);
int read_csv();
struct pop_entry *read_data();
int add_data();
int update_data();

int main(int argc, char *argv[]) {
  if (argc <= 1) {
	printf("Options: -read_csv -read_data -add_data -update_data\n");  
	exit(-1);
  }
  if (!strcmp("-read_csv",argv[1])) {
	return read_csv();
  } else if (!strcmp("-read_data",argv[1])) {
	void *p = read_data();
	if (p != NULL) {
		free(p); 
		return 0;
	}
	return 1;
  } else if (!strcmp("-add_data",argv[1])) {
	return add_data();
  } else if (!strcmp("-update_data",argv[1])) {
	update_data();
	return 1;
  } else {
	printf("Options: -read_csv -read_data -add_data -update_data\n");  
	exit(-1);
  }
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
  //printf("%d\n",rows);

  struct pop_entry *list = malloc(sizeof(struct pop_entry)*5*rows);
  int i = 0;
  
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
    
	int index = 0;
	if (sscanf(pt+1,"%d,%d,%d,%d,%d,%d",&year,&(list[i].population),&(list[i+1].population),&(list[i+2].population),&(list[i+3].population),&(list[i+4].population)) < 6) {
		//printf("sscanf error!\n");
		break;
	}
	list[i].year = year;
	list[i+1].year = year;
	list[i+2].year = year;
	list[i+3].year = year;
	list[i+4].year = year;
	
    if (newline != NULL) {*newline = '\n';}
    i += 5;
  }

  fd = open("nyc_data.bin",O_WRONLY | O_CREAT | O_TRUNC);
  if (fd == -1) {printf("%s\n",strerror(errno)); return errno;}
  if (write(fd,list,sizeof(struct pop_entry)*5*rows) < sizeof(struct pop_entry)*5*rows) {printf("Write error: %s\n",strerror(errno));}
  close(fd);
  
  free(list);
  free(file_contents);
  return 0;

}

struct pop_entry *read_data() {
	int fd = open("nyc_data.bin",O_RDONLY);
	if (fd == -1) {printf("%s\n",strerror(errno)); return NULL;}
	struct stat stp;
	stat("nyc_data.bin",&stp);
	struct pop_entry *list = malloc(stp.st_size);
	read(fd,list,stp.st_size);
	int length = stp.st_size / sizeof(struct pop_entry);
	int i;
	for (i = 0; i < length; i++) {
		printf("[%d] Year: %d Borough: %s Population: %d\n",i,list[i].year,list[i].boro,list[i].population);
	}
	close(fd);
	
	return list;
}

int add_data() {
	struct pop_entry entry;
	char *borough;
	
	char input[256];
	input[0] = 0; input[255] = 0;
	do {
		printf("\nYear: ");
		fgets(input,256,stdin);
	} while (sscanf(input,"%d\n",&(entry.year)) != 1);
	do {
		printf("\nBorough Name: ");
		fgets(input,256,stdin);
	} while (sscanf(input,"%ms\n",&borough) != 1);
	strncpy(entry.boro,borough,15-1);
	free (borough);
	do {
		printf("\nPopulation: ");
		fgets(input,256,stdin);
	} while (sscanf(input,"%d\n",&(entry.population)) != 1);
	
	int fd = open("nyc_data.bin",O_RDONLY);
	if (fd == -1) {printf("%s\n",strerror(errno)); return errno;}
	struct stat stp;
	stat("nyc_data.bin",&stp);
	struct pop_entry *list = malloc(stp.st_size + sizeof(struct pop_entry));
	read(fd,list,stp.st_size);
	close(fd);
	int length = stp.st_size / sizeof(struct pop_entry);
	list[length] = entry;
	fd = open("nyc_data.bin",O_WRONLY);
	if (fd == -1) {printf("%s\n",strerror(errno)); return errno;}
	write(fd,list,stp.st_size+sizeof(struct pop_entry));
	close(fd);
	
	return 0;
}

int update_data() {
	struct pop_entry *list = read_data();
	
	struct pop_entry entry;
	int index, first_wrong_index;
	char *borough;
	struct stat stp;
	stat("nyc_data.bin",&stp);
	first_wrong_index = stp.st_size / sizeof(struct pop_entry);
	
	char input[256];
	input[0] = 0; input[255] = 0;
	do {
		printf("\nIndex to Modify: ");
		fgets(input,256,stdin);
	} while (sscanf(input,"%d\n",&index) != 1 || index < 0 || index >= first_wrong_index);
	do {
		printf("\nYear: ");
		fgets(input,256,stdin);
	} while (sscanf(input,"%d\n",&(entry.year)) != 1);
	do {
		printf("\nBorough Name: ");
		fgets(input,256,stdin);
	} while (sscanf(input,"%ms\n",&borough) != 1);
	strncpy(entry.boro,borough,15-1);
	free (borough);
	do {
		printf("\nPopulation: ");
		fgets(input,256,stdin);
	} while (sscanf(input,"%d\n",&(entry.population)) != 1);
	// this sscanf doesnt like me
	
	//printf("[%d] Year: %d Borough: %s Population: %d\n",index,entry.year,entry.boro,entry.population);
	list[index] = entry;
	//printf("[%d] Year: %d Borough: %s Population: %d\n",index,list[index].year,list[index].boro,list[index].population);
	
	int fd = open("nyc_data.bin",O_WRONLY);
	if (fd == -1) {printf("%s\n",strerror(errno)); return errno;}
	if (-1 == write(fd,list,stp.st_size)) {printf("%s\n",strerror(errno));}
	close(fd);
	
	free(list);
	return 0;
}