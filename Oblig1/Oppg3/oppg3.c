#include <stdio.h>


struct datetime{
  int timer;
  int minutter;
  int sekunder;

  int dag;
  int maned;
  int ar;
};


void init_datetime(struct datetime* dt ){
  //init default tidspunkt
  dt-> timer = 12;
  dt-> minutter = 20;
  dt-> sekunder = 03;

  dt-> dag=13;
  dt-> maned =9;
  dt-> ar = 2016;
}

void datetime_set_date(struct datetime* dt, int dag,int maned,int ar ){
  dt->dag=dag;
  dt->maned=maned;
  dt->ar=ar;

}
void datetime_set_time(struct datetime* dt, int timer,int minutter,int sekunder){
  dt-> timer = timer;
  dt-> minutter = minutter;
  dt-> sekunder = sekunder;
}
void datetime_diff(struct datetime* dt_from, struct datetime* dt_to, struct datetime* dt_res){
  //while lokke for a beregne tid mellom 2 punkter
  int tim, min, sek;
  int da, ma, yr;

  int t=1;

  tim=dt_to->timer - dt_from->timer;
  //printf("%d \n", tim);
  //printf("%d\n", dt_to->timer);
  //printf("%d\n", dt_from->timer );

  min=dt_to->minutter - dt_from->minutter;
  sek=dt_to-> sekunder- dt_from->sekunder;
  da=dt_to-> dag- dt_from->dag;
  ma=dt_to->maned - dt_from->maned;
  yr=dt_to-> ar- dt_from->ar;
  while(t){
    t=0;
    if(tim <0){
      da++;
      t=1;
      tim=tim+24;
    }
    if(min<0){
      min=min+60;
      t=1;
      tim++;
    }
    if(sek<0){
      sek=sek+60;
    //  printf("%d\n", sek );
      t=1;
      min++;
    }
    if(da<0){
      ma++;
      da=da+31;
      t=1;
    }
    if(ma<0){
      ma=ma+12;
      yr++;
      t=1;
    }



  }
  dt_res->timer=tim;
  dt_res->minutter=min;
  dt_res->sekunder=sek;

  dt_res->dag=da;
  dt_res->maned=ma;
  dt_res->ar=yr;

}

int main(int argc, char const *argv[]) {
  struct datetime dt;
  init_datetime(&dt);

  //printer tidspunkt etc.
  printf("klokken er: \n" );
  printf("%d.", dt.timer);
  printf("%d.", dt.minutter);
  printf("%d\n", dt.sekunder);
  printf("datoen er:\n" );
  printf("%d.", dt.dag);
  printf("%d.", dt.maned);
  printf("%d\n", dt.ar);
  printf("\n");

  datetime_set_date(&dt, 1, 1, 2012 );

  datetime_set_time(&dt, 13, 45, 50);

  //printer tidspunkt etc.
  printf("klokken er: \n" );
  printf("%d.", dt.timer);
  printf("%d.", dt.minutter);
  printf("%d\n", dt.sekunder);
  printf("datoen er:\n" );
  printf("%d.", dt.dag);
  printf("%d.", dt.maned);
  printf("%d\n", dt.ar);



  struct datetime dt_from;
  struct datetime dt_to;
  struct datetime dt_res;

  datetime_set_date(&dt_from, 1, 1, 2012 );
  datetime_set_time(&dt_from, 13, 45, 50);

  datetime_set_date(&dt_to, 1, 8, 2015 );
  datetime_set_time(&dt_to, 14, 12, 12);

  datetime_diff(&dt_from, &dt_to, &dt_res);

  //printer forskjell i tidspunk
  printf("tids forskjellen er: \n" );
  printf("%d.", dt_res.timer);
  printf("%d.", dt_res.minutter);
  printf("%d\n", dt_res.sekunder);
  printf("dato forskjellen er:\n" );
  printf("%d.", dt_res.dag);
  printf("%d.", dt_res.maned);
  printf("%d\n", dt_res.ar);

  return 0;
}
