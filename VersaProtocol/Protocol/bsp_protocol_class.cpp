#include  <stdlib.h>           
#include  <string.h> 
#include  <iostream>
#include  <thread>
#include  "bsp_protocol_class.h"

#ifdef CLR_CONFIG
uint8_t debug_level_config;
string debug_level_str[5] =
{
	"[ComCanClass.DEBUG]:",
	"[ComCanClass.INFO_]:",
	"[ComCanClass.WARN_]:"
	"[ComCanClass.ERROR]:",
	"[ComCanClass.FATAL]:"
};
string debug_info;
#define TX_RX_BUFFER_SIZE 1024
#endif

#ifdef CLR_CONFIG

/// <summary>
/// Seasky 通信库 基于Win-Api
/// </summary>
namespace vSeaskyProtocol
{
#endif
	/// <summary>
	/// 设置缓冲区域
	/// </summary>
	vSeaskyPort::vSeaskyPort()
	{
#ifdef CLR_CONFIG
		vSerialPortClass->setBufferSize(TX_RX_BUFFER_SIZE, TX_RX_BUFFER_SIZE);
#endif
	}
	vSeaskyPort::~vSeaskyPort()
	{
#ifdef CLR_CONFIG
		vSerialPortClass->closeSerial();
		vSerialIsOpenIt = false;
#endif
	}



	/// <summary>
	/// 共用体数据的大小端测试
	/// </summary>
	/// <param name="uLen"></param>
	bool vSeaskyPort::GetStorageMethodIsSmall(void)
	{
		int ret = -1;
		data_union data_union_t[2];
		data_union_t[0].data_u32 = 1;
		data_union_t[1].data_u32 = 2;
		if (data_union_t[0].data_u8[0] == 1)
		{
			//            bsp_debug_c(LOG_LEVEL_INFO_,"little endian storage!\n");
			return true;
		}
		else
		{
			//            bsp_debug_c(LOG_LEVEL_INFO_,"big endian storage!\n");
			return false;
		}
	}


	/// <summary>
	/// 初始化协议所需内存
	/// </summary>
	/// <param name="uTxLen">pTxProtocol的uint32_t数据长度</param>
	/// <param name="uRxLen">pRxProtocol的uint32_t数据长度</param>
	void vSeaskyPort::ProtocolInit(uint16_t uTxLen, uint16_t uRxLen)
	{
		ProtocolAutoInitTx(uTxLen);
		ProtocolAutoInitRx(uRxLen);
	}

	/// <summary>
	/// 通过动态 Data(uint32_t) 长度计算 总数居Buffer(uint8_t) 的长度
	/// </summary>
	/// <param name="uLen"></param>
	/// <returns></returns>
	uint16_t vSeaskyPort::ProtocolCalcLen(uint16_t uLen)
	{
		return get_protocol_size(uLen);
	}

	/// <summary>
	/// 自动初始化Tx所需动态内存，uLen需小于128，内部分配内存，在外部托管无法访问
	/// </summary>
	/// <param name="uLen">pTxProtocol的uint32_t数据长度</param>
	void vSeaskyPort::ProtocolAutoInitTx(uint16_t uLen)
	{
		pTxProtocol->frame_st.frame_user.cmd_data.pData = NULL;
		pTxProtocol->message_st.pData = NULL;
		init_protocol(pTxProtocol, uLen);
	}

	/// <summary>
	/// 自动初始化Rx所需动态内存，uLen需小于128，内部分配内存，在外部托管无法访问
	/// </summary>
	/// <param name="uLen">pRxProtocol的uint32_t数据长度</param>
	void vSeaskyPort::ProtocolAutoInitRx(uint16_t uLen)
	{
		pRxProtocol->frame_st.frame_user.cmd_data.pData = NULL;
		pRxProtocol->message_st.pData = NULL;
		init_protocol(pRxProtocol, uLen);
	}

