//Программа функций вывода в статическом режиме
//              <OUT_STAT.c>
#pragma warn -rpt
#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tcxlwin.h"
#include "pro_achh.h"
#include "look_def.h"


static char sss[80*24*2];

void ScrSave(void) { gettext(1,1,80,25,sss);clrscr(); }
void ScrRest(void) { puttext(1,1,80,25,sss); }


//*********************************************
void Get(float *x,float *y,triadGRF *p,int num)
{
 *x=5.0*xh[num];
 *y=5.0*yh[(int)p->Array1][(int)p->Array2][num];
 return;
}
//*********************************************
static triadGRF param[16]={
{BOX_4_4,  -1,-1,1,1,  Get,NULL,      0,0, 1,0,    0,100,
 "1","Активн.1 вх.","%+6.3f/%+6.3f","%4g",grfPARAM_ALL|grfDOUBLE|grfUNMARK,grfRANGE},
{{0},	   -1,-1,1,1,	  Get,NULL,    0,1, 1,0,    0,200,
 NULL,NULL,NULL,NULL,grfDOUBLE},
{{0},	   -1,-1,1,1,	  Get,NULL,    0,2, 1,0,    0,200,
 NULL,NULL,NULL,NULL,grfDOUBLE},
{{0},	   -1,-1,1,1,	  Get,NULL,    0,3, 1,0,    0,200,
 NULL,NULL,NULL,NULL},

{BOX_3_4,  -1,-1,1,1,  Get,NULL,      1,1, 1,0,    0,100,
 "2","Активн.2 вх.","%+6.3f/%+6.3f","%4g",grfPARAM_ALL|grfDOUBLE|grfUNMARK,grfRANGE},
{{0},	   -1,-1,1,1,	  Get,NULL,    1,2, 1,0,    0,200,
 NULL,NULL,NULL,NULL,grfDOUBLE},
{{0},	   -1,-1,1,1,	  Get,NULL,    1,3, 1,0,    0,200,
 NULL,NULL,NULL,NULL,grfDOUBLE},
{{0},	   -1,-1,1,1,	  Get,NULL,    1,0, 1,0,    0,200,
 NULL,NULL,NULL,NULL},

{BOX_2_4,  -1,-1,1,1,  Get,NULL,      2,2, 1,0,    0,100,
 "3","Активн.3 вх.","%+6.3f/%+6.3f","%4g",grfPARAM_ALL|grfDOUBLE|grfUNMARK,grfRANGE},
{{0},	   -1,-1,1,1,	  Get,NULL,    2,3, 1,0,    0,200,
 NULL,NULL,NULL,NULL,grfDOUBLE},
{{0},	   -1,-1,1,1,	  Get,NULL,    2,0, 1,0,    0,200,
 NULL,NULL,NULL,NULL,grfDOUBLE},
{{0},	   -1,-1,1,1,	  Get,NULL,    2,1, 1,0,    0,200,
 NULL,NULL,NULL,NULL},

{BOX_1_4,  -1,-1,1,1,  Get,NULL,      3,3, 1,0,    0,100,
 "4","Активн.4 вх.","%+6.3f/%+6.3f","%4g",grfPARAM_ALL|grfDOUBLE|grfUNMARK,grfRANGE},
{{0},	   -1,-1,1,1,	  Get,NULL,    3,0, 1,0,    0,200,
 NULL,NULL,NULL,NULL,grfDOUBLE},
{{0},	   -1,-1,1,1,	  Get,NULL,    3,1, 1,0,    0,200,
 NULL,NULL,NULL,NULL,grfDOUBLE},
{{0},	   -1,-1,1,1,	  Get,NULL,    3,2, 1,0,    0,200,
 NULL,NULL,NULL,NULL}};


//****************************************************
//Вывод значений статических характеристик

