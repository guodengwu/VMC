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
    /*case CMD_ReadSysStatus://读取机器状态
			data_buf[len++] = motor.status.errCode;//故障码
			data_buf[len++] = motor.status.ship_status;//出货状态
			data_buf[len++] = sys_status.inside_temp;//箱内温度
			msg[1].Data = data_buf;
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
		}*/
		
		case CMD_NotifyShip://0x14 通知出货
		{
			u8 row,col;
		
			temp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);//行 货盘
			row= (temp&0xf0)>>4;//行 货盘
			col= temp&0x0f;//列 货道
			//param = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);//参数
			//timeout = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx)/10;//运行时间 s
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
		case CMD_ReplyOnlineStatus://15 回复联网状态
			break;
		case CMD_ModifyTempParm://16 修改温度参数
		{
			
			*pAck = MSG_SYSTEM_CMD_ACK;
			break;
		}
		case CMD_LightOnOff://0x17 灯开关
			break;
		case CMD_FogOnOff://0x18 除雾开关
			break;
		case CMD_SysReset://0x19 重启
			break;
		default:
			*pAck = MSG_SYSTEM_CMD_NAK;
			break;
	}
	return ret;
}