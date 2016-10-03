#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct ruter{
  unsigned char id;

  int flagg_active;
  int flagg_wierless;
  int flagg_5gz;
  int flagg_bruk;
  int flagg_endringsnr;

  unsigned char lengde;
  //modell har maks lengde 253
  char* array;
};

char* lesNavn() {

  char buf[512];
  char *name;
  char *newline;

  fgets(buf, 512, stdin);

  newline =strchr(buf, '\n');
  if(newline != NULL) {
    *newline = '\0';
  }

  name = malloc(strlen(buf) + 1);
  strcpy(name, buf);

  return name;
}

struct ruter** rutere;

void printRuter(int num){
  for (int i = 0; i < num; i++) {
    printf("ID: %d\n",rutere[i]->id );
    printf("Flagg active: %d\n",rutere[i]->flagg_active );
    printf("Flagg wierless:  %d\n",rutere[i]->flagg_wierless );
    printf("Flagg 5gz support: %d\n",rutere[i]->flagg_5gz );
    printf("Flagg bruk: %d\n",rutere[i]->flagg_bruk );
    printf("Flagg endingsnr: %d\n",rutere[i]->flagg_endringsnr );

    int lengde = rutere[i]->lengde;
    printf("lengde: %d\n", lengde );

    printf("Ruter Modell: ");
    printf("%s\n", rutere[i]->array);
    /*for (int k = 0; k < lengde; k++) {
    printf("%s", rutere[i]->array[k]  );
  }*/
  printf("\n\n" );


}
}

int readF(char* fil){
  printf("%s\n", fil);
  FILE *infile = fopen(fil, "r");

  if(infile == NULL){
    fprintf(stderr, "couldent open file\n" );
    return 0;
  }

  int num;
  fread(&num, sizeof(int), 1, infile);
  //  printf("Num of ruter : %d\n", num );

  rutere=malloc(sizeof(struct ruter)*num);

  for (int i = 0; i < num; i++) {

    struct ruter* ru;
    ru = malloc(sizeof(struct ruter));

    unsigned char is;
    fread(&is, 1, 1, infile);
    //printf("%c", is );

    //lese id i unsigned
    unsigned char id;
    fread(&id, sizeof(char), 1, infile);
    ru->id=id;
    //  printf("%d\n", id);

    //lese "ruter" sitt FLAGG
    char flagg;
    fread(&flagg, 1, 1, infile);
    ru->flagg_active=!!(flagg & 0b00000001);
    ru->flagg_wierless=!!(flagg & 0b00000010);
    ru->flagg_5gz=!!(flagg & 0b00000100);
    ru->flagg_bruk=!!(flagg & 0b00001000);
    ru->flagg_endringsnr=(flagg&0b11110000)/16;
    //  printf("active :%d\n", ru->flagg_active);

    unsigned char lengde;
    fread(&lengde, 1, 1, infile);
    //  printf("lengde %d\n", lengde );
    ru->lengde=lengde;


    //faktisk lese inn char array
    char *name;
    char array[lengde];
    fread(array, sizeof(char), lengde-1, infile);
    array[lengde-1]='\0';

    //printf("here\n" );
    name=malloc(strlen(array) +1 );
    strcpy(name, array);

    ru->array=name;
    printf("Ruter modell: %s\n", array);

    rutere[i]=ru;
  }


  printRuter(num);

  //avslutte fil
  fclose(infile);
  return num;

}

void meny(int num){
  char *buf;
  buf="this";
int k=1;
  while(k){
    printf("\nVelg fra 1-9 hva du vil endre\n" );
    printf("1: id\n2: flagg_active\n3: flagg_wierless\n4: flagg_5gz\n5: flagg_bruk\n6:flagg_endringsnr\n7. lengde\n8. endre navn\n9: avslutt ");

    char *navn=lesNavn();
    if(strcmp(navn, "1") == 0){
      printf("1\n" );
    }
    if(strcmp(navn, "2") == 0){

    }
    if(strcmp(navn, "3") == 0){

    }
    if(strcmp(navn, "4") == 0){

    }
    if(strcmp(navn, "5") == 0){

    }
    if(strcmp(navn, "6") == 0){

    }
    if(strcmp(navn, "7") == 0){

    }
    if(strcmp(navn, "8") == 0){

    }
    if(strcmp(navn, "9") == 0){
      free(navn);
      break;
    }

    free(navn);
    if(k==3)break;

    k++;
  }



  /*
  endre flagg
  fwrite
  //flag != ((active ? 0b11111111:0b00000000) & 0b00000001);

  endre produsent/modell
  legge inn en ny router (ID og annen data
  slette data fra programmet

  avslutte programmet */
}

int main(int argc, char *argv[]) {
  if(argc <2){
    return 0;
  }

  //struct ruter* ruters;
  int num=readF(argv[1]);

  meny(num);

  //free mem
  for (int i = 0; i < num; i++) {
    free(rutere[i]->array);
    free(rutere[i]);
  }
  free(rutere);

  return 0;
}
