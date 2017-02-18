/*   Application program inteface for UPI's drivers */
#ifndef __DAC__ADC__
#define __DAC__ADC__


#ifdef __cplusplus
  extern "C" {
#endif

typedef unsigned int Word;
typedef void interrupt (far *HookPtr)(void);

#define DRV_IDstring "Stend driver for DAC/ADC"

typedef struct
{
 int version;

 /* --- identification --- */
 char far *TableID;

 /* --- copyright,name --- */
 char far *Copyright;
 char far *NameDRV;

 /* --- initialize --- */
 int pascal (far *_Enable)(Word DivHi,Word DivLo);
 int pascal (far *_Disable)(void);
 int pascal (far *_GetVolADC)(Word far *ADCbuf);  /* current ADC vol */
 int pascal (far *_SetVolDAC)(Word far *DACbuf);  /* initialize DAC vol */

 /* --- usage --- */
 int pascal (far *_SetHook)(HookPtr HookFun);
 int pascal (far *_ClrHook)(void);
 int pascal (far *_Volume)(Word far *DACbuf,Word far *ADCbuf);
 int pascal (far *_GetBuf)(Word far *far *DACbuf,Word far *far *ADCbuf);
 int pascal (far *_WaitDAC_ADC)(void);

 /* --- channel --- */
 int pascal (far *_SetChannel)(int numDAC,int numADC);

 /* --- Skipped --- */
 int pascal (far *_ClrSkipped)(void);
 int pascal (far *_GetSkipped)(unsigned long far *val);

 /* --- Info --- */
 int pascal (far *_GetMaxFreq)(unsigned long far *val);
 int pascal (far *_GetMaxChannel)(Word far *dac,Word far *adc);
 int pascal (far *_GetInfoDAC)(Word far *dac);
 int pascal (far *_GetInfoADC)(Word far *adc);

} DRV_Table_T,far *DRV_Table_P,far *far *DRV_Table_I;


extern DRV_Table_T __FuncTable__;



#define DRV_OK	     0
#define DRV_FAILED  -1
#define DRV_NONE    -2
#define DRV_ERRARG  -3
#define DRV_ERRFUN  -4

int DrvGetFuncTable(DRV_Table_I tbl);



		/* Проверить присутствие драйвера ЦАП/АЦП в памяти */
int DrvIsPresent(void);
		/* Инициализировать драйвер ЦАП/АЦП */
int DrvInitialize(void);
		/* Деинициализировать драйвер ЦАП/АЦП */
int DrvDeinitialize(void);
		/* Взять строку авторских прав */
int DrvGetCopyright(char far *str);
		/* Взять строку названия драйвера */
int DrvGetName(char far *str);
		/* Взять версию драйвера */
int DrvGetVersion(int far *ver);
		/* Дать описание последней ошибки драйвера */
int DrvGetErrorMsg(char far *str);

		/* Включить систему ОС */
int DrvEnable(float Freq);
int _DrvEnable(Word DivHi,Word DivLo);
		/* Выключить систему ОС */
int DrvDisable(void);
		/* Прочитать текущие значения на всех каналах АЦП */
int _DrvGetVolADC(Word far *ADCbuf);
int huge DrvGetVolADC(float far *ADCbuf);
		/* Установить начальные значения на всех каналах ЦАП */
int _DrvSetVolDAC(Word far *DACbuf);
int huge DrvSetVolDAC(float far *DACbuf);


		/* Установить функцию слежения в ОС */
int DrvSetHook(HookPtr *HookFun);
		/* Снять функцию слежения с ОС */
int DrvClrHook(void);
		/* Передать на ЦАП / снять с АЦП новые значения */
int _DrvVolume(Word far *DACbuf,Word far *ADCbuf);
int DrvVolume(float far *DACbuf,float far *ADCbuf);
		/* Взять адреса областей хранения данных ЦАП/АЦП */
int _DrvGetBuf(Word far *far *DACbuf,Word far *far *ADCbuf);
int DrvGetBuf(float far *far *DACbuf,float far *far *ADCbuf);
		/* Установить новые значения ЦАП и прочитать АЦП */
int DrvWaitDAC_ADC(void);

		/* Установить число используемых каналов ЦАП/АЦП */
int _DrvSetChannel(Word NumDAC,Word NumADC);
int DrvSetChannel(Word NumDAC,Word NumADC);

		/* Обнулить число пропущенных тактов */
int DrvClrSkipped(void);
		/* Взять число пропущенных тактов */
int DrvGetSkipped(unsigned long far *val);

		/* Взять максимально возможную частоту */
int DrvGetMaxFreq(unsigned long far *val);
		/* Взять максимальное число каналов ЦАП/АЦП */
int DrvGetMaxChannel(Word far *NumDAC,Word far *NumADC);
		/* Взять мах возможный уровень ЦАП */
int DrvGetInfoDAC(Word far *NumDAC);
		/* Взять мах возможный уровень АЦП */
int DrvGetInfoADC(Word far *NumADC);

#ifdef __cplusplus
  }
#endif


/* ==============[ macroses ]================= */

#define _DrvEnable(a,b)        __FuncTable__._Enable(a,b)
#define DrvDisable()	       __FuncTable__._Disable()
#define _DrvGetVolADC(a)       __FuncTable__._GetVolADC(a)
#define _DrvSetVolDAC(a)       __FuncTable__._SetVolDAC(a)

#define DrvSetHook(a)	       __FuncTable__._SetHook(a)
#define DrvClrHook()	       __FuncTable__._ClrHook()
#define _DrvVolume(a,b)        __FuncTable__._Volume(a,b)

#define _DrvGetBuf(a,b)        __FuncTable__._GetBuf(a,b)
#define DrvWaitDAC_ADC()       __FuncTable__._WaitDAC_ADC()

#define _DrvSetChannel(a,b)    __FuncTable__._SetChannel(a,b)

#define DrvClrSkipped()        __FuncTable__._ClrSkipped()
#define DrvGetSkipped(a)       __FuncTable__._GetSkipped(a)

#define DrvGetMaxFreq(a)       __FuncTable__._GetMaxFreq(a)
#define DrvGetMaxChannel(a,b)  __FuncTable__._GetMaxChannel(a,b)
#define DrvGetInfoDAC(a)       __FuncTable__._GetInfoDAC(a)
#define DrvGetInfoADC(a)       __FuncTable__._GetInfoADC(a)


#endif
