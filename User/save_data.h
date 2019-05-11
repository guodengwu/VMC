#ifndef __SAVE_DATA_H__
#define __SAVE_DATA_H__

#include "includes.h"

enum WRITE_PAGE{
	WRITE_NONE=0,
	WRITE_NORMAL=0x01,
	WRITE_BACKUP=0x02,
	WRITE_BOTH=0x03,
};

#define	SAVE_NONE								0
#define	SAVE_SHIP_RESULT						DEF_BIT00_MASK

typedef struct _save_data{
	u8 type;
}save_data_t;

////////////////////////spi flash 各分区使用情况，sactor a用于保存产品信息 写保护////////////////////
//Sector a 0~7 = 8 Pages *512 = 4096 Bytes 保存产品配置信息
#define	ShipRes_EEPROMAddr				0						
#define ShipResBK_EEPROMAddr			0X200						

////////////////////////////////////////////////////////////////////////////////////////
extern save_data_t flash_savedat;
void save_data();

#endif
