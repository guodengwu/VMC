#include "tm1640.h"
#include "bsp.h"

#define WRITE_DATA_MODE_Z   0x40        // 地址自动加
#define WRITE_DATA_MODE_G   0x44        // 固定地址
#define START_DATA          0xC0        // 
#define DISPLAY_EN          0x8A        // 开显示
#define DISPLAY_DIS         0x80        // 关显示

static u8 DisBuf[16];           // 显示缓存

u8 code ch_table[]={
        0x3F,  //"0"
        0x06,  //"1"
        0x5B,  //"2"
        0x4F,  //"3"
        0x66,  //"4"
        0x6D,  //"5"
        0x7D,  //"6"
        0x07,  //"7"
        0x7F,  //"8"
        0x6F,  //"9"
        0x77,  //"A"
        0x7C,  //"B"
        0x39,  //"C"
        0x5E,  //"D"
        0x79,  //"E"
        0x71,  //"F"
        0x76,  //"H"
        0x38,  //"L"
        0x37,  //"n"
        0x3E,  //"u"
        0x73,  //"P"
        0x5C,  //"o"
        0x40,  //"-"
        0x00,  //熄灭
};

// 开始
static void start()
{
    TM1640_SCLK=1;
    delay_us(1);
    TM1640_DIO=1;
    delay_us(1);
    TM1640_DIO=0;
    delay_us(1);
    TM1640_SCLK=0;   
}

static void send_data(u8 x)          //送数据    低位先传
{
    u8  i,dat;
		
		dat = x;
    for(i=0;i<8;i++)
    {
        TM1640_SCLK=0;
        delay_us(1);
        TM1640_DIO=(bit)(dat&0x01);
        delay_us(1);
        TM1640_SCLK=1;
        dat>>=1;
    }
    TM1640_SCLK=0;
}

// 结束
static void stop()
{
    TM1640_SCLK=0;
    TM1640_DIO=0;
    TM1640_SCLK=1;
		_nop_();//_nop_();
    TM1640_DIO=1;
}

void Init_Display()
{
    start();
    send_data(DISPLAY_DIS);         // 关显示
    stop();

    start();
    send_data(WRITE_DATA_MODE_Z);   // 自动地址
    stop();

    start();
    send_data(0x8F);                // 开显示
    stop();
}
static void convert_dat(u8 *pbuf, s16 dat)
{
	u16 temp;
	u8 len=0;
	
	if(dat<0)	{
		pbuf[len++] = ch_table[22];//负数
		temp = -dat;
	}else	{
		temp = dat;
	}
	if(temp>=100)	{
		pbuf[len++] = ch_table[(u8)(temp/100)];
		pbuf[len++] = ch_table[(u8)(temp%100/10)];//取百位
		pbuf[len++] = ch_table[(u8)(temp%100%10)];//取个位
	}else if(temp>=10)	{
		pbuf[len++] = ch_table[(u8)(temp/10)];
		pbuf[len++] = ch_table[(u8)(temp%10)];
	}else {
		pbuf[len++] = ch_table[(u8)(temp)];
	}
}

static void Update_Display(void)
{
	u8 i;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif
		OS_ENTER_CRITICAL();
		start();
		send_data(START_DATA);              //起始地址
		for(i=0;i<4;i++)                   //送16位数
		{
				send_data(DisBuf[i]);
		}
		stop();
		OS_EXIT_CRITICAL();
}

void Update_DisInt(s16 dat)
{
    //if(Sys_Flag_B.Updata_Display_Flag)
    {
        //Sys_Flag_B.Updata_Display_Flag = false;
			memset(DisBuf,0,4);
			convert_dat(DisBuf, dat);
			Update_Display();
    }
}

void Update_DisString(u8 *string)
{
	u8 i;
	memset(DisBuf,0,4);
	for(i=0;i<4;i++)	{
		if(string[i]>='A' && string[i]<= 'L'){
			DisBuf[i] = ch_table[string[i] - 0X37];
		} else if(' '){
			DisBuf[i] = ch_table[23];
		}	else	{
			DisBuf[i] = ch_table[22];
		}
	}
	Update_Display();
}
