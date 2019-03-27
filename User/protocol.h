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

#define  IG_PROTOCOL_RX_SD0        0x7E           /* Start delimiters                                */
#define  IG_PROTOCOL_RX_SD1        0x7E
#define  IG_PROTOCOL_RX_END        0x0D           /* End   delimiter                                 */
                                                     /* Outbound packets (to NIOS-II)                   */
#define  IG_PROTOCOL_TX_SD0        0x7E           /* Start delimiters                                */
#define  IG_PROTOCOL_TX_SD1        0x7E
#define  IG_PROTOCOL_TX_END        0x0D           /* End   delimiter                                 */

#define	 IG_CMD_POSITION     6 
#define	 IG_LENGTH_SIZE     4                                               ///< ��Ϣ��Length��ռ�ֽ���
#define  IG_START_LEN       5                                               ///< 4�ֽڳ��� + 1�ֽ�����
#define  IG_EXTENT_LEN      4                                               ///< 1�ֽ����� + 2�ֽ�У��� + 1�ֽڽ�����
#define  IG_END_LEN         3                                               ///< 2�ֽ�У��� + 1�ֽڽ�����

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
    Periphery_StationHardwarePCB	        =	0x0B,	    //Station PCB�汾
    Periphery_StationHardwarePCBA	        =	0x0C,	    //Station PCBA�汾
    Periphery_StationFirmware		        =	0x0D,		//Station firmware�汾
    Periphery_StationData			=	0x0E,		//Station ���ʱ��
    
    IG_CMD_SET_SYS_INFOR			=	0X44,//�޸�ϵͳ������Ϣ
    IG_CMD_EXECUTE_SYS_INFOR			=	0X45,//ִ��ϵͳ������Ϣ�޸�
    IG_CMD_GET_SYS_STATUS                       =       0X47,//��ѯϵͳ״̬,��ѯ���1s
    IG_CMD_LPM					= 	0X54,//ϵͳ�͹��Ŀ���
    IG_CMD_MOTOR_SET_POS                        =       0x57,   // ���ÿ�λ
    IG_CMD_MOTOR_ADJUST				=	0X58,	// �����У׼��
    IG_CMD_MOTOR_GET_AJUST_VALUE	        =	0X59,	// ��ѯ�����У׼ֵ
    IG_CMD_SYSTEMINFO_BURN                      =       0x5a,   // ��дϵͳ��Ϣ����
    //IG_CMD_Z_MOTOR_RESET                        =       0X5c,
    IG_CMD_Z_MOTOR_RESET                        =       0X56,
    IG_CMD_Z_MOVE_ANY_POS_AtReset                       =       0X5d,                        
    IG_CMD_EXPOSURE_TIME                        =       0X5e,
    IG_CMD_SET_PHOTO_SIZE	                =       0X60,	//������Ƭsize
    IG_CMD_START_TAKE_PHOTO	                        =       0X61,	//��ȡ��Ƭ
    IG_CMD_CHECK_PHOTO_STATUS                   = 0x51,//0X51,	//��ѯ����״̬
    IG_CMD_TRANSFER_PHOTO_DATA                  =       0X62,
    IG_CMD_SET_LED_BRT                            =       0X63,//LED��������
    IG_CMD_DBG_GetZMotorStatus                  =       0X64,//��ѯZ���״̬
    IG_CMD_FW_UPGRADE                           =       0X77,//�̼�����
    IG_MSG_ACK_ERROR_CMD                	=	0x7A,    // �쳣ACK����
    
    IG_CMD_DBG_LED_ONOFF            		=       0xa0,  // ������/�����ƿ��ؿ���
    IG_CMD_DBG_MoveAnyPos                       =       0xa1,   // �ƶ�������λ��
    IG_CMD_DBG_MoveAnyPosAtReset                =       0xa2,    // ͨ��Э���������
    //IG_CMD_DBG_GetMotorStatus		        =	0xa3,	// ��ѯXY���״̬
    IG_CMD_DBG_Z_MOVE_ANY_POS_AtReset		=	0xa4,	//	debug ����z��λ��
    IG_CMD_DBG_GET_RESET_ADJUEST_STATUS		=	0xa5,	// ��ѯ��λУ׼״̬
    IG_CMD_DBG_GET_MOTOR_POS		        =	0xa6,	// �õ��ض�λ��
    IG_CMD_DBG_SET_MOTOR_PARA		        =	0xa7,	//	�����������
    IG_CMD_DBG_GET_MOTOR_PARA		        =	0xa8,	//	��������ض�
   // IG_CMD_DBG_SET_MOTOR_SPEED		        =	0xa8,	//	����ٶ�����
    IG_CMD_DBG_GET_ZMOTOR_POS		        =	0xa9,	// �õ��ض�λ��
    IG_CMD_DBG_Z_MOTOR_AGING_TEST               =       0xab,   //z����ϻ�����
    IG_CMD_GET_LED_BRT                          =       0xb5,   //��ȡ����������ֵ
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

enum photo_status       {
  PHOTO_IDLE=0,//����
  PHOTO_TAKING=1,//��������
  PHOTO_READY=2,//�������
  PHOTO_ERROR=3//���մ���
};

void protocol_process(usart_t *pUsart,message_pkt_t msg[2]);

#endif
