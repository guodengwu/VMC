#include "protocol.h"
#include "motor.h"
#include "app_ship.h"
#include "app_sysmonitor.h"

static u8 data_buf[100];
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
			if(len>0&&len<IMEI_LEN)	{	//�����ų�����ȷ 			
				if(memcmp(sys_status.pIMEI->dat, pUsart->rx_buf+pUsart->rx_idx, len)==0)	{//���ζ��������ϴ���ͬ �ܾ�����	
					*pAck = MSG_SYSTEM_CMD_NAK;					
				}else	{
						if(appShip.state == SHIP_STATE_IDLE)	{//���� ͬ�����
								memcpy(sys_status.pIMEI->dat, pUsart->rx_buf+pUsart->rx_idx, len);//��ȡ������
								sys_status.pIMEI->len = len;
								msg[0].Src = MSG_START_SHIP;
								OSMboxPost(appShip.MBox, &msg[0]);//����������������
								*pAck = MSG_SYSTEM_CMD_ACK;
								break;
						}else	{//��ǰæ ֪ͨ������� æ								
								data_buf[0] = (motor.row<<4)|motor.col;	
								data_buf[1] = SHIP_BUSY;
								memcpy(data_buf+2, sys_status.pIMEI->dat, sys_status.pIMEI->len);//����������
								msg[0].Src = USART_MSG_RX_TASK;
								msg[0].Data = data_buf;
								msg[0].dLen = 2 + sys_status.pIMEI->len;
								OSQPost(usart.Str_Q, &msg[0]);
						}
				}
			}
			else	{//�����ų��Ȳ��� �ܾ�
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
		case CMD_MOTOR_CTRL_TYPE://0x12 ���ֹͣ���Ʒ�ʽ�л�
		{
			*pAck = MSG_SYSTEM_CMD_ACK;
			temp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
			if(temp==1)	{//�������Ƶ��ֹͣ
				motor.CtrlType = MOTOR_CTRL_TYPE_HOLE;
			}else if(temp==0)	{//������Ƶ��ֹͣ
				motor.CtrlType = MOTOR_CTRL_TYPE_IR;
			}else	{
				*pAck = MSG_SYSTEM_CMD_NAK;
			}
				break;
		}
		case CMD_HUASHUANG_CTRL://0x13 ��˪ʹ�ܿ���
		{
			*pAck = MSG_SYSTEM_CMD_ACK;
			temp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
			if(temp==1)	{//�رջ�˪
				HuaShuangCtrl.enable = DEF_False;
				HuaShuangCtrl.run_time = 0;
				HuaShuangCtrl.run_interval = 0;
			}else if(temp==0)	{//ʹ�ܻ�˪
				HuaShuangCtrl.enable = DEF_Enable;
				HuaShuangCtrl.run_time = UsartRxGetINT16U(pUsart->rx_buf,&pUsart->rx_idx);
				HuaShuangCtrl.run_interval = UsartRxGetINT16U(pUsart->rx_buf,&pUsart->rx_idx);
				HuaShuangCtrl.run_timecnt = 0;
				HuaShuangCtrl.run_intercnt = 0;
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