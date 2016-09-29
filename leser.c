#include <stdio.h>
#include <stdlib.h>


#define BUF_SIZE 512

char* read_name() {

  char buf[BUF_SIZE];
  char *name;
  char *newline;

  printf("write soething\n" );
  fgets(buf, BUF_SIZE, stdin);

  newline =strchr(buf, '\n');
  if(newline != NULL) {
    *newline = '\0';
  }

  name = malloc(strlen(buf) + 1);
  strcpy(name, buf);


  printf("Du skrev: %s\n", buf );

  return name;
}


int main(void) {

  char *name;

  name = read_name();
  printf("Name: %s\n", name );

  free(name);
  return 0;
}
