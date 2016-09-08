#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int stringsum(char *s){
  int sum = 0;
  int i;
  //itererer igennom lista, breaker pa slutten av stringen
  for (i = 0; s[i] != '\0'; i++) {
    char tempchar = tolower(s[i]);
    int temp = tempchar - 96;

    //sjekker om det er ugyldige verdier
    if(temp < 1 || temp > 26){
      return -1;
    }

    //regner ut summen obviously
    sum = sum + temp;
  }
  printf("%d\n", sum);
  return sum;
}

void stringsum2(char *s, int *res){
  int sum = 0;
  int i;
  //itererer igennom lista, breaker pa slutten av stringen
  for (i = 0; s[i] != '\0'; i++) {
    char tempchar;

    if(islower(s[i]))
      tempchar = s[i];
    else
      tempchar = tolower(s[i]);

    int temp = tempchar - 96;

    //sjekker om det er ugyldige verdier
    if(temp < 1 || temp > 26){
      *res =-1;
      return;
    }

    //regner ut summen obviously
    sum = sum + temp;
  }
  printf("%d\n", sum);
  *res = sum;
}

char *string_between(char *s, char c){
  int a=0, i, k;
  char* ramPeker;
  //printf("%c\n", c );

  for (i = 0; s[i] != '\0'; i++) {
    if(s[i] == c){
        if(a){
          ramPeker = malloc(sizeof (char) *(i - a));
          printf("cradh\n");

          for (k = 0; k < (i - a); k++) {
            ramPeker[k] = s[a+k];
          }
          printf("cradk\n");

          return ramPeker;
          }
        else{
          a=i+1;
        }
    }
  }
  return NULL;
}
int distance_between(char *s, char c){
  int teller=0;
  bool keep_going = false;
  int i;
  //printf("Bokstaven er: %c\n", c);
  //itterere igjennom lista
  for (i = 0; i < s[i] != '\0'; i++) {
  //  printf("%c\n", s[i]);
    if(s[i] == c){
    //  printf("Fant: %c\n", c );

      if(keep_going==true){
        //printf("continues\n");
        keep_going=false;
        break;
      }
      keep_going=true;
    }
    //printf(" keeps going =1: %d\n", keep_going );
    if(keep_going==true){
      teller++;
    //  printf("teller\n" );
    }
  }
  //printf("%d\n", teller);
  if(teller <=2 || keep_going ==true){
    //printf("feil\n" );
    return -1;
  }

  return teller;
}
char **split(char *s){
  /*char
  int i;
  for (i = 0; s[i] != '\0'; i++) {

  }
*/
  return false;
}