	/// <summary>
	/// 获取Tx动态内存地址和长度，返回长度为 pRxData长度，pRxBuffer长度为 uMaxLen*4+12
	/// </summary>
	/// <param name="pTxData"></param>
	/// <param name="pTxBuffer"></param>
	/// <param name="uMaxLen">pTxData的数据长度</param>
	void vSeaskyPort::ProtocolTxPointGet(uint32_t* pTxData, uint8_t* pTxBuffer, uint16_t& uMaxLen)
	{
		pTxData = &pTxProtocol->frame_st.frame_user.cmd_data.pData[0];
		pTxBuffer = &pTxProtocol->message_st.pData[0];
		uMaxLen = pTxProtocol->frame_st.frame_user.cmd_data.max_data_len;
	}

	/// <summary>
	/// 获取Rx动态内存地址和长度，返回长度为 pRxData长度，pRxBuffer长度为 uMaxLen*4+12
	/// </summary>
	/// <param name="pRxData"></param>
	/// <param name="pRxBuffer"></param>
	/// <param name="uMaxLen">pRxData的数据长度</param>
	void vSeaskyPort::ProtocolRxPointGet(uint32_t* pRxData, uint8_t* pRxBuffer, uint16_t& uMaxLen)
	{
		pRxData = &pRxProtocol->frame_st.frame_user.cmd_data.pData[0];
		pRxBuffer = &pRxProtocol->message_st.pData[0];
		uMaxLen = pRxProtocol->frame_st.frame_user.cmd_data.max_data_len;
	}

	/// <summary>
	/// 初始化协议Tx所需内存，外部分配，uLen需小于128
	/// </summary>
	/// <param name="pTxData">预先分配的内存地址</param>
	/// <param name="pTxBuffer">预先分配的内存地址</param>
	/// <param name="uLen">pTxData的数据长度</param>
	void vSeaskyPort::ProtocolInitTx(uint32_t* pTxData, uint8_t* pTxBuffer, uint16_t uLen)
	{
		pTxProtocol->frame_st.frame_user.cmd_data.pData = NULL;
		pTxProtocol->message_st.pData = NULL;
		init_protocol_pointer(pTxProtocol, pTxData, pTxBuffer, uLen);
	}

	/// <summary>
	/// 初始化协议Rx所需内存,外部分配，uLen需小于128
	/// </summary>
	/// <param name="pRxData">预先分配的内存地址</param>
	/// <param name="pRxBuffer">预先分配的内存地址</param>
	/// <param name="uLen">pRxData的数据长度</param>
	void vSeaskyPort::ProtocolInitRx(uint32_t* pRxData, uint8_t* pRxBuffer, uint16_t uLen)
	{
		pRxProtocol->frame_st.frame_user.cmd_data.pData = NULL;
		pRxProtocol->message_st.pData = NULL;
		init_protocol_pointer(pRxProtocol, pRxData, pRxBuffer, uLen);
	}

#ifdef CLR_CONFIG
	/// <summary>
	/// 同步方式打开串口
	/// </summary>
	/// <param name="com_num">串口号</param>
	/// <param name="baud_rate">波特率</param>
	/// <param name="parity">奇偶校验位</param>
	/// <param name="byte_size">数据位</param>
	/// <param name="stop_bits">停止位</param>
	/// <returns></returns>
	bool vSeaskyPort::vSerialOpen(uint32_t com_num, uint32_t baud_rate, uint32_t parity, uint32_t byte_size, uint32_t stop_bits)
	{
		//托管不支持string
		string serial_name = "";
		//Win Api无法直接支持大于10的COM口
		if (com_num < 10)
		{
			serial_name += "COM";
		}
		else
		{
			serial_name += "\\\\.\\COM";
		}
		serial_name += std::to_string(com_num);
		vSerialIsOpenIt = vSerialPortClass->openSyn(serial_name, baud_rate, parity, byte_size, stop_bits);
		vSerialIsOpenIt = vSerialPortClass->isOpened();
		return vSerialIsOpenIt;
	}

	/// <summary>
	/// 关闭串口
	/// </summary>
	bool vSeaskyPort::vSerialClose()
	{
		vSerialIsOpenIt = false;
		return vSerialPortClass->closeSerial();
	}

