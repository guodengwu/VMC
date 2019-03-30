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

#define  IG_PROTOCOL_RX_SD0        0x20           /* Start delimiters                                */
#define  IG_PROTOCOL_RX_SD1        0x00
#define  IG_PROTOCOL_RX_END        0x0D           /* End   delimiter                                 */
                                                     /* Outbound packets (to NIOS-II)                   */
#define  IG_PROTOCOL_TX_SD0        0x20           /* Start delimiters                                */
#define  IG_PROTOCOL_TX_SD1        0x00
#define  IG_PROTOCOL_TX_END        0x0D           /* End   delimiter                                 */

#define	 IG_CMD_POSITION     6 
#define	 IG_LENGTH_SIZE     4                                               ///< 消息中Length所占字节数
#define  IG_START_LEN       5                                               ///< 4字节长度 + 1字节命令
#define  IG_EXTENT_LEN      4                                               ///< 1字节命令 + 2字节校验和 + 1字节结束符
#define  IG_END_LEN         3                                               ///< 2字节校验和 + 1字节结束符

#define  IG_RX_STATE_SD0              0           /* waiting for start first  start delimiter (SD0)  */
#define  IG_RX_STATE_SD1              1           /* waiting for start second start delimiter (SD1)  */
#define  IG_RX_STATE_CMD            	2           /* waiting for len0  byte                       */
#define  IG_RX_STATE_LEN             3           /* waiting for len1  byte                      */
//#define  IG_RX_STATE_LEN2             4           /* waiting for len2  byte                       */
//#define  IG_RX_STATE_LEN3             5           /* waiting for len3  byte                      */
#define  IG_RX_STATE_DATA             6           /* waiting for data                                */
#define  IG_RX_STATE_CHKSUM          7           /* waiting for checksum0 low byte                  */
//#define  IG_RX_STATE_CHKSUM1          8           /* waiting for checksum1 high byte                 */
//#define  IG_RX_STATE_END              9           /* waiting for end delimiter                       */

#define  IG_TX_STATE_SD0              0           /* Transmit state machine states                   */
#define  IG_TX_STATE_SD1              1
#define  IG_TX_STATE_CMD             2
#define  IG_TX_STATE_LEN             3
//#define  IG_TX_STATE_LEN2             4
//#define  IG_TX_STATE_LEN3             5
#define  IG_TX_STATE_DATA             6
#define  IG_TX_STATE_CHKSUM          7
//#define  IG_TX_STATE_CHKSUM1          8
#define  IG_TX_STATE_END              9

typedef enum {
    CMD_ReadSysStatus								=	0x01,//读取机器状态
    CMD_MotorMoveOneCircle					=	0x02,//出货电机转一圈
		CMD_ClearMoveResult							=	0x03,//清除运行结果
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

void protocol_process(usart_t *pUsart,message_pkt_t msg[2]);

#endif
