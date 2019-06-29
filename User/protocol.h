#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "includes.h"
#include "app_usart.h"

/*
	-----------------------------------------------------------------------\n
	|���� | ��Ϣͷ |	��Ϣ����  | ������ |	��Ϣ���� |	У��� |������|\n
	|	  | HEAD   |	LEN 	  |   MSG  |	PARA	 |	CHKSUM | END  |\n
	-----------------------------------------------------------------------\n
	|��Ϣ | 7E	7E |L1	L2	L3	L4| M	   |	Pi		 |C1	C2 |  0D  |\n
	-----------------------------------------------------------------------\n
	|���� | 2 Bytes|	4 Bytes   |1 Bytes |	n Bytes  | 2 Bytes |1 Byte|\n
	-----------------------------------------------------------------------\n
	1)	��Ϣ�е����ݶ�Ϊʮ��������ʽ��\n
	2)	��Ϣ������ͳһ���ø��ֽ���ǰ���ֽ��ں����ʽ\n
	3)	��Ϣ����Length=������+��Ϣ����+У���+���������ֽڸ���\n
	4)	��Ϣ����Parameter��iΪ0~n����nΪ��ʱ��ʾ����Ϣ������Ϣ����\n
	5)	У���Checksum=��Ϣͷ+��Ϣ����+������+��Ϣ������ÿ���ֽ��ۼӡ�
*/

#define BUFF_SIZE                      100u

#define MSG_SYSTEM_CMD_NONE								0X00
#define MSG_SYSTEM_CMD_ACK             		0x06    // ϵͳӦ������
#define MSG_SYSTEM_CMD_NAK                0x15    // ��Ϣ��������
#define MSG_FEEDBACK_ENABLE	1
#define MSG_FEEDBACK_DISABLE	0

#define  IG_PROTOCOL_RX_SD0        0x02           /* Start delimiters                                */
#define  IG_PROTOCOL_RX_SD1        0x00
#define  IG_PROTOCOL_RX_END        0x03           /* End   delimiter                                 */
                                                     /* Outbound packets (to NIOS-II)                   */
#define  IG_PROTOCOL_TX_SD0        0x02           /* Start delimiters                                */
#define  IG_PROTOCOL_TX_SD1        0x00
#define  IG_PROTOCOL_TX_END        0x03           /* End   delimiter                                 */

#define	 IG_CMD_POSITION     6 
#define	 IG_LENGTH_SIZE     4                                               ///< ��Ϣ��Length��ռ�ֽ���
#define  IG_START_LEN       5                                               ///< 4�ֽڳ��� + 1�ֽ�����
#define  IG_EXTENT_LEN      4                                               ///< 1�ֽ����� + 2�ֽ�У��� + 1�ֽڽ�����
#define  IG_END_LEN         3                                               ///< 2�ֽ�У��� + 1�ֽڽ�����
#define  IG_CMDANDSN_LEN         5

#define  IG_RX_STATE_SD0              0           /* waiting for start first  start delimiter (SD0)  */
#define  IG_RX_STATE_LEN0             1           /* waiting for len1  byte                      */
#define  IG_RX_STATE_LEN1             2           /* waiting for len2  byte                       */
#define  IG_RX_STATE_CMD            	3           /* waiting for len0  byte                       */
#define  IG_RX_STATE_SN0             4           /* waiting for len3  byte                      */
#define  IG_RX_STATE_SN1             5 
#define  IG_RX_STATE_SN2             6 
#define  IG_RX_STATE_SN3             7 
#define  IG_RX_STATE_DATA             8           /* waiting for data                                */
#define  IG_RX_STATE_CHKSUM          9           /* waiting for checksum0 low byte                  */
//#define  IG_RX_STATE_CHKSUM1          8           /* waiting for checksum1 high byte                 */
#define  IG_RX_STATE_END              10           /* waiting for end delimiter                       */

#define  IG_TX_STATE_SD0              0           /* waiting for start first  start delimiter (SD0)  */
#define  IG_TX_STATE_LEN0             1           /* waiting for len1  byte                      */
#define  IG_TX_STATE_LEN1             2           /* waiting for len2  byte                       */
#define  IG_TX_STATE_CMD            	3           /* waiting for len0  byte                       */
#define  IG_TX_STATE_SN0             4           /* waiting for len3  byte                      */
#define  IG_TX_STATE_SN1             5 
#define  IG_TX_STATE_SN2             6 
#define  IG_TX_STATE_SN3             7 
#define  IG_TX_STATE_DATA             8           /* waiting for data                                */
#define  IG_TX_STATE_CHKSUM          9           /* waiting for checksum0 low byte                  */
//#define  IG_RX_STATE_CHKSUM1          8           /* waiting for checksum1 high byte                 */
#define  IG_TX_STATE_END              10

typedef enum {
		CMD_ReportParam					= 0x01,//VMC���������ϱ�
		CMD_ReportError					= 0x02,//VMC�����ϱ�
		CMD_ReportShipResult	  = 0x04,//VMC��������ϱ�
		CMD_CheckOnlineStatus			= 0x05,//VMC��ȡ����״̬
	
		CMD_MOTOR_CTRL_TYPE			= 0X12,//����������� �������ƻ���ʱ�����
		CMD_HUASHUANG_CTRL			= 0X13,
    CMD_NotifyShip					=	0x14,//֪ͨ����
    CMD_ReplyOnlineStatus		=	0x15,//�ظ�����״̬
		CMD_ModifyTempParm			=	0x16,//�޸��¿ز���
		CMD_LightOnOff					=	0x17,//�ƿ���
		CMD_FogOnOff						=	0x18,//������
		CMD_SysReset						=	0x19//����
} EMessageCmd;

enum USART_MSG_ERR {
    IG_MSG_ERR_NONE              =  0x0000u, // ����Ϣ����
    IG_MSG_ERR_LENGTH            =  0xFFF7u, // ��Ϣ���ȴ������
    IG_MSG_ERR_DLE_WORD          =  0xFFF8u, // ����INSERTWARD�������
    IG_MSG_ERR_PARAMETER         =  0xFFF9u, // ��Ϣ�����������
    IG_MSG_ERR_CHECKSUM          =  0xFFFAu, // ��ϢУ��ʹ������
    IG_MSG_ERR_ETX_WORD          =  0xFFFBu, // ��Ϣ�������������
    IG_MSG_ERR_STATE             =  0xFFFCu, // ��Ϣδ����״̬
    IG_MSG_ERR_FRAGMENT          =  0xFFFDu, // ��Ϣ�������������
    IG_MSG_ERR_UNDEFINITION      =  0xFFFEu, // ��Ϣ����δ����������
    IG_MSG_ERR_UNKNOWN           =  0xFFFFu
};

u8 protocol_process(usart_t *pUsart,message_pkt_t msg[2], u8 *pAck);

#endif
