#include "protocol.h"
#include "motor.h"

static u8 data_buf[10];
void protocol_process(usart_t *pUsart,message_pkt_t msg[2])
{
	u8 cmd,len=0,param;
	
  cmd = msg[0].Cmd;
  switch (cmd)
  {
    case CMD_ReadSysStatus://��ȡ����״̬
			data_buf[len++] = motor.status.errCode;//������
			data_buf[len++] = motor.status.ship_status;//����״̬
			data_buf[len++] = sys_status.inside_temp;//�����¶�
			msg[1].Data    = data_buf;
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
		}
		case CMD_ClearMoveResult://������н��
		{
			msg[1].dLen = 0;
			OSMboxPost(pUsart->mbox, &msg[1]);
			break;
		}
		default:
			break;
	}
}