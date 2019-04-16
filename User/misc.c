#include "misc.h"

//????log??
float MYLOG(float a)
{
   int N = 10;//?????10+1????
   int k,nk;
   float x,xx,y;
   x = (a-1)/(a+1);
   xx = x*x;
   nk = 2*N+1;
   y = 1.0/nk;
   for(k=N;k>0;k--)
   {
     nk = nk - 2;
     y = 1.0/nk+xx*y;
   }
   return 2.0*x*y;
}
//???????????1/T1 =ln(Rt/Rp)/Bx+1/T2
/*#define Rp				120
#define Bx				3910*/
#define	Ka				273.15f
#define TEMP_25			25.0f
#define T2_cent			(1/(Ka+TEMP_25))//0.003354f//298.15f
float CalculateTemperature(float dat,const u16 Rp,const u16 Bx)
{
	float temp;

	temp = dat/Rp;
	temp = MYLOG(temp);//ln(Rt/Rp)
	temp/=Bx;//ln(Rt/Rp)/B
	temp+=T2_cent;
	temp = 1/(temp);
	temp-=Ka;

	return temp;
}