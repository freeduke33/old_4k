/*Программа <stat_har.c>
*Снятие статических характеристик
*       каналов
*/

#include <conio.h>
#include "pro_achh.h"
#include "dac_adc.h"

float be[KAN_MAX+2][KAN_MAX+2][KAN_MAX+2][KAN_MAX+2];

static int fi2[4][3]={
	      {-1,-1,+1},
	      {-1,+1,-1},
	      {+1,-1,-1},
	      {+1,+1,+1}
	      };
static int fi3[8][7]={
              {-1,-1,-1,+1,+1,+1,-1},
              {-1,-1,+1,+1,-1,-1,+1},
	      {-1,+1,-1,-1,+1,-1,+1},
              {-1,+1,+1,-1,-1,+1,-1},
              {+1,-1,-1,-1,-1,+1,+1},
	      {+1,-1,+1,-1,+1,-1,-1},
              {+1,+1,-1,+1,-1,-1,-1},
              {+1,+1,+1,+1,+1,+1,+1}
	      };
static int fi4[16][4]={
              {-1,-1,-1,-1},
              {-1,-1,-1,+1},
              {-1,-1,+1,-1},
              {-1,-1,+1,+1},
              {-1,+1,-1,-1},
              {-1,+1,-1,+1},
              {-1,+1,+1,-1},
              {-1,+1,+1,+1},
              {+1,-1,-1,-1},
              {+1,-1,-1,+1},
              {+1,-1,+1,-1},
              {+1,-1,+1,+1},
              {+1,+1,-1,-1},
              {+1,+1,-1,+1},
              {+1,+1,+1,-1},
              {+1,+1,+1,+1},
              };


float xmin=-1.0;
float xmax=1.0;
float ymin=-1.0;
float ymax=1.0;


/*****************/
void do_stat(void)
/*****************/
{
int i,ii,j,k,v,nq_4;
float del_x,mxmax,mxmin,mymax,mymin;

/*Imit_Driv();*/



del_x=(xmax-xmin)/((float)nq/2);
/* Вставить функцию разгрузки по каждому каналу
 * razgruzka(...);
*/

if(Enable()==DRV_OK)
  {
  xk[0]=xk[1]=xk[2]=xk[3]=0.1E-20;
  for(xh[0]=k=0;k<nki;k++)
    {
    xk[0]=xk[1]=xk[2]=xk[3]=0.1E-20;
    xk[k]=xh[0];
    Imit_Driv();
    yh[k][0][0]=yk[0];  yh[k][1][0]=yk[1];
    yh[k][2][0]=yk[2];  yh[k][3][0]=yk[3];
    for(i=0,ii=-1;i<=nq;i++)
      {
      if(i>=0&&i<=(nq/4))
        ++ii;
      else
      if((i>(nq/4))&&(i<=(3*nq/4)))
        --ii;
      else
      if((i>(3*nq/4))&&(i<=nq))
        ++ii;
      xh[i]=(float)ii*del_x;
      xk[k]=xh[i];
      for(v=0;v<=10;v++)
        Imit_Driv();
      for(j=0;j<nko;j++)  yh[k][j][i]=yk[j];
      }
    }

//  for(i=0;i<=nq;i++)
//    xh[i]=xh[i]*10.0-5.0;
//        ;
//  for(k=0;k<nki;k++)
//    {
//    for(i=1;i<=nq;i++)
//      {
//      for(j=0;j<nko;j++)
//        yh[k][j][i]=yh[k][j][i]*10.0-5.0;
//         ;
//      }
//    }

  Disable();
  }
  else printf("\007\007\007");
}

