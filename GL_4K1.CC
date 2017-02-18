#pragma option -N-


/*Главная программа для 4-х канальной адаптивной системы*/
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>

#include "tcxlwin.h"

#include "dac_adc.h"
#include "pro_achh.h"


static float psx[CHANNEL][PSMAX],psy[CHANNEL][PSMAX];
static float _psx[PSMAX],_psy[PSMAX];
static float lam[CHANNEL][PMAX],c[CHANNEL][PMAX];
static float _lam[PMAX],_c[PMAX];
static float kps[ZAPAZ][PMAX];

static short _flg[CHANNEL];
static unsigned long tik;


int preCalc(void)
{
 kps[0][0]=1;
 kps[1][0]=(q+2);
 kps[1][1]=-(q+1);
 kps[2][0]=(float)((q+2)*(q+3))/2.0;
 kps[2][1]=-(q+1)*(q+3);
 kps[2][2]=(float)((q+1)*(q+2))/2.0;
 fdisk=1.0/delt;
 return 0;
}


/*- - - - - - Блок вычисления управляющего сигнала- - - -*/
float huge Sign_upr(short k,unsigned long m)
{
float pr1,bet;
//int i,m_0=m%(qq+p),m_1=(m-1)%(qq+p),m_2=(m-2)%(qq+p);
int i,m_0=m%(10),m_1=(m-1)%(10),m_2=(m-2)%(10);

// for(i=0;i<1007;i++) bet=2.0*sin(cos(3.14/4.0));

switch(n1)
  {
  case 0:
    psy[k][m_0]=y[k][m-1]; psx[k][m_0]=x[k][m-1-q];
  break;
  case 1:
    psy[k][m_0]=kps[1][0]*y[k][m-1]+kps[1][1]*y[k][m-2];
    psx[k][m_0]=kps[1][0]*x[k][m-1-q]+kps[1][1]*x[k][m-2-q];
  break;
  case 2:
    psy[k][m_0]=kps[2][0]*y[k][m-1]+kps[2][1]*y[k][m-2]+kps[2][2]*y[k][m-3];
    psx[k][m_0]=kps[2][0]*x[k][m-1-q]+kps[2][1]*x[k][m-2-q]+kps[2][2]*x[k][m-3-q];
  break;
  }
switch(p)
  {
  case 0:
    x[k][m]=psx[k][m_0]+(yt[k][m+q]-psy[k][m_0])/c[k][0];
  break;
  case 1:
    x[k][m]=psx[k][m_0]+(yt[k][m+q]-psy[k][m_0]-lam[k][p]*(yt[k][m+q-1]-psy[k][m_1]))/c[k][0];
  break;
  case 2:
    if (q>=2) pr1=yt[k][m+q-2];
    else      pr1=y[k][m+q-2];
    x[k][m]=psx[k][m_0]+(yt[k][m+q]-psy[k][m_0]-lam[k][1]*(yt[k][m+q-1]-psy[k][m_1])-lam[k][2]*(pr1-psy[k][m_2]))/c[k][0];
  break;
  }

return(x[k][m]);
}
/*- - - - - - - - - - Вычисление параметров модели - - - - - - - -*/

void par_mod(short k,unsigned long n)
{
int qqq=qq+p;
int i;
float s1,s2,s3,s4,s5,pr1,pr2,pr3,pr4,bet;

//for(i=0;i<1007;i++) bet=2.0*sin(cos(3.14/4.0));

switch(p)
  {
  case 0:
    for(i=n;i<=n+qq-1;i++)
      {
      pr1=x[k][i]-_psx[i%10];
      s1+=(y[k][i+q]-_psy[i%10])*pr1;
      s2+=pr1*pr1;
      }
    _c[0]=s1/s2;
    break;
  case 1:
    for(i=n;i<=n+qq-1;i++)
      {
      pr1=x[k][i]-_psx[i%10];
      pr2=y[k][i+q]-_psy[i%10];
      pr3=y[k][i+q-1]-_psy[(i-1)%10];

      s1+=pr1*pr1;    s2+=pr2*pr1;
      s3+=pr3*pr1;    s4+=pr3*pr2;
      s5+=pr3*pr3;
      }
    bet=s3/s1;
    _lam[1]=(s4-bet*s2)/(s5-bet*s3);
    _c[0]=s1/s2;
    break;
  }
}


