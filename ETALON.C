
#include <math.h>
#include <stdio.h>

#include "tcxlwin.h"
#include "pro_achh.h"

VOID CTYP Inf_Form(NOARG);

/*--------------------  Ф У Н К Ц И И -----------------*/
/*Блок формирования эталонного сигнала*/
int etalon(void)
{
unsigned long i,_nr;
short k;
int val=0,_nk;
float _freq;
FILE *f;

Inf_Form();

switch(wybor)
  {
  case 1: /* полигамоника */
    for(i=0;i<nr;i++)
      {
      for(k=0;k<nko;k++)
	 {
	 yt[k][i]=a1[k]*sin(6.28*delt*i*f1[k]+fa1[k])+
		  a2[k]*sin(6.28*delt*i*f2[k]+fa2[k])+
		  a3[k]*sin(6.28*delt*i*f3[k]+fa3[k])+u0[k];
//	 printf("%f\t",yt[k][i]);
	 }
//     printf("\n");
      }
    val=1;
    break;
  case 2: /*  из файла */
    f=fopen(file_name,"r+t");
    if(f==NULL) Wperror("Нет файла эталонного сигнала");
    else
      {
      fscanf(f,"%u %f %lu",&_nk,&_freq,&_nr);
      if(_nk<nko || nr>_nr) Wperror("Несовпадение параметров эталонного сигнала с рабочими");
      else
        {
        delt=1.0/_freq;
//        if(nr>_nr) nr=_nr;

        for(i=0;i<=nr;i++)
          {
          for(k=0;k<_nk;k++)
            {
            if(k<nko) { fscanf(f,"%f",&yt[k][i]); yt[k][i]=(yt[k][i]*file_k[k]+file_st[k])/5.0; }
            else fscanf(f,"%f",&_freq);
            }
          }
        val=1;
        }
      fclose(f);
      }
    break;
  }

Wclose();
return val;
}


void from_stend(void)
{
 return;
}
