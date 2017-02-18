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
          {
//        xk[k]=(fi4[m][k]==-1)?xmin:xmax;
          if(fabs(fi4[m][k]-fi4[m-1][k])>((xmax-xmin)/2.0))
            {
            if(fi4[m][k]==1)
              {
              for(i=0;i<nq/2;i++)
                {
                xk[k]=xmin+i*del_x;
                Imit_Driv();
                }
              xk[k]=xmax;
              }
            else
              {
              for(i=0;i<nq/2;i++)
                {
                xk[k]=xmax-i*del_x;
                Imit_Driv();
                }
              xk[k]=xmin;
              }
            }

          Imit_Driv();
          per_pr();
          Out_kan(i,m);
          }
      }