	/// <summary>
	/// 判断串口是否打开
	/// </summary>
	/// <returns>正确返回为ture，错误返回为false</returns>
	bool vSeaskyPort::vSerialIsOpen()
	{
		vSerialIsOpenIt = vSerialPortClass->isOpened();
		return vSerialIsOpenIt;
	}

	/// <summary>
	/// 清除缓冲区
	/// </summary>
	void vSeaskyPort::vSerialClearBuffer()
	{
		vSerialPortClass->purgeBuff(PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_RXABORT);
	}

	/// <summary>
	/// 得到最后一次失败的错误信息
	/// </summary>
	/// <returns></returns>
	uint8_t vSeaskyPort::vSerialGetLastError()
	{
		return vSerialPortClass->getSerialLastError();
	}

	/// <summary>
	/// 将该函数放在一个独立的线程中，以实现串口消息的接收处理
	/// </summary>
	void vSeaskyPort::vSerialReceiveTask()
	{
		int Ret = -1;
		uint8_t  tRxBuffer[TX_RX_BUFFER_SIZE] = { 0 };
		static uint32_t curr_read_len = 0;		//当前新增长度
		static uint32_t last_len = 0;			//上一次待读取长度
		static uint32_t total_read_len = 0;     //串口缓冲区总数据长度
		static uint32_t reality_read_len = 0;   //处理缓冲区有效长度
		static uint32_t read_pos = 0;			//读取位置
		static uint32_t parse_pos = 0;          //数据解析指针
		static uint32_t ready_parse_len = 0;	//预解析数据长度
		uint32_t ready_read_len = 0;			//实际读取长度
		static double start_time = GetTickCount();
		static double read_time = GetTickCount();
		static double end_time = GetTickCount();
		while (vSerialIsOpenIt == true)
		{
			if (vSerialPortClass->isOpened())
			{
				total_read_len = 0;
				// 读取当前数据
				total_read_len = vSerialPortClass->getSerialReceiveLen();
				if (total_read_len > last_len)
				{
					curr_read_len = total_read_len - last_len;
					printf("curr_read_len:%d\n", curr_read_len);
					start_time = GetTickCount();
				}
				end_time = GetTickCount();
				// 距离上一次读到数据的时间
				if (((end_time - start_time) >= 16) || ((total_read_len + read_pos) >= 12))
				{
					//无论是否有数据都需要进行readData ，以清除错误标志
					if (total_read_len + read_pos < TX_RX_BUFFER_SIZE)
					{
						//数据长度必然是 12+4n
						ready_read_len = 4 * ((int)((total_read_len - 12) / 4)) + 12 - read_pos;
						reality_read_len = vSerialPortClass->readData(&((char*)tRxBuffer)[read_pos], ready_read_len) + read_pos;
					}
					else
					{
						ready_read_len = (TX_RX_BUFFER_SIZE - 12) / 4 + 12 - read_pos;
						reality_read_len = vSerialPortClass->readData(&((char*)tRxBuffer)[read_pos], ready_read_len) + read_pos;
					}
					//获取剩余数据长度
					last_len = vSerialPortClass->getSerialReceiveLen();
					// 本次
					if (reality_read_len > 0)
					{
						parse_pos = 0;
						printf("Data ready to be parsed [%d],Currently added[%d]!\n", reality_read_len, reality_read_len - read_pos);
						bsp_debug_c(LOG_LEVEL_DEBUG, "Data ready to be parsed [%d],Currently added[%d]!\n", reality_read_len, reality_read_len - read_pos);
						//等待解析完所有数据
						while ((parse_pos < reality_read_len))
						{
							//(reality_read_len - parse_pos) 为剩余未解析数据长度
							if ((reality_read_len - parse_pos) > pRxProtocol->message_st.max_data_len)
							{
								ready_parse_len = pRxProtocol->message_st.max_data_len;
							}
							else
							{
								ready_parse_len = reality_read_len - parse_pos;
							}
							memcpy(&pRxProtocol->message_st.pData[0], &tRxBuffer[parse_pos], ready_parse_len);
							Ret = parse_protocol(pRxProtocol, ready_parse_len);
							// 数据长度不足
							if (Ret == PROTOCOL_RESULT_INSUFFICIENT_OF_LEN)
							{
								// 剩余数据不满足解析长度
								memcpy(&pRxProtocol->message_st.pData[0], &tRxBuffer[parse_pos], ready_parse_len);
								memcpy(&tRxBuffer[0], &pRxProtocol->message_st.pData[0], ready_parse_len);
								read_pos = ready_parse_len;
								bsp_debug_c(LOG_LEVEL_WARN_, "The length to be parsed is not enough!\n");
								bsp_debug_c(LOG_LEVEL_WARN_, "The length to be parsed is [%d]!\n", read_pos);
								break;
							}
							else if ((Ret == PROTOCOL_RESULT_CHECK_HEAD_ERR)||(Ret == PROTOCOL_RESULT_CHECK_FRAME_ERR))
							{
								// 剩余待解析数据长度如果满足解析条件，但是仍然解析失败，认为数据有误
								if ((reality_read_len - parse_pos) > pRxProtocol->message_st.max_data_len)
								{
									//清除接收缓冲区数据
									vSerialPortClass->clearRxBuffer();
									total_read_len = vSerialPortClass->getSerialReceiveLen();
									//解析数据失败，清空缓冲区数据
									while (total_read_len != 0)
									{
										//检测接收缓冲区是否已经为空，如果不为空则采用读取方式清空
										if (total_read_len < TX_RX_BUFFER_SIZE)
										{
											//数据长度必然是 12+4n
											ready_read_len = (total_read_len - 12) / 4 + 12;
											reality_read_len = vSerialPortClass->readData(&((char*)tRxBuffer)[0], ready_read_len);
										}
										else
										{
											ready_read_len = (TX_RX_BUFFER_SIZE - 12) / 4 + 12;
											reality_read_len = vSerialPortClass->readData(&((char*)tRxBuffer)[0], ready_read_len);
										}
										total_read_len = vSerialPortClass->getSerialReceiveLen();
									}
									// 直接认为解析完成
									bsp_debug_c(LOG_LEVEL_ERROR, "ReceiveTask Parse Err!\n");
									bsp_debug_c(LOG_LEVEL_ERROR, "Clear RaedBuffer!\n");
									parse_pos = total_read_len;
									read_pos = 0;
									break;
								}
							}
							else if(Ret == PROTOCOL_RESULT_OK)
							{
								// 解析数据成功
								bsp_debug_c(LOG_LEVEL_DEBUG, "Parse the data successfully [%d]!\n", reality_read_len);
								vReceiveCallBack();
							}
							//移动到下一个未解析数据
							parse_pos += pRxProtocol->message_st.data_len;
							if (parse_pos >= reality_read_len)
							{

								bsp_debug_c(LOG_LEVEL_DEBUG, "To be parsed data parsing is complete[%d]!\n", reality_read_len);
								read_pos = 0;
								break;
							}
							//解析数据超时
							read_time = GetTickCount();
							if ((read_time - end_time) > 256)
							{
								bsp_debug_c(LOG_LEVEL_ERROR, "Parse Data Time Out [%f],parse_pos[%d],reality_read_len[%d]!\n", read_time - end_time, parse_pos, reality_read_len);
								parse_pos = reality_read_len;
								break;
							}
						}
						total_read_len = 0;
					}
				}
			}
			if (debug_info.size() > 0)
			{
				vSerialDebugPrintf(&debug_info[0]);
				debug_info.clear();
			}
		}
	}

