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

  struct ruter* rutere[num];

  for (int i = 0; i < num; i++) {

    struct ruter* ru;
    ru = malloc(sizeof(struct ruter));

    unsigned char is;
    fread(&is, 1, 1, infile);
    //printf("%c", is );

    //lese id i unsigned char
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
    char array[lengde];
    fread(array, sizeof(char), lengde-1, infile);
    array[lengde-1]='\0';
    ru->array=array;
    printf("Ruter modell: %s\n", array);

    rutere[i]=ru;
  }


//  while (1) {
    //meny
    char buf[253];
    printf("write soething\n" );
    fgets(buf, 253, stdin);


    printf("%s\n",buf );
  //}



  //avslutte fil
  fclose(infile);

  for (int i = 0; i < num; i++) {
    free(rutere[i]);

  }
  return 0;

}
/*
void meny(){
  innleser
  scanf

  valgmeny
  endre flagg
  fwrite

  endre produsent/modell
  legge inn en ny router (ID og annen data
  slette data fra programmet

  avslutte programmet
}*/

int main(int argc, char *argv[]) {
  if(argc <2){
    return 0;
  }

  //struct ruter* ruters;
  readF(argv[1]);

  //flag != ((active ? 0b11111111:0b00000000) & 0b00000001);


  return 0;
}
