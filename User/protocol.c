#include "protocol.h"

static u8 data_buf[10];
void protocol_process(usart_t *pUsart,message_pkt_t msg[2])
{
	u8 cmd,len=0;
	
  cmd = msg[0].Cmd;
  switch (cmd)
  {
    case CMD_ReadSysStatus://��ȡ����״̬
			data_buf[len++] = 0;
			data_buf[len++] = 0;
			data_buf[len++] = 0x1a;
			msg[1].Data    = data_buf;
			msg[1].dLen = len;
			OSMboxPost(pUsart->mbox, &msg[1]);
			break;
		case CMD_MotorMoveOneCircle://�������תһȦ
			
			break;
		case CMD_ClearMoveResult://������н��
			
			break;
		default:
			break;
	}
}