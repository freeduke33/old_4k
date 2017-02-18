//Общие характеристики
//   <DSYS.C>

//#define IMMITATOR

#include "tcxlwin.h"

#include "dac_adc.h"
#include "pro_achh.h"

static float wxy[4][4]={
		{0.92,0.10,0.07,0.06},
		{0.08,0.90,0.10,0.06},
		{0.08,0.07,0.80,0.10},
		{0.10,0.07,0.06,0.70}
		};


int Enable(void)
{
#ifdef IMMITATOR
 return DRV_OK;
#else
 int val;

 DrvSetChannel(nki,nko);
// Wperror(" Отключите каналы управления ");
 val=DrvEnable(1.0/delt);
 if(val==DRV_OK) Razgruzka();
// Wperror(" Подключите каналы управления ");
 return val;
#endif
}

void Disable(void)
{
#ifndef IMMITATOR
 Razgruzka();
 DrvDisable();
#endif
 return;
}

/*****************/
void Imit_Driv(void)
{
int k,j;
float su1;

#ifndef IMMITATOR
DrvVolume(xk,yk);
// fprintf(stdout,"%g %g %g %g\t",yk[0],yk[1],yk[2],yk[3]);
// fprintf(stdout,"<-- %g %g %g %g\n",xk[0],xk[1],xk[2],xk[3]);
#else

for(j=0;j<nko;j++)
  {
  for(su1=k=0;k<nki;k++)
    su1+=wxy[k][j]*xk[k];
  yk[j]=su1;
  }
#endif
}
//*************************************
void per_pr(void)
{
int i;
for(i=0;i<=10;i++) Imit_Driv();
}
//*************************************
void Out_kan(int i,int m)
{
int j;
for(j=0;j<nko;j++)  yh[i][j][m]=yk[j];
}
//*************************************
void Out_y(int m)
{
int j;
for(j=0;j<nko;j++)  y[j][m]=yk[j];
}

//*************************************
void shag(int m)
{
m=6;
}
//*************************************
void StartUp(void)
{
#ifndef IMMITATOR
 if(DrvIsPresent())
   {
   if(DrvInitialize()!=DRV_OK)
      {
      printf("\nUnable to initialize UPI driver\n");
      exit(-2);
      }
   if(Enable()==DRV_OK) Disable();
   }
 else
   {
   printf("\nUPI driver not present !\n");
   exit(-1);
   }
#endif
 return;
}

void ShutDown(void)
{
#ifndef IMMITATOR
 DrvDeinitialize();
#endif
 return;
}


void GotoNull(void)
{

 return;
}


void Razgruzka(void)
{
 float dx[CHANNEL];
 int nx=10,i,k,d;

// DrvGetVolADC(xk);
 for(k=0;k<nko;k++) xk[k]=0;

 for(k=0;k<nko;k++) dx[k]=xk[k]/(float)nx;

 for(i=0;i<nx;i++)
   {
   for(d=0;d<10;d++) Imit_Driv();
   for(k=0;k<nko;k++) xk[k]-=dx[k];
   }
 for(k=0;k<nko;k++) xk[k]=0.0;
 for(d=0;d<10;d++) Imit_Driv();

 return;
}
