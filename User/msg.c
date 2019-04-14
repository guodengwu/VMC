#include "msg.h"
#include <intrins.h>

void msg_init(void)
{
  //  INT8U err;

    //z_stop_sem          = OSSemCreate(0);
    //OSEventNameSet(piping_mail, (INT8U *)"piping_mail", &err);
}

void mutex_lock (OS_EVENT *lock)
{
    INT8U   err;

    OSSemPend(lock, 0, &err);
}

void mutex_unlock (OS_EVENT *lock)
{
    OSSemPost(lock);
}

INT8U  UsartRxGetINT8U (u8 *buf,u32 *idx)
{
    return (buf[(*idx)++]);
}

INT16U  UsartRxGetINT16U (u8 *buf,u32 *idx)
{
    INT16U  lowbyte;
    INT16U  highbyte;

    lowbyte  = UsartRxGetINT8U(buf,idx);
    highbyte = UsartRxGetINT8U(buf,idx);
    return ((highbyte << 8) | lowbyte);
}

INT32U  UsartRxGetINT32U (u8 *buf,u32 *idx)
{
    INT32U  highword;
    INT32U  lowword;

    lowword = UsartRxGetINT16U(buf,idx);
    highword = UsartRxGetINT16U(buf,idx);
    return ((highword << 16) | lowword);
}

void delay_us(u32 us)
{
	u16 i,j;
	
	for(i=0;i<2;i++)	{
		for(j=0;j<10;j++)
				_nop_();
	}
}

void DecToBCD(u32 Dec, u8 *pBCD, u8 len)
{
	u8 i;
	u32 temp;
	
	for(i=len-1;i>=0;i--)	{
		temp = Dec%100;
		pBCD[i] = ((temp/10)<<4) + ((temp%10)&0x0f);
		Dec /= 100;
	}
	return;
}
#if 0
INT16U crc16(INT8U *buf,INT8U len)
{
	u8 i;
	u16 ret;

	for(i=0;i<len;i++)	{
		ret += buf[i];
	}
	return ret;
}
#endif
