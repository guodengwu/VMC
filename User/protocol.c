#include "protocol.h"
#include "motor.h"
#include "app_ship.h"
#include "app_sysmonitor.h"

static u8 data_buf[10];
u8 protocol_process(usart_t *pUsart,message_pkt_t msg[2], u8 *pAck)
{
	u8 cmd,len=0,temp;
	u8 ret=MSG_FEEDBACK_DISABLE;
	
  cmd = msg[0].Cmd;
	*pAck = MSG_SYSTEM_CMD_NONE;
	sys_status.online_state = DEF_True;
  switch (cmd)
  {		
		case CMD_NotifyShip://0x14 ֪ͨ����
		{
//			u8 row,col;		
			temp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);//�� ����
			motor.row= (temp&0xf0)>>4;//�� ����
			motor.col= temp&0x0f;//�� ����
			//param = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);//����
			//timeout = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx)/10;//����ʱ�� s
			len = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
			if((appShip.state == SHIP_STATE_IDLE) && (len>0&&len<IMEI_LEN))	{
				sys_status.pIMEI->len = len;
				memcpy(sys_status.pIMEI->dat, pUsart->rx_buf+pUsart->rx_idx, len);//��ȡ������
				msg[0].Src = MSG_START_SHIP;
				OSMboxPost(appShip.MBox, &msg[0]);//����������������
				*pAck = MSG_SYSTEM_CMD_ACK;
			}else	{
				*pAck = MSG_SYSTEM_CMD_NAK;
			}
			break;
		}
		case CMD_ReplyOnlineStatus://15 �ظ�����״̬
		{
			temp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
			if(temp==0)	{
					sys_status.online_state = DEF_True;
					msg[0].Src = MSG_SYS_ONLINE;
					OSMboxPost(sysMonitor.Mbox, &msg[0]);
					
			}else if(temp==1)	{
					sys_status.online_state = DEF_False;
			}
			break;
		}
		case CMD_ModifyTempParm://16 �޸��¶Ȳ���
		{
			s8 targetTemp;
			*pAck = MSG_SYSTEM_CMD_ACK;
			temp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
			if(temp==1)	{//�ر�ѹ����
					IO_RELAY = 0;
					sys_status.pTempCtrl->flag = DEF_False;//�ر��¶ȿ���
			}else if(temp==0)	{
					sys_status.pTempCtrl->flag = DEF_True;//�����¶ȿ���
					targetTemp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);//����Ŀ���¶�
					sys_status.pTempCtrl->tInsideTempL = targetTemp - TEMP_RANGE;//������Ʒ�Χ
					sys_status.pTempCtrl->tInsideTempH = targetTemp + TEMP_RANGE;
			}else	{
					*pAck = MSG_SYSTEM_CMD_NAK;
			}									
			break;
		}
		case CMD_LightOnOff://0x17 �ƿ���
		{		
			*pAck = MSG_SYSTEM_CMD_ACK;
			temp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
			if(temp==1)	{//�رյƹ�
					sys_status.light_ctrl_enable = DEF_False;
			}else if(temp==0)	{//�����ƹ���ƣ��ƿ����ɹ����������
					sys_status.light_ctrl_enable = DEF_True;
			}else	{
					*pAck = MSG_SYSTEM_CMD_NAK;
				}			
			break;
		}
		case CMD_FogOnOff://0x18 ������
		{
				*pAck = MSG_SYSTEM_CMD_ACK;
				temp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
				if(temp==1)	{//�ر�
					IO_FOG_CTRL = 0;
				}else if(temp==0)	{//����
					IO_FOG_CTRL = 1;
				}else	{
					*pAck = MSG_SYSTEM_CMD_NAK;
				}							
				break;
		}
		case CMD_SysReset://0x19 ����
		{
				*pAck = MSG_SYSTEM_CMD_ACK;
				temp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
				if(temp==0)	{
					msg[0].Src = MSG_SYSTEM_RESTART;
					OSMboxPost(sysMonitor.Mbox, &msg[0]);
				}else if(temp==1)	{
				}else	{
					*pAck = MSG_SYSTEM_CMD_NAK;
				}			
				break;
		}
		default:
			sys_status.online_state = DEF_False;
			*pAck = MSG_SYSTEM_CMD_NAK;
			break;
	}
	return ret;
}