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
		case CMD_NotifyShip://0x14 通知出货
		{
//			u8 row,col;		
			temp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);//行 货盘
			motor.row= (temp&0xf0)>>4;//行 货盘
			motor.col= temp&0x0f;//列 货道
			//param = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);//参数
			//timeout = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx)/10;//运行时间 s
			len = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
			if(len>0&&len<IMEI_LEN)	{	//订单号长度正确 			
				if(memcmp(sys_status.pIMEI->dat, pUsart->rx_buf+pUsart->rx_idx, len)==0)	{//本次订单号与上次相同 拒绝出货	
					*pAck = MSG_SYSTEM_CMD_NAK;					
				}else	{
						if(appShip.state == SHIP_STATE_IDLE)	{//空闲 同意出货
								memcpy(sys_status.pIMEI->dat, pUsart->rx_buf+pUsart->rx_idx, len);//获取订单号
								sys_status.pIMEI->len = len;
								msg[1].Src = MSG_START_SHIP;
								OSMboxPost(appShip.MBox, &msg[1]);//启动出货控制任务
								*pAck = MSG_SYSTEM_CMD_ACK;
								break;
						}else	{//当前忙 通知出货结果 忙								
								data_buf[0] = (motor.row<<4)|motor.col;	
								data_buf[1] = SHIP_BUSY;
								memcpy(data_buf+2, sys_status.pIMEI->dat, sys_status.pIMEI->len);//拷贝订单号
								msg[1].Src = USART_MSG_RX_TASK;
								msg[1].Cmd = CMD_ReportShipResult;
								msg[1].Data = data_buf;
								msg[1].dLen = 2 + sys_status.pIMEI->len;
								OSMboxPost(usart.MBox, &msg[1]);
						}
				}
			}
			else	{//订单号长度不对 拒绝
				*pAck = MSG_SYSTEM_CMD_NAK;
			}
			break;
		}
		case CMD_ReplyOnlineStatus://15 回复联网状态
		{
			temp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
			if(temp==0)	{
					sys_status.online_state = DEF_True;
					msg[1].Src = MSG_SYS_ONLINE;
					OSMboxPost(sysMonitor.Mbox, &msg[1]);
					
			}else if(temp==1)	{
					sys_status.online_state = DEF_False;
			}
			break;
		}
		case CMD_ModifyTempParm://16 修改温度参数
		{
			s8 targetTemp;
			*pAck = MSG_SYSTEM_CMD_ACK;
			temp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
			if(temp==1)	{//关闭压缩机
				IO_RELAY = 0;
				//IO_HUASHUANG_CTRL = 0;
				sys_status.pTempCtrl->flag = DEF_False;//关闭温度控制
			}else if(temp==0)	{
				sys_status.pTempCtrl->flag = DEF_True;//开启温度控制
				targetTemp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);//控制目标温度
				sys_status.pTempCtrl->tInsideTempL = targetTemp;// - TEMP_RANGE;//计算控制范围
				sys_status.pTempCtrl->tInsideTempH = targetTemp + TEMP_RANGE;
			}else	{
				*pAck = MSG_SYSTEM_CMD_NAK;
			}									
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
					IO_FOG_CTRL = 0;
				}else if(temp==0)	{//开启
					IO_FOG_CTRL = 1;
				}else	{
					*pAck = MSG_SYSTEM_CMD_NAK;
				}							
				break;
		}
		case CMD_MOTOR_CTRL_TYPE://0x12 电机停止控制方式切换
		{
			*pAck = MSG_SYSTEM_CMD_ACK;
			temp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
			if(temp==1)	{//霍尔控制电机停止
				motor.CtrlType = MOTOR_CTRL_TYPE_HOLE;
			}else if(temp==0)	{//红外控制电机停止
				motor.CtrlType = MOTOR_CTRL_TYPE_IR;
			}else	{
				*pAck = MSG_SYSTEM_CMD_NAK;
			}
			break;
		}
		case CMD_HUASHUANG_CTRL://0x13 化霜使能控制
		{
			*pAck = MSG_SYSTEM_CMD_ACK;
			temp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
			if(temp==1)	{//关闭化霜
				HuaShuangCtrl.enable = DEF_False;
				HuaShuangCtrl.run_time = 0;
				HuaShuangCtrl.run_interval = 0;
				SysHuaShuangIOCtrl(DEF_OFF);//立即关闭化霜
			}else if(temp==0)	{//使能化霜
				HuaShuangCtrl.enable = DEF_Enable;
				HuaShuangCtrl.run_time = UsartRxGetINT16U(pUsart->rx_buf,&pUsart->rx_idx);
				HuaShuangCtrl.run_interval = UsartRxGetINT16U(pUsart->rx_buf,&pUsart->rx_idx);
				HuaShuangCtrl.run_timecnt = 0;
				HuaShuangCtrl.stop_timecnt = 0;
				//SysHuaShuangIOCtrl(DEF_ON);//立即启动化霜
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
					msg[1].Src = MSG_SYSTEM_RESTART;
					OSMboxPost(sysMonitor.Mbox, &msg[1]);
				}else if(temp==1)	{
				}else	{
					*pAck = MSG_SYSTEM_CMD_NAK;
				}
				break;
		}
		default:
			//sys_status.online_state = DEF_False;
			*pAck = MSG_SYSTEM_CMD_NAK;
			break;
	}
	return ret;
}