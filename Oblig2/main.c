#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct ruter{
  int id;

  int flagg_active;
  int flagg_wierless;
  int flagg_5gz;
  int flagg_bruk;
  int flagg_endringsnr;

  int lengde;
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
unsigned char num;

void printRuter(){
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

    printf("\n\n" );


  }
}

void readF(char* fil){
  printf("%s\n", fil);
  FILE *infile = fopen(fil, "r");

  if(infile == NULL){
    fprintf(stderr, "couldent open file\n" );
    return;
  }

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
    //printf("Ruter modell: %s\n", array);

    rutere[i]=ru;
  }


  //printRuter(num);

  //avslutte fil
  fclose(infile);
}

int ruterid(){
  printf("Velg hvilken ruter du vil endre fra nummer 0 - %d: \n", num-1);
  char *ruter=lesNavn();
  int rid = atoi(ruter);
  free(ruter);
  if(rid>num){
    printf("Wrong id!\n" );
    return -1;
  }
  return rid;
}

void meny(){
  int rid;
  int k=1;
  while(k){

    printf("\nVelg fra 1-9 hva du vil endre\n" );
    printf("1: print information\n2: flagg_active\n3: flagg_wierless\n4: flagg_5gz\n5: flagg_bruk\n6: flagg_endringsnr\n7. legg til ny router\n8. endre navn\n9: avslutt og skriv til fil\n10: slett fil\n");
    char *navn=lesNavn();

    if(strcmp(navn, "7") == 0){
      struct ruter* ru;
      //ikke freeed
      ru = malloc(sizeof(struct ruter));
      num++;
      printf("wat\n" );
      //invallid size of 8
      rutere=realloc(rutere, sizeof(struct ruter*)*num +1);
      free(navn);
      printf("Write ID: \n" );
      navn=lesNavn();
      ru->id=atoi(navn);

      free(navn);
      printf("Write flagg_active: \n" );
      navn=lesNavn();
      ru->flagg_active=!!(atoi(navn));

      free(navn);
      printf("Write flagg_wierless: \n" );
      navn=lesNavn();
      ru->flagg_wierless=!!(atoi(navn));

      free(navn);
      printf("Write flagg_5gz: \n" );
      navn=lesNavn();
      ru->flagg_5gz=!!(atoi(navn));

      free(navn);
      printf("Write flagg_bruk: \n" );
      navn=lesNavn();
      ru->flagg_bruk=!!(atoi(navn));
      free(navn);
      ru->flagg_endringsnr=0;

      printf("Skriv Ruter modellen\n" );
      navn=lesNavn();
      char *temp=malloc(strlen(navn)+1);
      strcpy(temp, navn);
      ru->array=temp;
      ru->lengde=strlen(temp);

      //num should be char?
      rutere[num-1]=ru;

    }

    else if(strcmp(navn, "9") == 0){
      free(navn);
      break;
    }
    else{
      rid=ruterid();
      if(rid==-1)return;
    }

    if(strcmp(navn, "1") == 0){
      printf("ID: %d\n",rutere[rid]->id );
      printf("Flagg active: %d\n",rutere[rid]->flagg_active );
      printf("Flagg wierless:  %d\n",rutere[rid]->flagg_wierless );
      printf("Flagg 5gz support: %d\n",rutere[rid]->flagg_5gz );
      printf("Flagg bruk: %d\n",rutere[rid]->flagg_bruk );
      printf("Flagg endingsnr: %d\n",rutere[rid]->flagg_endringsnr );
      printf("lengde: %d\n", rutere[rid]->lengde );
      printf("Ruter Modell: ");
      printf("%s\n", rutere[rid]->array);
      printf("\n\n" );
    }
    else if(strcmp(navn, "10")==0){
      free(rutere[rid]->array);
      free(rutere[rid]);
      for(int i=rid;i<num-1;i++){
        rutere[i]=rutere[i+1];
      }
      num=num-1;
      printf("slettet\n" );

    }
    else if(strcmp(navn, "2") == 0){
      rutere[rid]->flagg_active=!rutere[rid]->flagg_active;

    }
    else if(strcmp(navn, "3") == 0){
      rutere[rid]->flagg_wierless=!rutere[rid]->flagg_wierless;

    }
    else if(strcmp(navn, "4") == 0){
      rutere[rid]->flagg_5gz=!rutere[rid]->flagg_5gz;

    }
    else if(strcmp(navn, "5") == 0){
      rutere[rid]->flagg_bruk=!rutere[rid]->flagg_bruk;

    }
    else if(strcmp(navn, "6") == 0){
      free(navn);
      printf("Skriv nytt endringsnr:\n" );
      navn=lesNavn();
      int kis=atoi(navn);
      rutere[rid]->flagg_endringsnr = kis;

    }
    else if(strcmp(navn, "8") == 0){
      free(navn);
      printf("skriv nytt navn pao router\n" );
      navn=lesNavn();
      rutere[rid]->array=realloc(rutere[rid]->array, strlen(navn) +1);
      strcpy(rutere[rid]->array, navn);
      rutere[rid]->lengde=strlen(navn);

    }
    free(navn);
  }


}
void skrivTilFil(char* navn){
  FILE *fil = fopen(navn, "w");

  fwrite(&num, sizeof(int), 1, fil);

  char ch = '\n';
  fwrite(&ch, 1, 1, fil);

  for (int i = 0; i < num; i++) {

  char id=rutere[i]->id;
  fwrite(&id, 1, 1, fil);

  unsigned char flagg=0;
  flagg |= (rutere[i]->flagg_active<<0);
  flagg |= (rutere[i]->flagg_wierless<<1);
  flagg |= (rutere[i]->flagg_5gz<<2);
  flagg |= (rutere[i]->flagg_bruk<<3);
  flagg |= rutere[i]->flagg_endringsnr*16;
  fwrite(&flagg, 1, 1, fil);

  unsigned char length=strlen(rutere[i]->array)+1;
  fwrite(&length, 1, 1, fil);


  char *array=rutere[i]->array;
  fwrite(array, 1, length-1, fil);

  char ch = '\n';
  fwrite(&ch, 1, 1, fil);

}

fclose(fil);


}

int main(int argc, char *argv[]) {
  if(argc <2){
    return 0;
  }

  //struct ruter* ruters;
  readF(argv[1]);

  printRuter(num);
  meny();

  skrivTilFil(argv[1]);

  //free mem
  for (int i = 0; i < num; i++) {
    free(rutere[i]->array);
    free(rutere[i]);
  }
  free(rutere);

  return 0;
}
