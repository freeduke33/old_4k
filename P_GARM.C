//Функции управления с моделью и без в разомкнутом режиме

#include <math.h>

#include "tcxlwin.h"
#include "dac_adc.h"
#include "pro_achh.h"


void p_garm(void)
{
 int  j,k;
 unsigned long i,cl;
 extern int pp;
 float del_x[KAN_MAX];


 if(!AllocBuf() || !etalon()) return;

 Inf_Rab();

 if(Enable()==DRV_OK)

 {
 //Плавный выход на начало
 for(k=0;k<nki;k++)
   del_x[k]=yt[k][0]/(float)pp;
 for(i=0;i<=pp;i++)
   {
   for(k=0;k<nki;k++)
     xk[k]=(float)i*del_x[k];
   Imit_Driv();
   }
 for(cl=0;cl<cycle;cl++)
   {
   for(i=0;i<nr;i++)
     {
     for(k=0;k<nki;k++) x[k][i]=xk[k]=-yt[k][i];
     Imit_Driv();
     for(k=0;k<nki;k++) y[k][i]=yk[k];
     }
   //Плавный переход к началу внутри цикла
   if((cl+1)<(cycle))
     {
     for(k=0;k<nki;k++)
       del_x[k]=(yt[k][nr-1]-yt[k][0])/(float)pp;
     for(i=0;i<=10;i++)
       {
       for(k=0;k<nki;k++)
	 xk[k]=yt[k][nr-1]+(float)i*del_x[k];
       Imit_Driv();
       }
     }
   }
 //Плавный выход в конце эксперимента
  for(k=0;k<nki;k++)
    del_x[k]=yt[k][nr-1]/(float)pp;
  for(i=0;i<=pp;i++)
    {
    for(k=0;k<nki;k++)
      xk[k]=yt[k][nr-1]-(float)i*del_x[k];
    Imit_Driv();
    }
    for(k=0;k<nki;k++)
      xk[k]=0.0;
    Imit_Driv();
  Disable();
  graf_time();
  } else printf("\007\007\007");

 Wclose();
 return;
}

