#include "CalcTemp.h"

#define TEMP_TABLE_LIST_SIZE   116
//#define REFERENCE_RESISTOR     10000
//#define SUPPLY_VDD             3300

static u8 config_lookup_temp_tables(u32 resistance, s32 *outTemp);
static void CalcBatteryTemperature(u32 Rx, s32 *ptemp);

typedef struct _TEMP_TABLE_T
{
    short  	    temp;
    u32		resistance;
} TEMP_TABLE_T;

code TEMP_TABLE_T temp_table[TEMP_TABLE_LIST_SIZE] =
{
    {-1000,52724},
{-900,50058},
{-800,47546},
{-700,45177},
{-600,42943},
{-500,40834},
{-400,38838},
{-300,36952},
{-200,35172},
{-100,33489},
{0,31898},
{100,30377},
{200,28939},
{300,27579},
{400,26292},
{500,25074},
{600,23886},
{700,22762},
{800,21699},
{900,20692},
{1000,19739},
{1100,18835},
{1200,17978},
{1300,17166},
{1400,16396},
{1500,15665},
{1600,14960},
{1700,14290},
{1800,13655},
{1900,13052},
{2000,12480},
{2100,11932},
{2200,11411},
{2300,10917},
{2400,10447},
{2500,10000},
{2600,9576},
{2700,9173},
{2800,8789},
{2900,8423},
{3000,8075},
{3100,7737},
{3200,7415},
{3300,7109},
{3400,6816},
{3500,6538},
{3600,6275},
{3700,6024},
{3800,5785},
{3900,5556},
{4000,5338},
{4100,5127},
{4200,4925},
{4300,4733},
{4400,4549},
{4500,4373},
{4600,4203},
{4700,4041},
{4800,3886},
{4900,3737},
{5000,3591},
{5100,3461},
{5200,3332},
{5300,3208},
{5400,3090},
{5500,2977},
{5600,2868},
{5700,2764},
{5800,2664},
{5900,2568},
{6000,2477},
{6100,2390},
{6200,2308},
{6300,2228},
{6400,2151},
{6500,2078},
{6600,2007},
{6700,1938},
{6800,1872},
{6900,1809},
{7000,1748},
{7100,1690},
{7200,1635},
{7300,1582},
{7400,1531},
{7500,1482},
{7600,1432},
{7700,1384},
{7800,1339},
{7900,1295},
{8000,1252},
{8100,1212},
{8200,1173},
{8300,1136},
{8400,1100},
{8500,1066},
{8600,1033},
{8700,1001},
{8800,970},
{8900,940},
{9000,912},
{9100,884},
{9200,857},
{9300,831},
{9400,806},
{9500,781},
{9600,758},
{9700,737},
{9800,715},
{9900,695},
{10000,675},
{10100,655},
{10200,636},
{10300,618},
{10400,601},
{10500,583},
};

s32 CalcTemperature(u32 Rx)
{
	s32 temp;
	
	if(config_lookup_temp_tables(Rx, &temp)==1)	{//³¬³ö·¶Î§
		return -5000;
	}
	return temp;
}

static u32 config_temp_calculate_decimal(u32 max, u32 mix, u32 resistance)
{
    int i;
    u32 step, resistance_1;

    step = (max - mix) / 10;
    for(i = 0; i < 10 + 1; i++)
    {
        resistance_1 = max - i * step;
        if(resistance_1 < resistance )
            return i * 10;
    }

    return 0;
}

static u8 config_lookup_temp_tables(u32 resistance, s32 *outTemp)
{
    u8 i = 0;
    u32 difference;
    int table_size = sizeof(temp_table) / sizeof( TEMP_TABLE_T);
	
    if((resistance > temp_table[table_size - 1].resistance) && (resistance < temp_table[0].resistance))
    {
        for(i = 0; i < table_size; i++)
        {
            if(resistance == temp_table[i].resistance)
            {
                *outTemp = temp_table[i].temp;
                return 0;
            }
            if(temp_table[i].resistance < resistance )
            {
                difference = config_temp_calculate_decimal(temp_table[i - 1].resistance, temp_table[i].resistance, resistance);
                *outTemp = temp_table[i - 1].temp + difference;
                return 0;
            }
        }
    }
	return 1;
}

