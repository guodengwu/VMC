#include "protocol.h"
#include "motor.h"
#include "app_ship.h"
#include "app_sysmonitor.h"

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
			motor.row= (temp&0xf0)>>4;//行 货盘
			motor.col= temp&0x0f;//列 货道
			//param = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);//参数
			//timeout = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx)/10;//运行时间 s
			len = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
			if(len>0&&len<IMEI_LEN)	{
				sys_status.pIMEI->len = len;
				memcpy(sys_status.pIMEI->dat, pUsart->rx_buf+pUsart->rx_idx, len);//获取订单号
				msg[0].Src = MSG_START_SHIP;
				OSMboxPost(appShip.MBox, &msg[0]);//启动出货控制任务
				*pAck = MSG_SYSTEM_CMD_ACK;
			}else	{
				*pAck = MSG_SYSTEM_CMD_NAK;
			}
			break;
		}
		case CMD_ReplyOnlineStatus://15 回复联网状态
		{
			temp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
			if(temp==0)	{
					sys_status.online_state = DEF_True;
					BEEP=1;
			}else if(temp==1)	{
					sys_status.online_state = DEF_False;
			}
			break;
		}
		case CMD_ModifyTempParm://16 修改温度参数
		{
			*pAck = MSG_SYSTEM_CMD_ACK;
			temp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
			if(temp==1)	{
					RELAY = 0;
			}else if(temp==0)	{
					RELAY = 1;
			}else	{
					*pAck = MSG_SYSTEM_CMD_NAK;
				}			
			sys_status.tTemp = UsartRxGetINT16U(pUsart->rx_buf,&pUsart->rx_idx);
			
			break;
		}
		case CMD_LightOnOff://0x17 灯开关
		{		
			*pAck = MSG_SYSTEM_CMD_ACK;
			temp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
			if(temp==1)	{//关闭灯光
					sys_status.light_ctrl_enable = DEF_False;
			}else if(temp==0)	{//开启灯光控制，灯开关由光敏电阻决定
					sys_status.light_ctrl_enable = DEF_True;
			}else	{
					*pAck = MSG_SYSTEM_CMD_NAK;
				}			
			break;
		}
		case CMD_FogOnOff://0x18 除雾开关
		{
				*pAck = MSG_SYSTEM_CMD_ACK;
				temp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
				if(temp==1)	{//关闭
					FOG_CTRL = 0;
				}else if(temp==0)	{//开启
					FOG_CTRL = 1;
				}else	{
					*pAck = MSG_SYSTEM_CMD_NAK;
				}							
				break;
			}
		case CMD_SysReset://0x19 重启
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
			//*pAck = MSG_SYSTEM_CMD_NAK;
			break;
	}
	return ret;
}