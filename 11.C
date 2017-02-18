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
      }
    i=0;
    xk[0]=xk[1]=xk[2]=xk[3]=xmin;
    per_pr();
    Out_kan(i,m);

    for(m=1;m<=15;m++)
      {
      shag(m);
      for(k=0;k<=3;k++)
        xk[k]=(fi4[m][k]==-1)?xmin:xmax;
      Imit_Driv();
      per_pr();
      Out_kan(i,m);
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
