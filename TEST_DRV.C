/*  */
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>
/* #include <sys\\stat.h> */


#include "DAC_ADC.h"

/***********************************************************************/

struct
  {
  float Freq,SinFreq,MeandrFreq;
  Word  Act,NumDAC,NumADC,Sig;
  float LineAmpl,SinAmpl,ConstVol,MeandrAmpl;
  } Cfg={10.0,0.6,0.1,  1,1,1,0, 1,1,1,1};

float SinArg;
char SigMsg[][10]={"УРОВЕНЬ","СИНУС","МЕАНДР","ПИЛА"};
volatile float BufDAC[50],BufADC[50];
Word tik,MaxVol,NumMAX;


/***********************************************************************/

int ExitDrvError(int val)
{
 char msg[180];

 DrvGetErrorMsg(msg);
 printf("Fatal DAC/ADC driver error: %s\n",msg);
 DrvDeinitialize();
 exit(val);
 return 0;
}

int DrvError(char *fmt)
{
 char msg[180];

 DrvGetErrorMsg(msg);
 printf(fmt,msg);
 return 0;
}

int ClrScr(void)
{
 char str[80];

  clrscr();
  DrvGetName(str);
  printf("Название УСО : %s\n",str);
  DrvGetCopyright(str);
  printf("Авторское право : %s\n",str);

  return 0;
}

int ShowBuf(int mode)
{
 int i,x,y;

 x=wherex();
 y=wherey();

 cprintf("\n\r           --ЦАП--   --АЦП--\n\r");
 for(i=0;i<NumMAX;i++)
   {
   cprintf("Канал %02d:  ",i+1);
   if(i<Cfg.NumDAC) cprintf("%7.4f   ",BufDAC[i]);
   else         cprintf("%7s   ","");
   if(i<Cfg.NumADC) cprintf("%7.4f   \n\r",BufADC[i]);
   else         cprintf("%7s   \n\r","");
   }

 if(mode) gotoxy(x,y);
 return 0;
}
/***********************************************************************/

int CalcSignal(void)
{
 int i;
 float tmp,vol;

 tik++;
 switch(Cfg.Sig)
   {
   case 0:  /* Const */  tmp=Cfg.ConstVol;                         break;
   case 1:  /* Sin   */  tmp=Cfg.SinAmpl*sin(SinArg*tik/Cfg.Freq); break;
   case 2:  /* Meandr*/  tmp= tik%2 ? -Cfg.MeandrAmpl/2 : Cfg.MeandrAmpl/2; break;
   case 3:  /* Line  */  tmp=tik%MaxVol;                           break;
   }
 vol=Cfg.Act ? 0 : tmp;
 for(i=0;i<Cfg.NumDAC;i++) BufDAC[i]=vol;
 if(Cfg.Act) BufDAC[Cfg.Act-1]=tmp;
 return 0;
}

/***********************************************************************/

int TestA(void)
{
 unsigned long num,i;
 int step,val;

 ClrScr();
 printf("\n ******* Прямое управление УСО *******"
        "\n Введите число точек (0 если неограниченно) :");
 scanf("%ld",&num);
 step = num ? 1 : 0;
 num-=step;
 printf("\nТестирование ...");
 val=DrvEnable(Cfg.Freq);
 if(val==DRV_OK)
   {
   tik=0;
   for(i=0;i<=num;i+=step)
      {
      CalcSignal();
      DrvVolume(BufDAC,BufADC);
      ShowBuf(1);
      if(kbhit() && getch()==27) { printf(" *** Прерванно *** ");break; }
      }
   DrvDisable();
   ShowBuf(0);
   printf("Все\n"
          "\n --- Нажмите любую клавишу --- \n");
   }

 while(kbhit()) getch();
 getch();
 return 0;
}

void interrupt HookFunc(void)
{
 DrvGetVolADC(BufADC);
 ShowBuf(1);
 CalcSignal();
 DrvSetVolDAC(BufDAC);

 return;
}

int TestB(void)
{
 unsigned long num,step,i,val;

 ClrScr();
 printf("\n **** Управление УСО в фоновом режиме ****"
        "\n Введите число точек (0 если неограниченно) :");
 scanf("%ld",&num);

 /* ---------------------[ step 1 ]-----------------*/
 step = num ? 1 : 0;
 num-=step;

 DrvSetHook(HookFunc);
 /* ---------------------[ step 2 ]-----------------*/
 printf("\nТестирование ...\n");
 val=DrvEnable(Cfg.Freq);
 if(val==DRV_OK)
   {
   tik=0;
   for(i=0;i<=num;i+=step)
     {
/*     DrvWaitDAC_ADC();
     ShowBuf(1); */
     if(kbhit() && getch()==27) { printf(" *** User Break *** ");break; }
     }
   /* ---------------------[ step 3 ]-----------------*/
   DrvDisable();
   ShowBuf(0);
   }
 else DrvError("\007\nОшибка инициализации УСО :%s\n");

 DrvClrHook();
 printf("Все\n"
        "\n --- Нажмите любую клавишу --- \n");
 while(kbhit()) getch();
 getch();
 return 0;
}

