#include <alloc.h>
#include "tcxlwin.h"
#include "pro_achh.h"

char wid_modeli[]="Обычная";
char file_name[20],ifile_name[20],sfile_name[20],ufile_name[20],wfile_name[20];
float file_k[10]={1,1,1,1,1,1,1,1,1,1},file_st[10];
float ifile_k=1,ifile_st=0,sfile_k=1,sfile_st=0;

unsigned long nr=1000,cycle=1;
int nki=4,nko=4,nq=100,qq=4,p=0,q=1,n1=0,wybor=1,pp=50;
int kod_upr=0,nn=25;
#ifdef VIRTUAL
float huge *x[4],huge *y[4],huge *yt[4];
#else
float x[4][NMAX+1],y[4][NMAX+1],yt[4][NMAX+1];
#endif
float w[WMAX+1];
float delt=0.01,fn=1.0e-22,fw=100.0,fdisk;
float xh[WMAX+1],yh[KAN_MAX][KAN_MAX][WMAX+1];
float xk[KAN_MAX+1],yk[KAN_MAX+1];
float a0[4],a1[4]={0.2,0.21,0.22,0.23},a2[4]={0,0,0,0},a3[4]={0,0,0,0};
float f0[4],f1[4]={3,3,3,3},f2[4]={6,6,6,6},f3[4]={9,9,9,9};
float fa0[4],fa1[4]={0,0,0,0},fa2[4]={M_PI/8.0,M_PI/8.0,M_PI/8.0,M_PI/8.0},fa3[4]={M_PI/4.0,M_PI/4.0,M_PI/4.0,M_PI/4.0};
float u0[4]={0,0,0,0};
float psx[CHANNEL][PSMAX],psy[CHANNEL][PSMAX];
//float psi_k[CHANNEL][PMAX],ps_k_v[CHANNEL][PMAX][PMAX];
float del_x_k[6][CHANNEL][PSMAX],del_y_k[6][CHANNEL][PSMAX];
float beta[CHANNEL][PMAX],gamma[CHANNEL];



int AllocBuf(void)
{
#ifdef VIRTUAL
 int val=1,i;

 // free all
 FreeBuf();
 // allocate
 for(i=0;i<CHANNEL;i++)
  {
  x[i]= farcalloc(sizeof(float),nr);
  y[i]= farcalloc(sizeof(float),nr);
  yt[i]=farcalloc(sizeof(float),nr);
  }

 // test for ok
 for(i=0;i<CHANNEL;i++) if(!y[i] || !x[i] || !yt[i])  val=0;

 if(!val)
   {
   FreeBuf();
   Wperror("Нет памяти для выбранной длины ряда");
   }

 return val;
#else
 return 1;
#endif
}

void FreeBuf()
{
 int i;
 // free all
 for(i=0;i<CHANNEL;i++)
   {
   if(y[i])  farfree(y[i]);
   if(x[i])  farfree(x[i]);
   if(yt[i]) farfree(yt[i]);
   }
}


void about()
{
 return;
}
