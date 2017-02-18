/*Функции и переменные для программы <ACHH.C>  */
/*            <PRO_ACHH.H>                     */
#define VIRTUAL


#include <stdio.h>
#include <math.h>
#include <tcxldef.h>

#define NMAX      300
#define WMAX      102
#define KAN_MAX   4
#define CHANNEL   4
#define ZAPAZ     4
#define PMAX      3
#define QMAX      4
#define PSMAX     50



extern int nki,nko,nq,q,qq,n1,p,wybor,pp,kod_upr,nn;
extern unsigned long nr,cycle;
#ifdef VIRTUAL
extern float huge *x[4],huge *y[4],huge *yt[4];
#else
extern float x[4][NMAX+1],y[4][NMAX+1],yt[4][NMAX+1];
#endif
extern float w[WMAX+1];
extern float delt,fn,fw,fdisk;
extern float xh[WMAX+1],yh[KAN_MAX][KAN_MAX][WMAX+1];
extern float wxy[4][4];
extern float xmin,ymin;
extern float xmax,ymax;
extern float xk[KAN_MAX+1],yk[KAN_MAX+1];
extern float be[KAN_MAX+2][KAN_MAX+2][KAN_MAX+2][KAN_MAX+2];
extern float psx[CHANNEL][PSMAX],psy[CHANNEL][PSMAX];
extern float del_x_k[6][CHANNEL][PSMAX],del_y_k[6][CHANNEL][PSMAX];
extern float beta[CHANNEL][PMAX],gamma[CHANNEL];
/* эталон из файла */
extern char file_name[];
extern float file_k[],file_st[];

/* эталон/стат.хар-ка/управление/взаимодействие в стат. в файл */
extern char  ifile_name[],sfile_name[],ufile_name[],wfile_name[];
extern float ifile_k,ifile_st,sfile_k,sfile_st;

extern float a0[4],a1[4],a2[4],a3[4];
extern float f0[4],f1[4],f2[4],f3[4];
extern float fa0[4],fa1[4],fa2[4],fa3[4];
extern float u0[4];
extern char wid_modeli[];

void ScrSave(void);
void ScrRest(void);

void Imit_Driv(void);
int     Enable(void);
void   Disable(void);

void graf_achh(void);
void tabl_achh(void);
void   do_achh(void);

void p_garm(void);
void m_p_garm(void);

void  graf4(float,float,float,float);
void  graf_stat(void);
void  tabl_stat(void);
void    do_stat(void);
void    wz_stat(void);

void CTYP wz_s(NOARG);
void CTYP poly_sin(NOARG);
void CTYP sys_har(NOARG);
void CTYP Inf_Rab(NOARG);

void Out_kan(int,int);
void       Out_y(int);
void     per_pr(void);
void        shag(int);
void   tab_y_wz(void);
void   tab_beta(void);

void  tabl_time(void);
void  graf_time(void);
void  graf_2time(void);


void  ShutDown(void);
void  StartUp(void);

void  Razgruzka(void);
void  GotoNull(void);
int   AllocBuf(void);
void  FreeBuf(void);

float huge Sign_upr(short,unsigned long);
float huge Sign_per(short,unsigned long);
void  par_mod(short,unsigned long);
void  Loop(void);
int  etalon(void);


void write_et(void);
void write_st(void);
void write_wzs(void);
void write_upr(void);

void vi_et_graph(void);
void vi_et_tabl(void);

void from_stend(void);


