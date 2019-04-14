#include "protocol.h"
#include "motor.h"

static u8 data_buf[10];
u8 protocol_process(usart_t *pUsart,message_pkt_t msg[2], u8 *pAck)
{
	u8 cmd,len=0,param,temp;
	u8 ret=MSG_FEEDBACK_DISABLE;
	
  cmd = msg[0].Cmd;
	*pAck = MSG_SYSTEM_CMD_NONE;
  switch (cmd)
  {
    /*case CMD_ReadSysStatus://��ȡ����״̬
			data_buf[len++] = motor.status.errCode;//������
			data_buf[len++] = motor.status.ship_status;//����״̬
			data_buf[len++] = sys_status.inside_temp;//�����¶�
			msg[1].Data = data_buf;
			msg[1].dLen = len;
			OSMboxPost(pUsart->mbox, &msg[1]);
			break;
		case CMD_MotorMoveOneCircle://�������תһȦ
		{
			u8 row,col,timeout=0;
			
			row= UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);//�� ����
			col= UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);//�� ����
			param = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);//����
			//timeout = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx)/10;//����ʱ�� s
			start_motor(row, col, param);
			msg[1].dLen = 0;
			OSMboxPost(pUsart->mbox, &msg[1]);	
			break;
		}*/
		
		case CMD_NotifyShip://0x14 ֪ͨ����
		{
			u8 row,col;
		
			temp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);//�� ����
			row= (temp&0xf0)>>4;//�� ����
			col= temp&0x0f;//�� ����
			//param = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);//����
			//timeout = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx)/10;//����ʱ�� s
			len = pUsart->rx_len-pUsart->rx_idx-IG_CMDANDSN_LEN;
			if(len>0&&len<IMEI_LEN)	{
				memcpy(sys_status.IMEI,pUsart->rx_buf+pUsart->rx_idx,len);
				start_motor(row, col, param);
				*pAck = MSG_SYSTEM_CMD_ACK;
			}else	{
				*pAck = MSG_SYSTEM_CMD_NAK;
			}
			break;
		}
		case CMD_ReplyOnlineStatus://15 �ظ�����״̬
			break;
		case CMD_ModifyTempParm://16 �޸��¶Ȳ���
		{
			
			*pAck = MSG_SYSTEM_CMD_ACK;
			break;
		}
		case CMD_LightOnOff://0x17 �ƿ���
			break;
		case CMD_FogOnOff://0x18 ������
			break;
		case CMD_SysReset://0x19 ����
			break;
		default:
			*pAck = MSG_SYSTEM_CMD_NAK;
			break;
	}
	return ret;
}