int Signal(void)
{
 char val;

 ClrScr();
 printf("\n   Активный тестовый сигнал : %s\n"
        "\n    Выберите новый сигнал для проверки УСО:"
        "\n      1) Постоянный уровень"
        "\n              уровень=%g"
        "\n      2) Синусоида"
        "\n              частота=%g"
        "\n              амплитуда=%g"
        "\n      3) Меандр"
        "\n              частота=%g"
        "\n              амплитуда=%g"
        "\n      4) Пила"
        "\n              амплитуда=%g"
        "\n      0) Изменение параметров активного сигнала\n"
        "\n   Ваш выбор> ",SigMsg[Cfg.Sig],
        Cfg.ConstVol,Cfg.SinFreq,Cfg.SinAmpl,
        Cfg.MeandrFreq,Cfg.MeandrAmpl,Cfg.LineAmpl);
 val=getche()-'0';
 if(val>=0 && val<5)
    {
    if(val!=0) Cfg.Sig=val-1;
    else  switch(Cfg.Sig)
       {
       case 0: /*CONST*/
           printf("  Введите уровень постоянного сигнала :");
           scanf("%f",&Cfg.ConstVol);
           break;
       case 1: /*SIN*/
           printf("  Введите частоту и амплитуду синусоиды: ");
           scanf("%f %f",&Cfg.SinFreq,&Cfg.SinAmpl);
           break;
       case 2: /*MEANDR*/
           printf("  Введите частоту и амплитуду меандра: ");
           scanf("%f %f",&Cfg.MeandrFreq,&Cfg.MeandrAmpl);
           break;
       case 3: /*LINE*/
           printf(" Введите амплитуду :");
           scanf("%f",&Cfg.LineAmpl);
       }
    }
 else putch(7);


 return 0;
}

int Channel(void)
{
 printf("\n Введите новое число каналов ЦАП : ");
 scanf("%d",&Cfg.NumDAC);
 printf(" Введите новое число каналов АЦП : ");
 scanf("%d",&Cfg.NumADC);

 NumMAX= Cfg.NumDAC>Cfg.NumADC ? Cfg.NumDAC : Cfg.NumADC;
 if(NumMAX>15) NumMAX=15;
 return 0;
}

int SetFreq(void)
{
 printf("\n Введите новую частоту управления (>0): ");
 scanf("%f",&Cfg.Freq);
 return 0;
}

int ActChannel(void)
{
 printf("\n Введите номер управляемого канала АЦП (0 - все):");
 scanf("%u",&Cfg.Act);
 if(Cfg.Act>Cfg.NumDAC) Cfg.Act=1;
 return 0;
}

/***********************************************************************/

int main()
{
 int val;
 Word tmpDAC,tmpADC;

 printf("\nTesting DAC/ADC written by D.Kokorev (C) Samara 1993.\n");

 val=open("test_drv.ini",O_RDONLY|O_BINARY);
 if(val>0)
   {
   read(val,&Cfg,sizeof(Cfg));
   close(val);
   }

 if(DrvIsPresent())
   {
   val=DrvInitialize();
   if(val!=DRV_OK) ExitDrvError(val);
   DrvGetMaxChannel(&tmpDAC,&tmpADC);
   Cfg.NumDAC= Cfg.NumDAC>tmpDAC ? tmpDAC : Cfg.NumDAC;
   Cfg.NumADC= Cfg.NumADC>tmpADC ? tmpADC : Cfg.NumADC;
   NumMAX= Cfg.NumDAC>Cfg.NumADC ? Cfg.NumDAC : Cfg.NumADC;
   if(NumMAX>15) NumMAX=15;
   DrvGetInfoDAC(&MaxVol);

   val=1;
   while(val)
      {
      SinArg=(2.0*3.1416)*Cfg.SinFreq;
      ClrScr();
      printf("\n\n  --- Главное меню --- \n"
          "\n1) Тест А: Прямое управление УСО"
          "\n2) Тест Б: Управление УСО в фоновом режиме"
          "\n3) Изменить частоту подачи управляющих сигналов"
          "\n4) Изменить число активных каналов УСО"
          "\n5) Изменить характер тестового сигнала"
          "\n6) Изменить номер управляемого канала АЦП"
        "\n\n0) Выйти в DOS"
          "\n -------------------------------------"
          "\n Частота=%7.3f, ЦАП=%u(%u), АЦП=%u, Сигнал :%s"
          "\n -------------------------------------"
          "\n\nВаш выбор >",Cfg.Freq,Cfg.NumDAC,Cfg.Act,Cfg.NumADC,SigMsg[Cfg.Sig]);
      while(kbhit()) getch();
      switch(getche())
         {
         case '1':  TestA(); break;
         case '2':  TestB(); break;
         case '3':  SetFreq(); break;
         case '4':  Channel(); break;
         case '5':  Signal();  break;
         case '6':  ActChannel(); break;
         case '0':
              DrvDeinitialize();
              printf("\n\n ****** Конец работы *******\n\n");
              val=0;
              break;
         default: putch(7);
         }
      }
   }
 else printf("DAC/ADC driver mast be installed previosly.\n");

 val=open("test_drv.ini",O_BINARY|O_WRONLY|O_CREAT,0);
 if(val>0)
   {
   write(val,&Cfg,sizeof(Cfg));
   close(val);
   }
 return 0;
}

/* ====== END OF FILE ====== */
