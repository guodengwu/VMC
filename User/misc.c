#include "misc.h"


void delay_us(u32 us)
{
	u16 i,j;
	
	for(i=0;i<us;i++)	{
		for(j=0;j<20;j++)
				_nop_();
	}
}

void DecToBCD(u16 Dec, u8 *pBCD, u8 len)
{
	u8 i;
	u16 temp,dec_tmp;
	
	dec_tmp=Dec;
	for(i=len-1;i>=0;i--)	{
		temp = dec_tmp%100;
		pBCD[i] = ((temp/10)<<4) + ((temp%10)&0x0f);
		dec_tmp /= 100;
	}
	return;
}

//自己写的log函数
float MYLOG(float a)
{
   int N = 10;//取5+1项计算
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
//热敏电阻和温度转换公式 1/T1 =ln(Rt/Rp)/Bx+1/T2
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

INT8U crc8(INT8U *dat, INT8U len)
{
	INT8U crc=0,j;
	for(j=0; j<len; j++)
	{
		crc = crc + dat[j];
	}
	return crc;
}
