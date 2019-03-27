#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "includes.h"
#include "app_usart.h"

/*
	-----------------------------------------------------------------------\n
	|名称 | 消息头 |	消息长度  | 命令字 |	消息参数 |	校验和 |结束符|\n
	|	  | HEAD   |	LEN 	  |   MSG  |	PARA	 |	CHKSUM | END  |\n
	-----------------------------------------------------------------------\n
	|消息 | 7E	7E |L1	L2	L3	L4| M	   |	Pi		 |C1	C2 |  0D  |\n
	-----------------------------------------------------------------------\n
	|长度 | 2 Bytes|	4 Bytes   |1 Bytes |	n Bytes  | 2 Bytes |1 Byte|\n
	-----------------------------------------------------------------------\n
	1)	消息中的数据都为十六进制形式；\n
	2)	消息中数据统一采用高字节在前低字节在后的形式\n
	3)	消息长度Length=命令字+消息参数+校验和+结束符的字节个数\n
	4)	消息参数Parameter中i为0~n，当n为零时表示此消息不含消息参数\n
	5)	校验和Checksum=消息头+消息长度+命令字+消息参数的每个字节累加。
*/

#define BUFF_SIZE                      100u

#define MSG_SYSTEM_ACK_CMD              0x78    // 系统应答命令
#define MSG_ABNORMAL_CMD                0x79    // 消息错误命令

#define  IG_PROTOCOL_RX_SD0        0x7E           /* Start delimiters                                */
#define  IG_PROTOCOL_RX_SD1        0x7E
#define  IG_PROTOCOL_RX_END        0x0D           /* End   delimiter                                 */
                                                     /* Outbound packets (to NIOS-II)                   */
#define  IG_PROTOCOL_TX_SD0        0x7E           /* Start delimiters                                */
#define  IG_PROTOCOL_TX_SD1        0x7E
#define  IG_PROTOCOL_TX_END        0x0D           /* End   delimiter                                 */

#define	 IG_CMD_POSITION     6 
#define	 IG_LENGTH_SIZE     4                                               ///< 消息中Length所占字节数
#define  IG_START_LEN       5                                               ///< 4字节长度 + 1字节命令
#define  IG_EXTENT_LEN      4                                               ///< 1字节命令 + 2字节校验和 + 1字节结束符
#define  IG_END_LEN         3                                               ///< 2字节校验和 + 1字节结束符

#define  IG_RX_STATE_SD0              0           /* waiting for start first  start delimiter (SD0)  */
#define  IG_RX_STATE_SD1              1           /* waiting for start second start delimiter (SD1)  */
#define  IG_RX_STATE_LEN0             2           /* waiting for len0  byte                       */
#define  IG_RX_STATE_LEN1             3           /* waiting for len1  byte                      */
#define  IG_RX_STATE_LEN2             4           /* waiting for len2  byte                       */
#define  IG_RX_STATE_LEN3             5           /* waiting for len3  byte                      */
#define  IG_RX_STATE_DATA             6           /* waiting for data                                */
#define  IG_RX_STATE_CHKSUM0          7           /* waiting for checksum0 low byte                  */
#define  IG_RX_STATE_CHKSUM1          8           /* waiting for checksum1 high byte                 */
#define  IG_RX_STATE_END              9           /* waiting for end delimiter                       */

#define  IG_TX_STATE_SD0              0           /* Transmit state machine states                   */
#define  IG_TX_STATE_SD1              1
#define  IG_TX_STATE_LEN0             2
#define  IG_TX_STATE_LEN1             3
#define  IG_TX_STATE_LEN2             4
#define  IG_TX_STATE_LEN3             5
#define  IG_TX_STATE_DATA             6
#define  IG_TX_STATE_CHKSUM0          7
#define  IG_TX_STATE_CHKSUM1          8
#define  IG_TX_STATE_END              9

