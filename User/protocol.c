#include "protocol.h"
#include "motor.h"

static u8 data_buf[10];
void protocol_process(usart_t *pUsart,message_pkt_t msg[2])
{
	u8 cmd,len=0,param;
	
  cmd = msg[0].Cmd;
  switch (cmd)
  {
    case CMD_ReadSysStatus://读取机器状态
			data_buf[len++] = motor.status.errCode;//故障码
			data_buf[len++] = motor.status.ship_status;//出货状态
			data_buf[len++] = sys_status.inside_temp;//箱内温度
			msg[1].Data    = data_buf;
			msg[1].dLen = len;
			OSMboxPost(pUsart->mbox, &msg[1]);
			break;
		case CMD_MotorMoveOneCircle://出货电机转一圈
		{
			u8 row,col,timeout=0;
			
			row= UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);//行 货盘
			col= UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);//列 货道
			param = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);//参数
			//timeout = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx)/10;//运行时间 s
			start_motor(row, col, param);
			msg[1].dLen = 0;
			OSMboxPost(pUsart->mbox, &msg[1]);	
			break;
		}
		case CMD_ClearMoveResult://清除运行结果
		{
			msg[1].dLen = 0;
			OSMboxPost(pUsart->mbox, &msg[1]);
			break;
		}
		default:
			break;
	}
}