/*- - - - - - Блок вычисления сигнала для переходного процесса - - - -*/
float huge Sign_per(short k,unsigned long m)
{
 static const float s[4]={0.02,0.03,0.04,0.05};

 x[k][m]= m%2 ? s[k] : -s[k];
 return x[k][m];
}

/* ================================================================== */


void interrupt Hook(void)
{
 float buf[CHANNEL];
 register short i;

 DrvGetVolADC(buf);

 for(i=0;i<CHANNEL;i++)
    {
    y[i][tik]=buf[i];  // отклик системы
    if(_flg[i]) buf[i]=Sign_upr(i,tik); // управление по модели
    else        buf[i]=Sign_per(i,tik); // переходный процесс без модели
    }
 tik++;


 DrvSetVolDAC(buf);  // управляющее воздействие
}

void Loop(void)
// Главный цикл управления в асинхронном режиме
{
 unsigned long i,cl;
 short k;
 static unsigned long skip;
 char buf[40];


 for(k=0;k<CHANNEL;k++) _flg[k]=0;
 tik=0; k=0;

 preCalc();
 if(!AllocBuf() || !etalon()) return;

 Inf_Rab(); // индикация работы со стендом

 DrvSetHook(Hook);
 if(Enable()==DRV_OK)
   {
   for(cl=0;cl<cycle;cl++)
     {
     while(tik<nr)
	{
	printf("%d,%d\t",tik,_flg);
	if(tik>pp)
	  {
	  DrvWaitDAC_ADC();
	  for(i=0;i<q;i++)
	     {
	     _psx[i]=psx[k][(tik-q+i)%q];
	     _psy[i]=psy[k][(tik-q+i)%q];
	     }
	  par_mod(k,tik-qq);
	  DrvWaitDAC_ADC();
	  for(i=0;i<PMAX;i++) { lam[k][i]=_lam[i]; c[k][i]=_c[i]; }
	  _flg[k]=1; k=(k+1)%CHANNEL;
	  }
	}
     tik=0;
     } /* for(cycle) */
   Disable();
   DrvGetSkipped(&skip);
   if(skip)
     {
     sprintf(buf," Пропущено %lu отсчетов ",skip);
     Wperror(buf);
     }
   graf_2time();
   }
 else Wperror("Error enable UPI\n");

 Wclose();
 return;
}



void Loop_debug(void)
// Главный цикл управления в синхронном режиме
{
 unsigned long i,cl;
 short k;
 static unsigned long skip;
 char buf[40];


 for(k=0;k<CHANNEL;k++) _flg[k]=0;
 tik=0; k=0;

 preCalc();
 if(!AllocBuf() || !etalon()) return;

 Inf_Rab(); // индикация работы со стендом

 if(Enable()==DRV_OK)
   {
   for(cl=0;cl<cycle;cl++)
     {
     while(tik<nr)
	{
	printf("\n %g %g %g %g <--- %g %g %g %g",yk[0],yk[1],yk[2],yk[3],xk[0],xk[1],xk[2],xk[3]);
        for(k=0;k<CHANNEL;k++)
          {
          if(tik>pp)
            {
            // готовим пси для вычисления модели
	    for(i=0;i<q;i++)
	        {
	        _psx[i]=psx[k][(tik-q+i)%10];
	        _psy[i]=psy[k][(tik-q+i)%10];
	        }
            // вычисляем модель
	    par_mod(k,tik-qq);
            // передаем параметры для формирования упр.воздействия
	    for(i=0;i<PMAX;i++) { lam[k][i]=_lam[i]; c[k][i]=_c[i]; }
            // формируем упр. сигнал
            xk[k]=Sign_upr(k,tik);
            }
          else xk[k]=Sign_per(k,tik); // переходный процесс без модели
          }
        tik++;
        Imit_Driv();
	}
     tik=0;
     } /* for(cycle) */
   Disable();
   DrvGetSkipped(&skip);
   if(skip)
     {
     sprintf(buf," Пропущено %lu отсчетов ",skip);
     Wperror(buf);
     }
   graf_2time();
   }
 else Wperror("Error enable UPI\n");

 Wclose();
 return;
}



