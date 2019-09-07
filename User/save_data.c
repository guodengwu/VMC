#include "save_data.h"

save_data_t flash_savedat;

//保存产品信息
void SaveShipResult(u8 *buf,u16 len,u8 flag)
{
	
	if(flag&WRITE_NORMAL)	{
			EEPROM_SectorErase(ShipRes_EEPROMAddr);
			EEPROM_write_n(ShipRes_EEPROMAddr,buf,len);
	}
	if(flag&WRITE_BACKUP)	{
		EEPROM_SectorErase(ShipResBK_EEPROMAddr);
		EEPROM_write_n(ShipRes_EEPROMAddr,buf,len);
	}
}
/*
//保存96 led亮度值
void SaveLedBrt(u8 *buf,u16 len,u8 flag)
{
	FlashUnwp();//保护引脚有效后，Flash自动进入寄存器保护状态，需要重新解锁
	if(flag==WRITE_NORMAL)
		g_FlashWr.all = LED_BRIGHTNESS;			//产品信息
	else if(flag==WRITE_BACKUP)
		g_FlashWr.all = LED_BRIGHTNESS_BACKUP;			//产品信息
	else if(flag==WRITE_BOTH) 	{
		g_FlashWr.all = LED_BRIGHTNESS;
		WriteFlash(buf, len, WITH_ERASE, WITH_CHECK);		//写Flash
		g_FlashWr.all = LED_BRIGHTNESS_BACKUP;
	}
	WriteFlash(buf, len, WITH_ERASE, WITH_CHECK);		//写Flash
	FlashWp();//打开硬件写保护
}*/

//根据情况保存数据，在idle任务中运行
u16 ShipResCRC16_bk=0;
void save_data(void)
{
	if(flash_savedat.type==SAVE_NONE)	{
		return;
	}
	else if(flash_savedat.type&SAVE_SHIP_RESULT)	{		
		flash_savedat.type &= ~SAVE_SHIP_RESULT;
		SaveShipDat.crc16 = SaveShipDat.flag + SaveShipDat.len + crc16(SaveShipDat.buf,SaveShipDat.len);//计算CRC16
		if(ShipResCRC16_bk != SaveShipDat.crc16)	{
			//BSP_PRINTF("save ship data\r\n");			
			ShipResCRC16_bk = SaveShipDat.crc16;
			SaveShipResult((u8 *)&SaveShipDat,sizeof(SaveShipDat),WRITE_BOTH);
		}		
	}
}


