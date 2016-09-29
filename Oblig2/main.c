#include <stdlib.h>
#include <stdio.h>

struct ruter{
  unsigned char id;
  unsigned char FLAGG;
  unsigned char lengde;
  //modell har maks lengde 253
  char* array;
};

int readF(char* fil){
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

  struct ruter* rutere[254];

  for (int i = 0; i < num; i++) {

    //her mao du mallokere
    struct ruter* ru=malloc(sizeof(*ru));
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
    printf("%x\n", flagg);
    //iterrere over bitsene til flagg
    ru->FLAGG=flagg;

    unsigned char lengde;
    fread(&lengde, 1, 1, infile);
    printf("lengde %d\n", lengde );
    ru->lengde=lengde;
    //faktisk lese inn char array
    char array[lengde];
    fread(array, sizeof(char), lengde-1, infile);
    ru->array=array;
    printf("Ruter modell: %s\n", array);

    //sette strukt inni array
    rutere[i]=ru;

  }



  fclose(infile);

  return 0;

}

int main(int argc, char const *argv[]) {
  if(argc <2){
    return 0;
  }
  printf("%s\n", argv[1] );
  readF(argv[1]);


  return 0;
}
