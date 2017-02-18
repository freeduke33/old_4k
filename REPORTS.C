#include <stdio.h>

#include "tcxlwin.h"
#include "pro_achh.h"


void CTYP Inf_file(NOARG);


void write_upr(void)
{
 FILE *f;
 unsigned long i;
 int k;

 if(x[0]==NULL || y[0]==NULL || yt[0]==NULL)
   {
   Wperror("Нет данных для записи в файл");
   return;
   }

 Inf_file();

 f=fopen(ufile_name,"w+t");
 if(f)
   {
   fprintf(f,"+++ Заголовок_: \"ПРОТОКОЛ_УПРАВЛЕНИЯ_СТЕНДОМ\"\n"
             "+++ Длинна_ряда_: %lu\n"
             "+++ Частота_дискретизации_: %8.3f\n"
             "+++ Число_каналов_: %d x %d\n",nr,1.0/delt,nko,nki);
   for(k=0;k<nko;k++)
     {
     fprintf(f,"\n+++ Канал_N %d\n",k+1);
     for(i=0;i<nr;i++)
       fprintf(f,"%-6lu %+7.3f %+7.3f %+7.3f\n",i,x[k][i]*5.0,y[k][i]*5.0,yt[k][i]*5.0);
     }
   fclose(f);
   }
 else Wperror(" Не могу открыть файл для записи протокола управления ");

 Wclose();

 return;
}

void write_et(void)
{
 FILE *f;
 unsigned long i;
 int k;

 if(yt[0]==NULL)
   {
   Wperror("Нет данных для записи в файл");
   return;
   }

 Inf_file();

 f=fopen(ifile_name,"w+t");
 if(f)
   {
   fprintf(f,"%d %8.3f %lu\n",nko,1.0/delt,nr);
   for(i=0;i<nr;i++)
     {
     for(k=0;k<nko;k++) fprintf(f,"%6.2f ",yt[k][i]*5.0*ifile_k+ifile_st);
     fprintf(f,"\n");
     }
   fclose(f);
   }
 else Wperror(" Не могу открыть файл для записи эталонного сигнала ");

 Wclose();

 return;
}

void write_wzs(void)
{
 FILE *f;
 int k,i;

 Inf_file();

 f=fopen(wfile_name,"w+t");
 if(f)
   {
   fprintf(f,"+++ Заголовок_: \"ВЗАИМОДЕЙСТВИЕ_В_СТАТИКЕ\"\n"
             "+++ Частота_дискретизации_: %8.3f\n"
             "+++ Число_каналов_: %d x %d\n",1.0/delt,nko,nki);

   fprintf(f,"\n+++ Значения_выходных_сигналов_системы\n");
   for(i=0;i<15;i++)
     {
     fprintf(f,"%d ",i);
     for(k=0;k<nko;k++) fprintf(f,"%6.2f ",yh[0][k][i]*5.0);
     fprintf(f,"\n");
     }

   fprintf(f,"\n+++ Значения_коэффициентов_взаимосвязи\n");
   for(i=0;i<4;i++)
     {
     for(k=0;k<nko;k++) fprintf(f,"%7.4f  ",be[0][0][k][i]);
     fprintf(f,"\n");
     }

   fclose(f);
   }
 else Wperror(" Не могу открыть файл для записи взаимодействия в статике ");

 Wclose();

 return;
}

void write_st(void)
{
 FILE *f;
 unsigned long i;
 int k,j;

 Inf_file();

 f=fopen(sfile_name,"w+t");
 if(f)
   {
   fprintf(f,"+++ Заголовок_: \"СТАТИЧЕСКИЕ_ХАРАКТЕРИСТИКИ\"\n"
             "+++ Частота_дискретизации_: %8.3f\n"
             "+++ Число_каналов_: %d x %d\n",1.0/delt,nko,nki);
   for(j=0;j<nki;j++)
     {
     fprintf(f,"\n+++ Управляемый_канал_: %d\n",j+1);
     for(i=0;i<nq;i++)
       {
       fprintf(f,"%-6lu %6.2f ",i,xh[i]*5.0);
       for(k=0;k<nko;k++) fprintf(f,"%6.2f ",yh[j][k][i]*5.0);
       fprintf(f,"\n");
       }
     }
   fclose(f);
   }
 else Wperror(" Не могу открыть файл для записи статических характеристик ");

 Wclose();

 return;
}