void tabl_stat(void)
{
int i,k,j;

ScrSave();
for(k=0;k<nki;k++)
  {
  printf("\n Начало таблицы для вх.канала k=%d\n",k);
  if(k>0)
    getch();
  for(i=0;i<=nq;i++)
      {
      printf("k=%d x[%2d]=%5.3f ",k,i,xh[i]*5.0);
      for(j=0;j<nko;j++)
        {
        printf("%5.2f ",yh[k][j][i]*5.0);
        if(nko==1)
          putchar('\n');
        else
          {
          if((j%(nko-1)==0)&&(j!=0))
            putchar('\n');
          }
        }
      }
   }
 printf("\n---- END ----");
 getch();
 ScrRest();
 return;
}
//**** Вывод в виде таблицы значений выходных сигналов
void tab_y_wz(void)
{
int j,m;
ScrSave();
clrscr();
puts("     Значения выходных сигналов системы\n");
puts("Ном.экс.    1 кан   2 кан.  3 кан.  4кан.");
puts("─────────────────────────────────────────");
for(m=0;m<=15;m++)
  {
  gotoxy(2,5+m);
  cprintf("m=%2d  ",m);
  for(j=0;j<nko;j++)
    {
    gotoxy(12+8*j,5+m);
    cprintf("%5.2f  ",yh[0][j][m]*5.0);
    }
  }
puts("\n─────────────────────────────────────────");
puts("\n\nНажмите любую клавишу");
getch();
ScrRest();
return;
}
//**** Вывод значений коэффициентов взаимосвязи <beta> **
void tab_beta(void)
{
int i,j,k,v,kk;
extern float be[KAN_MAX+2][KAN_MAX+2][KAN_MAX+2][KAN_MAX+2];

ScrSave();
clrscr();
puts("  Значения коэффициентов взаимосвязи <beta>\n");
puts("Коэффиц.        1 кан.      2 кан.      3 кан.      4кан.");
puts("─────────────────────────────────────────────────────────");
gotoxy(1,5);
cprintf("beta(0)");
gotoxy(1,5+1);
cprintf("beta(1)");
gotoxy(1,5+2);
cprintf("beta(2)");
gotoxy(1,5+3);
cprintf("beta(3)");
gotoxy(1,5+4);
cprintf("beta(4)");
gotoxy(1,5+5);
cprintf("beta(1,2)");
gotoxy(1,5+6);
cprintf("beta(1,3)");
gotoxy(1,5+7);
cprintf("beta(1,4)");
gotoxy(1,5+8);
cprintf("beta(2,3)");
gotoxy(1,5+9);
cprintf("beta(2,4)");
gotoxy(1,5+10);
cprintf("beta(3,4)");
gotoxy(1,5+11);
cprintf("beta(1,2,3)");
gotoxy(1,5+12);
cprintf("beta(1,2,4)");
gotoxy(1,5+13);
cprintf("beta(1,3,4)");
gotoxy(1,5+14);
cprintf("beta(2,3,4)");
gotoxy(1,5+15);
cprintf("beta(1,2,3,4)");

for(v=0;v<=4;v++)
  {
  for(k=0;k<nko;k++)
    {
    gotoxy(15+12*k,5+v);
    cprintf("%7.4f  ",be[0][0][v][k]);
    }
  }
kk=10;
for(j=1;j<=3;j++)
  {
  for(v=j+1;v<=4;v++)
    {
    for(k=0;k<nko;k++)
      {
      gotoxy(15+12*k,kk);
      cprintf("%7.4f  ",be[0][j][v][k]);
      }
    kk++;
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
            gotoxy(15+12*k,kk);
            cprintf("%7.4f  ",be[i][j][v][k]);
            }
          kk++;
          }
        }
      }
for(k=0;k<nko;k++)
  {
  gotoxy(15+12*k,kk);
  cprintf("%7.4f  ",be[3][3][3][k]);
  }

gotoxy(1,5+16);
puts("─────────────────────────────────────────────────────────");
puts("\nНажмите любую клавишу");
getch();
ScrRest();
return;
}
//*******************************************************

void graf_stat() { graf4(-1,1,-1,1);return; }

