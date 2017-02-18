/*  */
#include <dos.h>
#include <string.h>

#include "dac_adc.h"

#define _fstrcmp(a,b) strcmp(a,b)
#define _fstrcpy(a,b) strcpy(a,b)
#define _fmemcpy(a,b,c) memcpy(a,b,c)

DRV_Table_T __FuncTable__;

static unsigned long DrvTimerFreq=0;
static unsigned char DrvNum=0;
static char NullErrMsg[]="Sorry, unknown description",
	    NoneDrvMsg[]="Driver don't installed";

static Word Adc[50],Dac[50],NumDAC,NumADC,MaxDAC,MaxADC;

#define TEST_SUBFUN	   0
#define INIT_SUBFUN	   1
#define DEINIT_SUBFUN	   2
#define GETTBL_SUBFUN	   3
#define COPYRIGHT_SUBFUN   4
#define ERRMSG_SUBFUN	   5


#define ToDAC(a)    ( (int)(( (a)<-1 ? -1: ((a)>1?1:(a)) *MaxDAC)+MaxDAC+0.5) )
#define FromADC(a)  ( ((float)(((int)(a))-(int)MaxADC)) /(int)MaxADC)



int DrvEnable(float Freq)
{
 Word DivHi,DivLo;
 int val;

 if(Freq<=0) return DRV_ERRARG;
 if(DrvTimerFreq/Freq>65000L) DivHi=1000;
 else DivHi=10;
 DivLo=(Word)(DrvTimerFreq/(Freq*DivHi)+0.5);

// printf(">>>>>>>> %u %u",DivHi,DivLo);

 if(DivLo*DivHi>0) val=_DrvEnable(DivHi,DivLo);
 else		   val=DRV_ERRARG;

 return val;
}

int DrvIsPresent(void)
{
 union REGS r;
 unsigned char i;
 char far *IDstring,IDnormal[]=DRV_IDstring;

 DrvNum=0;
 for(i=0xff;i>=0xc0;i--)
   {
   r.h.al=TEST_SUBFUN;
   r.h.ah=i;
   int86(0x2f,&r,&r);
   IDstring=MK_FP(r.x.bx,r.x.dx);
   if(!_fstrcmp(IDstring,IDnormal))  { DrvNum=i; break; }
   }
 return DrvNum;
}

int DrvInitialize(void)
{
 union REGS r;
 int val;
 DRV_Table_P ptr;

 if(DrvIsPresent())
   {
   r.h.al=INIT_SUBFUN;
   r.h.ah=DrvNum;
   int86(0x2f,&r,&r);


   val=DrvGetFuncTable(&ptr);
   _fmemcpy(&__FuncTable__,ptr,sizeof(*ptr));
   if(val==DRV_OK)
     {
     DrvGetMaxFreq(&DrvTimerFreq);
     DrvGetMaxChannel(&NumDAC,&NumADC);
     DrvGetInfoDAC(&MaxDAC); MaxDAC/=2; MaxDAC--;
     DrvGetInfoADC(&MaxADC); MaxADC/=2; MaxADC--;
     }
   }
 else val=DRV_NONE;

 return val;
}

int DrvDeinitialize(void)
{
 union REGS r;
 int val;

 if(DrvNum)
   {
   r.h.al=DEINIT_SUBFUN;
   r.h.ah=DrvNum;
   int86(0x2f,&r,&r);
   val=DRV_OK;
   }
 else val=DRV_NONE;

 return val;
}


int DrvGetCopyright(char far *str)
{
 int val;

 if(DrvNum)
   {
   _fstrcpy(str,__FuncTable__.Copyright);
   val=DRV_OK;
   }
 else val=DRV_NONE;

 return val;
}

int DrvGetName(char far *str)
{
 int val;

 if(DrvNum)
   {
   _fstrcpy(str,__FuncTable__.NameDRV);
   val=DRV_OK;
   }
 else val=DRV_NONE;
 return val;
}

int DrvGetErrorMsg(char far *msg)
{
 union REGS r;
 char far *str=NullErrMsg;
 int val=DRV_OK;

 if(DrvNum)
   {
   r.h.ah=DrvNum;
   r.h.al=ERRMSG_SUBFUN;
   int86(0x2f,&r,&r);
   str=MK_FP(r.x.bx,r.x.dx);
   if(!str[0]) str=NullErrMsg;
   }
 else
   {
   val=DRV_NONE;
   str=NoneDrvMsg;
   }
 _fstrcpy(msg,str);

 return val;
}

int DrvGetFuncTable(DRV_Table_I tbl)
{
 union REGS r;
 int val;

 if(DrvNum)
   {
   r.h.al=GETTBL_SUBFUN;
   r.h.ah=DrvNum;
   int86(0x2f,&r,&r);
   *tbl=MK_FP(r.x.bx,r.x.dx);
   val=DRV_OK;
   }
 else val=DRV_NONE;

 return val;
}

int DrvSetChannel(Word dac,Word adc)
{
 int val;

 val=_DrvSetChannel(dac,adc);
 if(val==DRV_OK)
    {
    NumDAC=dac;
    NumADC=adc;
    }
 return val;
}

int DrvVolume(float far *BufDAC,float far *BufADC)
{
 int i,val;

 for(i=0;i<NumDAC;i++) Dac[i]=ToDAC(BufDAC[i]);

 val=_DrvVolume(Dac,Adc);

 for(i=0;i<NumADC;i++) BufADC[i]=FromADC(Adc[i]); 
 return val;
}

int huge DrvSetVolDAC(float far *BufDAC)
{
 int i;

 for(i=0;i<NumDAC;i++) Dac[i]=ToDAC(BufDAC[i]); 
 return _DrvSetVolDAC(Dac);
}

int huge DrvGetVolADC(float far *BufADC)
{
 int i,val;

 val=_DrvGetVolADC(Adc);
 for(i=0;i<NumADC;i++) BufADC[i]=FromADC(Adc[i]); 
 return val;
}
