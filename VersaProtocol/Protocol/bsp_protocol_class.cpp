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
/// Seasky ͨ�ſ� ����Win-Api
/// </summary>
namespace vSeaskyProtocol
{
#endif
	/// <summary>
	/// ���û�������
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
	/// ���������ݵĴ�С�˲���
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
	/// ��ʼ��Э�������ڴ�
	/// </summary>
	/// <param name="uTxLen">pTxProtocol��uint32_t���ݳ���</param>
	/// <param name="uRxLen">pRxProtocol��uint32_t���ݳ���</param>
	void vSeaskyPort::ProtocolInit(uint16_t uTxLen, uint16_t uRxLen)
	{
		ProtocolAutoInitTx(uTxLen);
		ProtocolAutoInitRx(uRxLen);
	}

	/// <summary>
	/// ͨ����̬ Data(uint32_t) ���ȼ��� ������Buffer(uint8_t) �ĳ���
	/// </summary>
	/// <param name="uLen"></param>
	/// <returns></returns>
	uint16_t vSeaskyPort::ProtocolCalcLen(uint16_t uLen)
	{
		return get_protocol_size(uLen);
	}

	/// <summary>
	/// �Զ���ʼ��Tx���趯̬�ڴ棬uLen��С��128���ڲ������ڴ棬���ⲿ�й��޷�����
	/// </summary>
	/// <param name="uLen">pTxProtocol��uint32_t���ݳ���</param>
	void vSeaskyPort::ProtocolAutoInitTx(uint16_t uLen)
	{
		pTxProtocol->frame_st.frame_user.cmd_data.pData = NULL;
		pTxProtocol->message_st.pData = NULL;
		init_protocol(pTxProtocol, uLen);
	}

	/// <summary>
	/// �Զ���ʼ��Rx���趯̬�ڴ棬uLen��С��128���ڲ������ڴ棬���ⲿ�й��޷�����
	/// </summary>
	/// <param name="uLen">pRxProtocol��uint32_t���ݳ���</param>
	void vSeaskyPort::ProtocolAutoInitRx(uint16_t uLen)
	{
		pRxProtocol->frame_st.frame_user.cmd_data.pData = NULL;
		pRxProtocol->message_st.pData = NULL;
		init_protocol(pRxProtocol, uLen);
	}

	/// <summary>
	/// ��ȡTx��̬�ڴ��ַ�ͳ��ȣ����س���Ϊ pRxData���ȣ�pRxBuffer����Ϊ uMaxLen*4+12
	/// </summary>
	/// <param name="pTxData"></param>
	/// <param name="pTxBuffer"></param>
	/// <param name="uMaxLen">pTxData�����ݳ���</param>
	void vSeaskyPort::ProtocolTxPointGet(uint32_t* pTxData, uint8_t* pTxBuffer, uint16_t& uMaxLen)
	{
		pTxData = &pTxProtocol->frame_st.frame_user.cmd_data.pData[0];
		pTxBuffer = &pTxProtocol->message_st.pData[0];
		uMaxLen = pTxProtocol->frame_st.frame_user.cmd_data.max_data_len;
	}

	/// <summary>
	/// ��ȡRx��̬�ڴ��ַ�ͳ��ȣ����س���Ϊ pRxData���ȣ�pRxBuffer����Ϊ uMaxLen*4+12
	/// </summary>
	/// <param name="pRxData"></param>
	/// <param name="pRxBuffer"></param>
	/// <param name="uMaxLen">pRxData�����ݳ���</param>
	void vSeaskyPort::ProtocolRxPointGet(uint32_t* pRxData, uint8_t* pRxBuffer, uint16_t& uMaxLen)
	{
		pRxData = &pRxProtocol->frame_st.frame_user.cmd_data.pData[0];
		pRxBuffer = &pRxProtocol->message_st.pData[0];
		uMaxLen = pRxProtocol->frame_st.frame_user.cmd_data.max_data_len;
	}

	/// <summary>
	/// ��ʼ��Э��Tx�����ڴ棬�ⲿ���䣬uLen��С��128
	/// </summary>
	/// <param name="pTxData">Ԥ�ȷ�����ڴ��ַ</param>
	/// <param name="pTxBuffer">Ԥ�ȷ�����ڴ��ַ</param>
	/// <param name="uLen">pTxData�����ݳ���</param>
	void vSeaskyPort::ProtocolInitTx(uint32_t* pTxData, uint8_t* pTxBuffer, uint16_t uLen)
	{
		pTxProtocol->frame_st.frame_user.cmd_data.pData = NULL;
		pTxProtocol->message_st.pData = NULL;
		init_protocol_pointer(pTxProtocol, pTxData, pTxBuffer, uLen);
	}