void graf4(float Xmin,float Xmax,float Ymin,float Ymax)
{
 int i;

/*for(i=0;i<16;i++) { param[i].beg=0; param[i].end=WMAX; }*/
 for(i=0;i<16;i++)
   {
   param[i].beg=0;
   param[i].end=nq;
   param[i].Xmax=Xmax*5.0;
   param[i].Xmin=Xmin*5.0;
   param[i].Ymax=Ymax*5.0;
   param[i].Ymin=Ymin*5.0;
   }

 ScrSave();
 InitGRF(grfGRAPHICS);
 LookGRF(16,param);
 DeinitGRF(grfTEXT);
 ScrRest();

/* printf("num=%d\n",num);*/
/* getch();*/

 return;
}
/*****************************************************************************/


void tGetX(float *xx,float *yy,triadGRF *p,int num)
{
 *xx=num*delt;
 *yy=5.0*x[(int)p->Array2][num];
 return;
}

void tGetY(float *xx,float *yy,triadGRF *p,int num)
{
 *xx=num*delt;
 *yy=5.0*y[(int)p->Array2][num];
 return;
}



static triadGRF intime[8]={
{HOR_2_2,  -1,-1,1,1,     tGetX,NULL,      0,0, 1,0,    0,100,
 "Входы","t","f[%5.2f]=%+6.3f,%+6.3f,%+6.3f,%+6.3f","mark=%4g",grfPARAM_ALL|grfDOUBLE,grfRANGE},
{{0},	   -1,-1,1,1,	  tGetX,NULL,    0,1, 1,0,    0,200,
 NULL,NULL,NULL,NULL,grfDOUBLE},
{{0},	   -1,-1,1,1,	  tGetX,NULL,    0,2, 1,0,    0,200,
 NULL,NULL,NULL,NULL,grfDOUBLE},
{{0},	   -1,-1,1,1,	  tGetX,NULL,    0,3, 1,0,    0,200,
 NULL,NULL,NULL,NULL},

{HOR_1_2,  -1,-1,1,1,     tGetY,NULL,      1,0, 1,0,    0,100,
 "Выходы","t","f[%5.2f]=%+6.3f,%+6.3f,%+6.3f,%+6.3f","mark=%4g",grfPARAM_ALL|grfDOUBLE,grfRANGE},
{{0},	   -1,-1,1,1,	  tGetY,NULL,    1,1, 1,0,    0,200,
 NULL,NULL,NULL,NULL,grfDOUBLE},
{{0},	   -1,-1,1,1,	  tGetY,NULL,    1,2, 1,0,    0,200,
 NULL,NULL,NULL,NULL,grfDOUBLE},
{{0},	   -1,-1,1,1,	  tGetY,NULL,    1,3, 1,0,    0,200,
 NULL,NULL,NULL,NULL}
};


void graf_time(void)
{
 int i;

 if(!y[0]) { Wperror("Нет данных для показа");return; }

 for(i=0;i<8;i++)
    {
    intime[i].beg=0;  intime[i].end=nr;
    intime[i].Xmin=0; intime[i].Xmax=nr*delt;
    intime[i].Ymax=5; intime[i].Ymin=-5;
    }

 ScrSave();
 InitGRF(grfGRAPHICS);
 LookGRF(8,intime);
 DeinitGRF(grfTEXT);
 ScrRest();
 return;
}

/* ==================================================================== */

void Get1(float *x,float *y,triadGRF *p,int num)
{
 *x=(unsigned)num*delt;
 *y=5.0*p->Array1[(unsigned)num];
 return;
}

