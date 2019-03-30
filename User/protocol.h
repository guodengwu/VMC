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

#define MSG_SYSTEM_ACK_CMD              0x78    // ϵͳӦ������
#define MSG_ABNORMAL_CMD                0x79    // ��Ϣ��������

#define  IG_PROTOCOL_RX_SD0        0x20           /* Start delimiters                                */
#define  IG_PROTOCOL_RX_SD1        0x00
#define  IG_PROTOCOL_RX_END        0x0D           /* End   delimiter                                 */
                                                     /* Outbound packets (to NIOS-II)                   */
#define  IG_PROTOCOL_TX_SD0        0x20           /* Start delimiters                                */
#define  IG_PROTOCOL_TX_SD1        0x00
#define  IG_PROTOCOL_TX_END        0x0D           /* End   delimiter                                 */

#define	 IG_CMD_POSITION     6 
#define	 IG_LENGTH_SIZE     4                                               ///< ��Ϣ��Length��ռ�ֽ���
#define  IG_START_LEN       5                                               ///< 4�ֽڳ��� + 1�ֽ�����
#define  IG_EXTENT_LEN      4                                               ///< 1�ֽ����� + 2�ֽ�У��� + 1�ֽڽ�����
#define  IG_END_LEN         3                                               ///< 2�ֽ�У��� + 1�ֽڽ�����

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
    CMD_ReadSysStatus								=	0x01,//��ȡ����״̬
    CMD_MotorMoveOneCircle					=	0x02,//�������תһȦ
		CMD_ClearMoveResult							=	0x03,//������н��
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

void protocol_process(usart_t *pUsart,message_pkt_t msg[2]);

#endif