	/// <summary>
	/// 协议计算，并发送数据
	/// </summary>
	void vSeaskyPort::ProtocolTransmit(uint16_t equipment_type, uint16_t equipment_id, uint16_t data_id, uint32_t* pData, uint16_t data_len)
	{
		int ret = -1;
		/*修改待发送的信息*/
		user_data_struct* pTxUser = &pTxProtocol->frame_st.frame_user;
		pTxUser->equipment_type = equipment_type;
		pTxUser->equipment_id = equipment_id;
		pTxUser->data_id = data_id;
		pTxUser->cmd_data.data_len = data_len;
		//memcpy(&pTxUser->cmd_data.pData[0], &pData[0], pTxUser->cmd_data.data_len* sizeof(uint32_t));
		memcpy(&pTxUser->cmd_data.pData[0], &pData[0], pTxUser->cmd_data.data_len * 4);
		ret = make_protocol(pTxProtocol);
		if (vSerialIsOpen())
		{
			vSerialPortClass->writeData((char*)pTxProtocol->message_st.pData, pTxProtocol->message_st.data_len);
		}
	}

	/// <summary>
	/// 接收数据处理完成中断
	/// </summary>
	void vSeaskyPort::vReceiveCallBack()
	{
		static uint16_t CanCmdId = 0;
		static uint32_t pRxData[24];
		user_data_struct* pRxUser = &pRxProtocol->frame_st.frame_user;
		//memcpy(&pRxData[0], &pRxUser->cmd_data.pData[0], pRxUser->cmd_data.data_len * sizeof(uint32_t));
		memcpy(&pRxData[0], &pRxUser->cmd_data.pData[0], pRxUser->cmd_data.data_len * 4);
		if (pSerialReceiveCppPointerIsEnable)
		{
			pReceiveCallbackFun(pRxUser->equipment_type, pRxUser->equipment_id, pRxUser->data_id, &pRxData[0], pRxUser->cmd_data.data_len);
		}
		else
		{
			vSerialDebugPrintf("Needs to be  SetCanReceivCallbackFun \n");
		}
	}