typedef enum {
    Periphery_StationHardwarePCB	        =	0x0B,	    //Station PCB版本
    Periphery_StationHardwarePCBA	        =	0x0C,	    //Station PCBA版本
    Periphery_StationFirmware		        =	0x0D,		//Station firmware版本
    Periphery_StationData			=	0x0E,		//Station 入库时间
    
    IG_CMD_SET_SYS_INFOR			=	0X44,//修改系统配置信息
    IG_CMD_EXECUTE_SYS_INFOR			=	0X45,//执行系统配置信息修改
    IG_CMD_GET_SYS_STATUS                       =       0X47,//查询系统状态,查询间隔1s
    IG_CMD_LPM					= 	0X54,//系统低功耗开关
    IG_CMD_MOTOR_SET_POS                        =       0x57,   // 设置孔位
    IG_CMD_MOTOR_ADJUST				=	0X58,	// 电机自校准、
    IG_CMD_MOTOR_GET_AJUST_VALUE	        =	0X59,	// 查询电机自校准值
    IG_CMD_SYSTEMINFO_BURN                      =       0x5a,   // 烧写系统信息命令
    //IG_CMD_Z_MOTOR_RESET                        =       0X5c,
    IG_CMD_Z_MOTOR_RESET                        =       0X56,
    IG_CMD_Z_MOVE_ANY_POS_AtReset                       =       0X5d,                        
    IG_CMD_EXPOSURE_TIME                        =       0X5e,
    IG_CMD_SET_PHOTO_SIZE	                =       0X60,	//设置照片size
    IG_CMD_START_TAKE_PHOTO	                        =       0X61,	//读取照片
    IG_CMD_CHECK_PHOTO_STATUS                   = 0x51,//0X51,	//查询拍照状态
    IG_CMD_TRANSFER_PHOTO_DATA                  =       0X62,
    IG_CMD_SET_LED_BRT                            =       0X63,//LED亮度设置
    IG_CMD_DBG_GetZMotorStatus                  =       0X64,//查询Z电机状态
    IG_CMD_FW_UPGRADE                           =       0X77,//固件升级
    IG_MSG_ACK_ERROR_CMD                	=	0x7A,    // 异常ACK命令
    
    IG_CMD_DBG_LED_ONOFF            		=       0xa0,  // 明场灯/激发灯开关控制
    IG_CMD_DBG_MoveAnyPos                       =       0xa1,   // 移动到任意位置
    IG_CMD_DBG_MoveAnyPosAtReset                =       0xa2,    // 通信协议调试命令
    //IG_CMD_DBG_GetMotorStatus		        =	0xa3,	// 查询XY电机状态
    IG_CMD_DBG_Z_MOVE_ANY_POS_AtReset		=	0xa4,	//	debug 设置z轴位置
    IG_CMD_DBG_GET_RESET_ADJUEST_STATUS		=	0xa5,	// 查询零位校准状态
    IG_CMD_DBG_GET_MOTOR_POS		        =	0xa6,	// 得到回读位置
    IG_CMD_DBG_SET_MOTOR_PARA		        =	0xa7,	//	电机参数设置
    IG_CMD_DBG_GET_MOTOR_PARA		        =	0xa8,	//	电机参数回读
   // IG_CMD_DBG_SET_MOTOR_SPEED		        =	0xa8,	//	电机速度设置
    IG_CMD_DBG_GET_ZMOTOR_POS		        =	0xa9,	// 得到回读位置
    IG_CMD_DBG_Z_MOTOR_AGING_TEST               =       0xab,   //z电机老化测试
    IG_CMD_GET_LED_BRT                          =       0xb5,   //获取激发灯亮度值
} EMessageCmd;

enum USART_MSG_ERR {
    IG_MSG_ERR_NONE              =  0x0000u, // 无消息错误
    IG_MSG_ERR_LENGTH            =  0xFFF7u, // 消息长度错误代码
    IG_MSG_ERR_DLE_WORD          =  0xFFF8u, // 插入INSERTWARD错误代码
    IG_MSG_ERR_PARAMETER         =  0xFFF9u, // 消息参数错误代码
    IG_MSG_ERR_CHECKSUM          =  0xFFFAu, // 消息校验和错误代码
    IG_MSG_ERR_ETX_WORD          =  0xFFFBu, // 消息结束符错误代码
    IG_MSG_ERR_STATE             =  0xFFFCu, // 消息未定义状态
    IG_MSG_ERR_FRAGMENT          =  0xFFFDu, // 消息不完整错误代码
    IG_MSG_ERR_UNDEFINITION      =  0xFFFEu, // 消息命令未定义错误代码
    IG_MSG_ERR_UNKNOWN           =  0xFFFFu
};

enum photo_status       {
  PHOTO_IDLE=0,//空闲
  PHOTO_TAKING=1,//正在拍照
  PHOTO_READY=2,//拍照完成
  PHOTO_ERROR=3//拍照错误
};

void protocol_process(usart_t *pUsart,message_pkt_t msg[2]);

#endif
