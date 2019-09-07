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
u16 ShipResCRC16_bk=0;
void save_data(void)
{
	if(flash_savedat.type==SAVE_NONE)	{
		return;
	}
	else if(flash_savedat.type&SAVE_SHIP_RESULT)	{		
		flash_savedat.type &= ~SAVE_SHIP_RESULT;
		SaveShipDat.crc16 = SaveShipDat.flag + SaveShipDat.len + crc16(SaveShipDat.buf,SaveShipDat.len);//����CRC16
		if(ShipResCRC16_bk != SaveShipDat.crc16)	{
			//BSP_PRINTF("save ship data\r\n");			
			ShipResCRC16_bk = SaveShipDat.crc16;
			SaveShipResult((u8 *)&SaveShipDat,sizeof(SaveShipDat),WRITE_BOTH);
		}		
	}
}