	/// <summary>
	/// 接收调试信息信息打印函数
	/// </summary>
	/// <param name="pStr"></param>
	void vSeaskyPort::vSerialDebugPrintf(char* pStr)
	{
		if (pSerialDebugIsEnable)
		{
			pDebugCallbackFun(pStr);
		}
	}



	/// <summary>
	/// 设置接收数据处理完成回调函数
	/// </summary>
	/// <param name="pFun">接收数据处理完成回调函数</param>
	void vSeaskyPort::vSerialSetReceivCallbackFun(pReceivePointer^ pFun)
	{
		pReceiveCallbackFun = pFun;
		pSerialReceiveCppPointerIsEnable = true;
		bsp_debug_c(LOG_LEVEL_DEBUG, "SetCanReceivCallbackFun\n");
	};

	/// <summary>
	/// 设置调试信息等级和调试信息回调显示函数
	/// </summary>
	/// <param name="debugLevel">调试信息等级</param>
	/// <param name="pFun">调试信息回调显示函数</param>
	void vSeaskyPort::vSerialSetDebugCallbackFun(uint8_t debugLevel, pDebugPointer^ pFun)
	{
		debug_level_config = debugLevel;
		pDebugCallbackFun = pFun;
		pSerialDebugIsEnable = true;
		debug_info.clear();
	}
#endif

#ifdef CLR_CONFIG
}
#endif

#ifdef CLR_CONFIG
void bsp_debug_c(uint8_t level, const char* Format, ...)
{
	string str;
	int ret;
	char debugBuffer[4096];
	if ((level & debug_level_config) != 0)
	{
		va_list aptr;
		va_start(aptr, Format);
		ret = vsprintf(debugBuffer, Format, aptr);
		va_end(aptr);
		switch (level)
		{
			case LOG_LEVEL_DEBUG:debug_info.append(debug_level_str[0]); break;
			case LOG_LEVEL_INFO_:debug_info.append(debug_level_str[1]); break;
			case LOG_LEVEL_WARN_:debug_info.append(debug_level_str[2]); break;
			case LOG_LEVEL_ERROR:debug_info.append(debug_level_str[3]); break;
			case LOG_LEVEL_FATAL:debug_info.append(debug_level_str[4]); break;
		}
		debug_info.append(debugBuffer);
	}
}
#endif