/*****************/
void wz_stat(void)
/*****************/
{
int i,j,k,v,m;
static float ai[KAN_MAX+1][WMAX+1];
static float beta[WMAX+1][KAN_MAX+1];
extern float be[KAN_MAX+2][KAN_MAX+2][KAN_MAX+2][KAN_MAX+2];
float su1,del_x;

del_x=(xmax-xmin)/(float)nq;

//Запустить функцию <Разгрузка>

Enable();
switch(nki)
  {
  case 2:
    ai[0][0]=xmin; ai[1][0]=xmin;
    ai[0][1]=xmin; ai[1][1]=xmax;
    ai[0][2]=xmax; ai[1][2]=xmin;
    ai[0][3]=xmax; ai[1][3]=xmax;

    ScrSave();

    for(m=0;m<=3;m++)
      {
      for(k=0;k<nki;k++)
	{
	xk[k]=ai[k][m];
	printf("xk[%d]=%6.3f ",k,xk[k]);
	}
	printf("\n");
      Imit_Driv();
      for(j=0;j<nko;j++)
	{
	yh[0][j][m]=yk[j];
	printf("yh[0][%d][%d]=%6.3f\n",j,m,yh[0][j][m]);
	}
      }
    getch();
    for(i=0;i<nko;i++)
      {
      for(k=0;k<=3;k++)
	{
	for(su1=m=0;m<4;m++)
	  su1+=(k==0)?yh[0][i][m]:fi2[m][k-1]*yh[0][i][m];
	beta[k][i]=su1/4.0;
	}
      }
//    ScrSave();
    clrscr();
    puts("Значения коэффициентов взаимосвязи для системы");
    printf("с числом входов %2d и с числом выходов %2d\n\n\n",nki,nko);
    for(i=0;i<nko;i++)
      {
      for(k=0;k<=3;k++)
	printf("beta[%1d][%1d]=%5.2f ",k,i,beta[k][i]);
      printf("\n");
      }
    puts("Нажмите любую клавишу");
    getch();
    ScrRest();

  break;
  case 3:

  break;
  case 4:
    m=0;
    for(i=0;i<nq/2;i++)
      {
      xk[0]=xk[1]=xk[2]=xk[3]=-(int)i*del_x;
      Imit_Driv();
//      per_pr();
      }
    i=0;
    xk[0]=xk[1]=xk[2]=xk[3]=xmin;
    per_pr();
    Out_kan(i,m);

    for(m=1;m<=15;m++)
      {
      shag(m);
      for(k=0;k<=3;k++)
          {
//        xk[k]=(fi4[m][k]==-1)?xmin:xmax;
          if(fabs(fi4[m][k]-fi4[m-1][k])>0.5)
            {
            if(fi4[m][k]==1)
              {
              for(i=0;i<nq;i++)
                {
                xk[k]=xmin+i*del_x;
                Imit_Driv();
//                per_pr();
                }
              xk[k]=xmax;
              Imit_Driv();
              per_pr();
              }
            else
              {
              for(i=0;i<nq;i++)
                {
                xk[k]=xmax-i*del_x;
                Imit_Driv();
//                per_pr();
                }
              xk[k]=xmin;
              Imit_Driv();
              }
            }
            else
              xk[k]=(fi4[m][k]==-1)?xmin:xmax;
          Imit_Driv();
          per_pr();
          Out_kan(0,m);
          }
      }
for(k=0;k<=3;k++)
  {
  if(fi4[15][k]==1)
    {
    for(i=0;i<nq/2;i++)
      {
      xk[k]=xmax-i*del_x;
      Imit_Driv();
      }
    xk[k]=0.0;
    Imit_Driv();
    }
  else
    {
    for(i=0;i<nq/2;i++)
      {
      xk[k]=xmin+i*del_x;
      Imit_Driv();
      }
    xk[k]=0.0;
    Imit_Driv();
    }
  }

// Вычисление величин <beta>
    for(v=0;v<=4;v++)
      {
      for(k=0;k<nko;k++)
        {
        for(su1=m=0;m<=15;m++)
          su1=(v==0)?su1+yh[0][k][m]:su1+yh[0][k][m]*(int)fi4[m][v-1];
        be[0][0][v][k]=su1/16.0;
        }
      }
    for(j=1;j<=3;j++)
      {  
      for(v=j+1;v<=4;v++)
        {
        for(k=0;k<nko;k++)
          {
          for(su1=m=0;m<=15;m++)
            su1+=((int)fi4[m][j-1])*((int)fi4[m][v-1])*yh[0][k][m];
          be[0][j][v][k]=su1/16.0;
          }
        }
      }
    for(i=1;i<=2;i++)
      {
      for(j=i+1;j<=3;j++)
        {  
        for(v=j+1;v<=4;v++)
          {
          for(k=0;k<nko;k++)
            {
            for(su1=m=0;m<=15;m++)
              su1+=((int)fi4[m][j-1])*((int)fi4[m][v-1])*((int)fi4[m][i-1])*yh[0][k][m];
            be[i][j][v][k]=su1/16.0;
            }
          }
        }
      }
  for(k=0;k<nko;k++)
    {
    for(su1=m=0;m<=15;m++)
      su1+=((int)fi4[m][0])*((int)fi4[m][1])*((int)fi4[m][2])*((int)fi4[m][3])*yh[0][k][m];
    be[3][3][3][k]=su1/16.0;
    }
  break;
  
  }
Disable();

}

