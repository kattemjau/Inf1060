#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int stringsum(char *s){
  int sum = 0;
  int i;
  for (i = 0; i < s[i] != '\0'; i++) {
    char tempchar = tolower(s[i]);
    int temp = tempchar - 96;

    if(temp < 1 || temp > 26){
      return -1;
    }

    sum = sum + temp;
  }
  printf("%d\n", sum);
  return sum;
}

void stringsum2(char *s, int *res){

}

char *string_between(char *s, char c){
return 0;
}
int distance_between(char *s, char c){
return 0;
}
char **split(char *s){
return 0;
}