//*********************************************
static triadGRF in2time[8]={
{BOX_4_4,  0,-5,1,5,  Get1,NULL,   0,0, 1,0,    0,100,
 "Канал N1","[t]","f[%5.2f]=%+6.3f(%+6.3f)","(%5.2f)",grfDOUBLE,grfRANGE},
{{0},	   0,-5,1,5,  Get1,NULL,    0,3, 1,0,    0,200,
 NULL,NULL,NULL,NULL},

{BOX_3_4,  0,-5,1,5,  Get1,NULL,   1,1, 1,0,    0,100,
 "Канал N2","[t]","f[%5.2f]=%+6.3f(%+6.3f)","(%5.2f)",grfDOUBLE,grfRANGE},
{{0},	   0,-5,1,5,  Get1,NULL,    1,0, 1,0,    0,200,
 NULL,NULL,NULL,NULL},

{BOX_2_4,  0,-5,1,5,  Get1,NULL,   2,2, 1,0,    0,100,
 "Канал N3","[t]","f[%5.2f]=%+6.3f(%+6.3f)","(%5.2f)",grfDOUBLE,grfRANGE},
{{0},	   0,-5,1,5,  Get1,NULL,    2,1, 1,0,    0,200,
 NULL,NULL,NULL,NULL},

{BOX_1_4,  0,-5,1,5,  Get1,NULL,   3,3, 1,0,    0,100,
 "Канал N4","[t]","f[%5.2f]=%+6.3f(%+6.3f)","(%5.2f)",grfDOUBLE,grfRANGE},
{{0},	   0,-5,1,5,  Get1,NULL,    3,2, 1,0,    0,200,
 NULL,NULL,NULL,NULL}};



void graf_2time(void)
{
 int i;

 if(!y[0]) { Wperror("Нет данных для показа");return; }

 for(i=0;i<8;i++)
   {
   in2time[i].beg=0;
   in2time[i].end=nr;
   in2time[i].Xmax=nr*delt; in2time[i].Xmin=0;
   in2time[i].Ymax=5;       in2time[i].Ymin=-5;
   in2time[i].Array1= (i%2)==0 ? y[i/2] : yt[i/2];
   }

 ScrSave();
 InitGRF(grfGRAPHICS);
 LookGRF(8,in2time);
 DeinitGRF(grfTEXT);
 ScrRest();
 return;
}

/* ==================================================================== */

void GetE(float *x,float *y,triadGRF *p,int num)
{
 *x=(unsigned)num*delt;
 *y=5.0*p->Array1[(unsigned)num];
 return;
}

//*********************************************
static triadGRF et_time[8]={
{BOX_4_4,  0,-5,1,5,  GetE,NULL,   0,0, 1,0,    0,100,
 "Канал N1","[t]","f[%5.2f]=%+6.3f","(%5.2f)",grfPARAM_ALL,grfRANGE},

{BOX_3_4,  0,-5,1,5,  GetE,NULL,   1,1, 1,0,    0,100,
 "Канал N2","[t]","f[%5.2f]=%+6.3f","(%5.2f)",grfPARAM_ALL,grfRANGE},

{BOX_2_4,  0,-5,1,5,  GetE,NULL,   2,2, 1,0,    0,100,
 "Канал N3","[t]","f[%5.2f]=%+6.3f","(%5.2f)",grfPARAM_ALL,grfRANGE},

{BOX_1_4,  0,-5,1,5,  GetE,NULL,   3,3, 1,0,    0,100,
 "Канал N4","[t]","f[%5.2f]=%+6.3f","(%5.2f)",grfPARAM_ALL,grfRANGE}};



void vi_et_graph(void)
{
 int i;

 if(!AllocBuf() || !etalon()) return;

 for(i=0;i<4;i++)
   {
   et_time[i].beg=0;
   et_time[i].end=nr;
   et_time[i].Xmax=nr*delt; et_time[i].Xmin=0;
   et_time[i].Ymax=5;       et_time[i].Ymin=-5;
   et_time[i].Array1=yt[i];
   }

 ScrSave();
 InitGRF(grfGRAPHICS);
 LookGRF(4,et_time);
 DeinitGRF(grfTEXT);
 ScrRest();
 return;
}

void vi_et_tabl(void)
{
 ScrSave();
 //
 ScrRest();
 return;
}


void  tabl_achh(void)
{
 ScrSave();
 //
 ScrRest();
 return;
}

void tabl_time(void)
{
 ScrSave();
 //
 ScrRest();
 return;
}