//*********************************************
//Функция работы с моделью в разомкнутом режиме
//*********************************************
void m_p_garm(void)
{
 static float cr[CHANNEL][PMAX];

 int k,j,v,n_uch,m,n0,n2,kp;
 unsigned long i,cl;
 float del_x[KAN_MAX],s0,s1,su1,su2,su3;
// int kod_upr,nn;

//float Del_x(int),Del_y(int),Psi_k(int),Psi_kv(int,int),Psi(void);

 kod_upr=0;

 n_uch=20;
 n0=40;
 n2=40;

 if(!AllocBuf() || !etalon()) return;

 Inf_Rab();

 if(Enable()==DRV_OK)

 {

 // Подготовительные операции разомкнутого упр. с моделью
for(k=0;k<nki;k++)
  del_x[k]=yt[k][n_uch-1]/(float)n_uch;
for(j=0;j<n_uch;j++)
  {
  for(k=0;k<nki;k++)
    x[k][j]=yt[k][j]=(float)j*del_x[k];
  }
for(j=n_uch;j<=n0;j++)
  {
  for(k=0;k<nki;k++)
    x[k][j]=yt[k][j];
  }

//Первый технологический прогон
for(k=0;k<nki;k++)
  xk[k]=x[k][0];
Imit_Driv();
for(k=0;k<nki;k++)
  y[k][0]=yk[k];

for(i=1;i<=n0;i++)
  {
  for(k=0;k<nki;k++)
    xk[k]=x[k][i];
  Imit_Driv();
  for(k=0;k<nki;k++)
    {
    y[k][i]=yk[k];
    del_x_k[0][k][i]=x[k][i];
    del_y_k[0][k][i]=y[k][i];
    for(v=1;v<=n1+3;v++)
      {
      if(i==0)
	del_x_k[v][k][i]=del_x_k[v-1][k][i];
      else
	del_x_k[v][k][i]=del_x_k[v-1][k][i]-del_x_k[v-1][k][i-1];
      if(v>n1+1)
	continue;
      else
	{
	if(i==0)
	  del_y_k[v][k][i]=del_y_k[v-1][k][i];
	else
	  del_y_k[v][k][i]=del_y_k[v-1][k][i]-del_y_k[v-1][k][i-1];
	}
      }
    }
  }

//Построение модели
switch(p)
  {
  case 0:
    for(k=0;k<nki;k++)
      {
      for(su1=su3=0,m=0;m<=nn+qq-1;m++)
	{
	su1+=del_x_k[kod_upr][k][m]*del_y_k[kod_upr][k][m+qq];
	su3+=del_x_k[kod_upr][k][m]*del_x_k[kod_upr][k][m];
	}
      beta[k][0]=su3/su1;
      }
  break;

  case 1:
    for(k=0;k<nki;k++)
      {
      for(kp=1;kp<=p;kp++)
	{
	for(v=0;v<=kp;v++)
	  {
	  for(su1=su2=su3=0,m=0;m<=nn+qq-1;m++)
	    {
	    if(v==kp)
	      su1+=del_x_k[kod_upr][k][m]*del_y_k[kod_upr][k][m+qq];
	    su2+=del_y_k[kod_upr][k][m+qq-kp]*del_y_k[kod_upr][k][m+qq-v];
	    if(kp==1&&v==0)
	      su3+=del_x_k[kod_upr][k][m]*del_x_k[kod_upr][k][m];
	    }
	  }

	}

//     beta[k][0]=su3/su1;
      }
  break;

  case 2:
  break;
  }

//Генерация вх.массива

for(cl=0;cl<cycle;cl++)
{
for(i=n0;i<nr;i++)
  {
  for(k=0;k<nki;k++)
    {
    switch(n1)
      {
      case 0:
	x[k][i]=x[k][i-1-q]+(yt[k][i+q-1]-y[k][i-1])/cr[k][0];
      break;
      case 1:
	x[k][i]=x[k][i-1-q]*(float)(q+2)-x[k][i-2-q]*(float)(q+1)+(yt[k][i+q]-y[k][i-1]*(float)(q+2)+y[k][i-2]*(float)(q+1))/cr[k][0];
      break;
      }
    if(fabs(del_x[k]=x[k][i]-x[k][i-1])>0.1*(xmax-xmin))
      {
      x[k][i]=x[k][i-1]+0.1*del_x[k];
      if(x[k][i]>1.0)
	x[k][i]=0.96;
      else
      if(x[k][i]<-1.0)
	x[k][i]=-0.96;
      }
    }
    for(k=0;k<nki;k++) xk[k]=x[k][i];
    Imit_Driv();
    for(k=0;k<nki;k++) y[k][i]=yk[k];
  }
//Плавный переход к началу внутри цикла
  if((cl+1)<(cycle))
    {
    for(k=0;k<nki;k++)
      del_x[k]=(x[k][nr-1]-x[k][n0])/(float)pp;
    for(i=0;i<=pp;i++)
      {
      for(k=0;k<nki;k++)
	xk[k]=(x[k][nr-1]+(float)i*del_x[k]);
      Imit_Driv();
      }
    }
}
//Плавный выход в конце эксперимента
 for(k=0;k<nki;k++)
   del_x[k]=x[k][nr-1]/(float)pp;
 for(i=0;i<=pp;i++)
   {
   for(k=0;k<nki;k++)
     xk[k]=(x[k][nr-1]-(float)i*del_x[k]);
   Imit_Driv();
   }
   for(k=0;k<nki;k++)
     xk[k]=0.0;
 Imit_Driv();
 Disable();
 graf_time();
 } else printf("\007\007\007");

 Wclose();
 return;

}
//**************************************
//     Вспомогательные функции
//**************************************

float Psi_k(int k,int nn)
{
int i,a;
float s;
;
return(a);
}

float Psi_kv(int k,int v)
{
int a;
;
return(a);
}

float Psi(void)
{
int a;
;
return(a);
}