// Функция вычисления АЧХ всех каналов системы
//****************
void do_achh(void)
//****************
{
extern float fn,fw,delt,fdisk,psi;

int i,j,k,v,kk;
float fi,psii,lam,amp,ga,cc,xt[3],e[3],ps[3];

psii=0.05;
//fw=1.0/(2.0*delt);
fdisk=1.0/delt;

if(!AllocBuf()) return;

ScrSave();
clrscr();

puts("Вычисление АЧХ");
if(Enable()==DRV_OK)
{
for(kk=0;kk<nki;kk++)
  {
  xk[0]=xk[1]=xk[2]=xk[3]=0.1E-20;
  for(k=0;k<=nq;k++)
    {
    fi=2.0*M_PI*delt*(fn+((float)k*(fw-fn)/((float)(nq))));
    xh[k]=fi/(2.0*M_PI*delt);
    printf("xh[%3d]=%8.3f\n",k,fi/(2.0*M_PI*delt));
    lam=2.0*cos(fi);
    xt[0]=amp*sin(psii);
    xk[kk]=xt[0];
    Imit_Driv();
    xt[1]=amp*sin(fi+psii);
    xk[kk]=xt[1];
    Imit_Driv();
    for(v=2;v<=200;v++)
      {
      xk[kk]=xt[v%3]=lam*xt[(v-1)%3]-xt[(v-2)%3];
      Imit_Driv();
      }
    xk[kk]=x[0][0]=xt[(v-3)%3];
    Imit_Driv();
    Out_y(0);
    xk[kk]=x[0][1]=xt[(v-2)%3];
    Imit_Driv();
    Out_y(1);
    xk[kk]=x[0][2]=xt[(v-1)%3];
    Imit_Driv();
    Out_y(2);
    for(v=3;v<=nr;v++)
      {
      x[0][v]=lam*x[0][v-1]-x[0][v-2];
      Imit_Driv();
      Out_y(v);
      }
    ga=sin((float)nr*fi);
    cc=2.0/(amp*(float)nr*(1.0-ga*ga));
    for(v=0;v<nr;v++)
      {
      ps[v%2]=y[kk][nr-1-v]-ga*y[kk][v];
      if(v==0)
        e[v%3]=ps[v%2];
      else 
      if(v==1)
        e[v%3]=lam*e[(v-1)%3]+ps[v%2];        
      else
        e[v%3]=lam*e[(v-1)%3]-e[(v-2)%3]+ps[v%2];
      }
    yh[kk][0][k]=cc*sqrt(e[(v-1)%3]*(ps[(v-2)%2]-e[(v-3)%3])+e[(v-2)%3]*e[(v-2)%3]);
    }
  }
    
getch();
ScrRest();
return;
 }
 else printf("\007\007\007");
}





