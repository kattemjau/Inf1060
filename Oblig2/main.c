#include <stdlib.h>
#include <stdio.h>

struct ruter{
  unsigned char id;

  int flagg_active;
  int flagg_wierless;
  int flagg_5gz;
  int flagg_bruk;

  unsigned char lengde;
  //modell har maks lengde 253
  char* array;
};

int readF(char* fil, struct ruter* ru){
  printf("%s\n", fil);
  FILE *infile = fopen(fil, "r");

  if(infile == NULL){
    fprintf(stderr, "couldent open file\n" );
    return -1;
  }


  //forste tall er en 4 bytes in som hvor mange rutere filen har info om.
  //resten er maks 256 bytes

  //bruker fread for a leste forste int
  int num;
  fread(&num, sizeof(int), 1, infile);
  printf("Num of ruter : %d\n", num );

  //struct array
  struct ruter* rutere[num];

  for (int i = 0; i < num; i++) {

    ru = malloc(sizeof(struct ruter));
    //bruker fread for a lese linjeskift
    unsigned char is;
    fread(&is, 1, 1, infile);
    //printf("%c", is );


    //lese id i unsigned char
    unsigned char id;
    fread(&id, sizeof(char), 1, infile);

    ru->id=id;
    //printe ut som en int
    printf("%d\n", id);

    //lese "ruter" sitt FLAGG
    unsigned char flagg;
    fread(&flagg, 1, 1, infile);
    //iterrere over bitsene til flagg

    ru->flagg_active=!!(flagg & 0b00000001);
    ru->flagg_wierless=!!(flagg & 0b00000010);
    ru->flagg_5gz=!!(flagg & 0b00000100);
    ru->flagg_bruk=!!(flagg & 0b00001000);

    printf("active :%d\n", ru->flagg_active);


    unsigned char lengde;
    fread(&lengde, 1, 1, infile);
    printf("lengde %d\n", lengde );
    ru->lengde=lengde;


    //faktisk lese inn char array
    char array[254];
    fread(array, sizeof(char), lengde-1, infile);
    array[lengde-1]='\0';
    ru->array=array;
    printf("Ruter modell: %s\n", array);

    //sette strukt inni array
    rutere[i]=ru;

    //memset(array, 1, 254);
  }
/*
  for (size_t i = 0; i < 5; i++) {
    unsigned char length = rutere[i]->lengde;
    printf("Lengde: %d\n",length );
  }*/



  fclose(infile);
  for (size_t i = 0; i < num; i++) {
    free(rutere[i]);

  }
  return 0;

}


int main(int argc, char const *argv[]) {
  if(argc <2){
    return 0;
  }

  struct ruter ru;
  readF(argv[1], &ru);

  //SKRIVE TIL flagg
  //flag != ((active ? 0b11111111:0b00000000) & 0b00000001);


  return 0;
}
