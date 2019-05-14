#include "save_data.h"

save_data_t flash_savedat;

//�����Ʒ��Ϣ
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
//����96 led����ֵ
void SaveLedBrt(u8 *buf,u16 len,u8 flag)
{
	FlashUnwp();//����������Ч��Flash�Զ�����Ĵ�������״̬����Ҫ���½���
	if(flag==WRITE_NORMAL)
		g_FlashWr.all = LED_BRIGHTNESS;			//��Ʒ��Ϣ
	else if(flag==WRITE_BACKUP)
		g_FlashWr.all = LED_BRIGHTNESS_BACKUP;			//��Ʒ��Ϣ
	else if(flag==WRITE_BOTH) 	{
		g_FlashWr.all = LED_BRIGHTNESS;
		WriteFlash(buf, len, WITH_ERASE, WITH_CHECK);		//дFlash
		g_FlashWr.all = LED_BRIGHTNESS_BACKUP;
	}
	WriteFlash(buf, len, WITH_ERASE, WITH_CHECK);		//дFlash
	FlashWp();//��Ӳ��д����
}*/

//��������������ݣ���idle����������
void save_data(void)
{
	if(flash_savedat.type==SAVE_NONE)	{
		return;
	}
	else if(flash_savedat.type&SAVE_SHIP_RESULT)	{
		BSP_PRINTF("save ship data\r\n");
		flash_savedat.type &= ~SAVE_SHIP_RESULT;
		SaveShipDat.crc8 = SaveShipDat.flag + SaveShipDat.len + crc8(SaveShipDat.buf,SaveShipDat.len);
		SaveShipResult((u8 *)&SaveShipDat,sizeof(SaveShipDat),WRITE_BOTH);
	}
}