	/// <summary>
	/// ��ʼ��Э��Rx�����ڴ�,�ⲿ���䣬uLen��С��128
	/// </summary>
	/// <param name="pRxData">Ԥ�ȷ�����ڴ��ַ</param>
	/// <param name="pRxBuffer">Ԥ�ȷ�����ڴ��ַ</param>
	/// <param name="uLen">pRxData�����ݳ���</param>
	void vSeaskyPort::ProtocolInitRx(uint32_t* pRxData, uint8_t* pRxBuffer, uint16_t uLen)
	{
		pRxProtocol->frame_st.frame_user.cmd_data.pData = NULL;
		pRxProtocol->message_st.pData = NULL;
		init_protocol_pointer(pRxProtocol, pRxData, pRxBuffer, uLen);
	}

#ifdef CLR_CONFIG
	/// <summary>
	/// ͬ����ʽ�򿪴���
	/// </summary>
	/// <param name="com_num">���ں�</param>
	/// <param name="baud_rate">������</param>
	/// <param name="parity">��żУ��λ</param>
	/// <param name="byte_size">����λ</param>
	/// <param name="stop_bits">ֹͣλ</param>
	/// <returns></returns>
	bool vSeaskyPort::vSerialOpen(uint32_t com_num, uint32_t baud_rate, uint32_t parity, uint32_t byte_size, uint32_t stop_bits)
	{
		//�йܲ�֧��string
		string serial_name = "";
		//Win Api�޷�ֱ��֧�ִ���10��COM��
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
	/// �رմ���
	/// </summary>
	bool vSeaskyPort::vSerialClose()
	{
		vSerialIsOpenIt = false;
		return vSerialPortClass->closeSerial();
	}

	/// <summary>
	/// �жϴ����Ƿ��
	/// </summary>
	/// <returns>��ȷ����Ϊture�����󷵻�Ϊfalse</returns>
	bool vSeaskyPort::vSerialIsOpen()
	{
		vSerialIsOpenIt = vSerialPortClass->isOpened();
		return vSerialIsOpenIt;
	}

	/// <summary>
	/// ���������
	/// </summary>
	void vSeaskyPort::vSerialClearBuffer()
	{
		vSerialPortClass->purgeBuff(PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_RXABORT);
	}

	/// <summary>
	/// �õ����һ��ʧ�ܵĴ�����Ϣ
	/// </summary>
	/// <returns></returns>
	uint8_t vSeaskyPort::vSerialGetLastError()
	{
		return vSerialPortClass->getSerialLastError();
	}

	/// <summary>
	/// ���ú�������һ���������߳��У���ʵ�ִ�����Ϣ�Ľ��մ���
	/// </summary>
	void vSeaskyPort::vSerialReceiveTask()
	{
		int Ret = -1;
		uint8_t  tRxBuffer[TX_RX_BUFFER_SIZE] = { 0 };
		static uint32_t curr_read_len = 0;		//��ǰ��������
		static uint32_t last_len = 0;			//��һ�δ���ȡ����
		static uint32_t total_read_len = 0;     //���ڻ����������ݳ���
		static uint32_t reality_read_len = 0;   //����������Ч����
		static uint32_t read_pos = 0;			//��ȡλ��
		static uint32_t parse_pos = 0;          //���ݽ���ָ��
		static uint32_t ready_parse_len = 0;	//Ԥ�������ݳ���
		uint32_t ready_read_len = 0;			//ʵ�ʶ�ȡ����
		static double start_time = GetTickCount();
		static double read_time = GetTickCount();
		static double end_time = GetTickCount();
		while (vSerialIsOpenIt == true)
		{
			if (vSerialPortClass->isOpened())
			{
				total_read_len = 0;
				// ��ȡ��ǰ����
				total_read_len = vSerialPortClass->getSerialReceiveLen();
				if (total_read_len > last_len)
				{
					curr_read_len = total_read_len - last_len;
					printf("curr_read_len:%d\n", curr_read_len);
					start_time = GetTickCount();
				}
				end_time = GetTickCount();
				// ������һ�ζ������ݵ�ʱ��
				if (((end_time - start_time) >= 16) || ((total_read_len + read_pos) >= 12))
				{
					//�����Ƿ������ݶ���Ҫ����readData ������������־
					if (total_read_len + read_pos < TX_RX_BUFFER_SIZE)
					{
						//���ݳ��ȱ�Ȼ�� 12+4n
						ready_read_len = 4 * ((int)((total_read_len - 12) / 4)) + 12 - read_pos;
						reality_read_len = vSerialPortClass->readData(&((char*)tRxBuffer)[read_pos], ready_read_len) + read_pos;
					}
					else
					{
						ready_read_len = (TX_RX_BUFFER_SIZE - 12) / 4 + 12 - read_pos;
						reality_read_len = vSerialPortClass->readData(&((char*)tRxBuffer)[read_pos], ready_read_len) + read_pos;
					}
					//��ȡʣ�����ݳ���
					last_len = vSerialPortClass->getSerialReceiveLen();
					// ����
					if (reality_read_len > 0)
					{
						parse_pos = 0;
						printf("Data ready to be parsed [%d],Currently added[%d]!\n", reality_read_len, reality_read_len - read_pos);
						bsp_debug_c(LOG_LEVEL_DEBUG, "Data ready to be parsed [%d],Currently added[%d]!\n", reality_read_len, reality_read_len - read_pos);
						//�ȴ���������������
						while ((parse_pos < reality_read_len))
						{
							//(reality_read_len - parse_pos) Ϊʣ��δ�������ݳ���
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
							// ���ݳ��Ȳ���
							if (Ret == PROTOCOL_RESULT_INSUFFICIENT_OF_LEN)
							{
								// ʣ�����ݲ������������
								memcpy(&pRxProtocol->message_st.pData[0], &tRxBuffer[parse_pos], ready_parse_len);
								memcpy(&tRxBuffer[0], &pRxProtocol->message_st.pData[0], ready_parse_len);
								read_pos = ready_parse_len;
								bsp_debug_c(LOG_LEVEL_WARN_, "The length to be parsed is not enough!\n");
								bsp_debug_c(LOG_LEVEL_WARN_, "The length to be parsed is [%d]!\n", read_pos);
								break;
							}
							else if ((Ret == PROTOCOL_RESULT_CHECK_HEAD_ERR)||(Ret == PROTOCOL_RESULT_CHECK_FRAME_ERR))
							{
								// ʣ����������ݳ�������������������������Ȼ����ʧ�ܣ���Ϊ��������
								if ((reality_read_len - parse_pos) > pRxProtocol->message_st.max_data_len)
								{
									//������ջ���������
									vSerialPortClass->clearRxBuffer();
									total_read_len = vSerialPortClass->getSerialReceiveLen();
									//��������ʧ�ܣ���ջ���������
									while (total_read_len != 0)
									{
										//�����ջ������Ƿ��Ѿ�Ϊ�գ������Ϊ������ö�ȡ��ʽ���
										if (total_read_len < TX_RX_BUFFER_SIZE)
										{
											//���ݳ��ȱ�Ȼ�� 12+4n
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
									// ֱ����Ϊ�������
									bsp_debug_c(LOG_LEVEL_ERROR, "ReceiveTask Parse Err!\n");
									bsp_debug_c(LOG_LEVEL_ERROR, "Clear RaedBuffer!\n");
									parse_pos = total_read_len;
									read_pos = 0;
									break;
								}
							}
							else if(Ret == PROTOCOL_RESULT_OK)
							{
								// �������ݳɹ�
								bsp_debug_c(LOG_LEVEL_DEBUG, "Parse the data successfully [%d]!\n", reality_read_len);
								vReceiveCallBack();
							}
							//�ƶ�����һ��δ��������
							parse_pos += pRxProtocol->message_st.data_len;
							if (parse_pos >= reality_read_len)
							{

								bsp_debug_c(LOG_LEVEL_DEBUG, "To be parsed data parsing is complete[%d]!\n", reality_read_len);
								read_pos = 0;
								break;
							}
							//�������ݳ�ʱ
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
	/// Э����㣬����������
	/// </summary>
	void vSeaskyPort::ProtocolTransmit(uint16_t equipment_type, uint16_t equipment_id, uint16_t data_id, uint32_t* pData, uint16_t data_len)
	{
		int ret = -1;
		/*�޸Ĵ����͵���Ϣ*/
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
	/// �������ݴ�������ж�
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
	/// ���յ�����Ϣ��Ϣ��ӡ����
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
	/// ���ý������ݴ�����ɻص�����
	/// </summary>
	/// <param name="pFun">�������ݴ�����ɻص�����</param>
	void vSeaskyPort::vSerialSetReceivCallbackFun(pReceivePointer^ pFun)
	{
		pReceiveCallbackFun = pFun;
		pSerialReceiveCppPointerIsEnable = true;
		bsp_debug_c(LOG_LEVEL_DEBUG, "SetCanReceivCallbackFun\n");
	};

	/// <summary>
	/// ���õ�����Ϣ�ȼ��͵�����Ϣ�ص���ʾ����
	/// </summary>
	/// <param name="debugLevel">������Ϣ�ȼ�</param>
	/// <param name="pFun">������Ϣ�ص���ʾ����</param>
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
