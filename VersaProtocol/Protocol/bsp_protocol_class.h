#ifndef _BSP_PROTOCOL_CLASS_H_
#define _BSP_PROTOCOL_CLASS_H_

#include "bsp_protocol.h"

#ifdef CLR_CONFIG
//CLR_CONFIG 用于混编，C++生成C# 可调用Dll
#include  <SerialPort/SerialPort.h>
#define PROTOCOL_CLASS_REF  public ref class
#define PROTOCOL_STRUCT_REF public ref struct
#define PROTOCOL_CPP_CLR_DEBUG
void bsp_debug_c(uint8_t level, const char* Format, ...);
#else
#define PROTOCOL_CLASS_REF		class
#define PROTOCOL_STRUCT_REF		struct
#endif// CLR_CONFIG


#ifdef CLR_CONFIG
/// <summary>
/// Seasky 通信库 基于Win-Api
/// </summary>
namespace vSeaskyProtocol
{
#endif

#ifdef CLR_CONFIG
	// 完整的Seasky协议数据返回
	public delegate void pReceivePointer(uint16_t equipment_type, uint16_t equipment_id, uint16_t data_id, uint32_t* pData, uint16_t data_len);
	public delegate void pDebugPointer(char* pStrError);
	public enum class COM_LOG_LEVEL
	{
		COM_LOG_LEVEL_DEBUG = (1 << 0),
		COM_LOG_LEVEL_INFO_ = (1 << 1),
		COM_LOG_LEVEL_WARN_ = (1 << 2),
		COM_LOG_LEVEL_ERROR = (1 << 3),
		COM_LOG_LEVEL_FATAL = (1 << 4),
	};
#endif
	/// <summary>
	/// vSeasky协议通信组件,基于Win-Api
	/// </summary>
	PROTOCOL_CLASS_REF vSeaskyPort
	{
		public:
			vSeaskyPort(void);
			~vSeaskyPort();
			//协议依赖
			protocol_struct* pTxProtocol = new protocol_struct();
			protocol_struct* pRxProtocol = new protocol_struct();
			bool GetStorageMethodIsSmall(void);

			/// <summary>
			/// 初始化协议所需内存
			/// </summary>
			/// <param name="uTxLen">pTxProtocol的uint32_t数据长度</param>
			/// <param name="uRxLen">pRxProtocol的uint32_t数据长度</param>
			void ProtocolInit(uint16_t uTxLen, uint16_t uRxLen);

			/// <summary>
			/// 通过动态 Data(uint32_t) 长度计算总数据Buffer(uint8_t) 的长度
			/// </summary>
			/// <param name="uLen"></param>
			/// <returns></returns>
			uint16_t ProtocolCalcLen(uint16_t uLen);

			/// <summary>
			/// 自动初始化Tx所需动态内存，uLen需小于128，内部分配内存，在外部托管无法访问
			/// </summary>
			/// <param name="uLen">pTxProtocol的uint32_t数据长度</param>
			void ProtocolAutoInitTx(uint16_t uLen);

			/// <summary>
			/// 自动初始化Rx所需动态内存，uLen需小于128，内部分配内存，在外部托管无法访问
			/// </summary>
			/// <param name="uLen">pRxProtocol的uint32_t数据长度</param>
			void ProtocolAutoInitRx(uint16_t uLen);


			/// <summary>
			/// 获取Tx动态内存地址和长度，返回长度为 pRxData长度，pRxBuffer长度为 uMaxLen*4+12
			/// </summary>
			/// <param name="pTxData"></param>
			/// <param name="pTxBuffer"></param>
			/// <param name="uMaxLen">pTxData的数据长度</param>
			void ProtocolTxPointGet(uint32_t* pTxData, uint8_t* pTxBuffer, uint16_t& uMaxLen);


			/// <summary>
			/// 获取Rx动态内存地址和长度，返回长度为 pRxData长度，pRxBuffer长度为 uMaxLen*4+12
			/// </summary>
			/// <param name="pRxData"></param>
			/// <param name="pRxBuffer"></param>
			/// <param name="uMaxLen">pRxData的数据长度</param>
			void ProtocolRxPointGet(uint32_t* pTxData, uint8_t* pTxBuffer, uint16_t& uMaxLen);

			/// <summary>
			/// 初始化协议Tx所需内存，外部分配，uLen需小于128
			/// </summary>
			/// <param name="pTxData">预先分配的内存地址</param>
			/// <param name="pTxBuffer">预先分配的内存地址</param>
			/// <param name="uLen">pTxData的数据长度</param>
			void ProtocolInitTx(uint32_t* pTxData, uint8_t* pTxBuffer, uint16_t uLen);

			/// <summary>
			/// 初始化协议Rx所需内存,外部分配，uLen需小于128
			/// </summary>
			/// <param name="pRxData">预先分配的内存地址</param>
			/// <param name="pRxBuffer">预先分配的内存地址</param>
			/// <param name="uLen">pRxData的数据长度</param>
			void ProtocolInitRx(uint32_t* pRxData, uint8_t* pRxBuffer, uint16_t uLen);
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
			bool vSerialOpen(uint32_t com_num, uint32_t baud_rate, uint32_t parity, uint32_t byte_size, uint32_t stop_bits);

			/// <summary>
			/// 关闭串口
			/// </summary>
			bool vSerialClose();

			/// <summary>
			/// 判断串口是否打开
			/// </summary>
			/// <returns>正确返回为ture，错误返回为false</returns>
			bool vSerialIsOpen();

			/// <summary>
			/// 清除缓冲区
			/// </summary>
			void vSerialClearBuffer(void);

			/// <summary>
			/// 得到最后一次失败的错误信息
			/// </summary>
			/// <returns></returns>
			uint8_t vSerialGetLastError();

			/// <summary>
			/// 将该函数放在一个独立的线程中，以实现串口消息的接收处理
			/// </summary>
			void vSerialReceiveTask();

			/// <summary>
			/// 协议计算，并发送数据
			/// </summary>
			void ProtocolTransmit(uint16_t equipment_type, uint16_t equipment_id, uint16_t data_id, uint32_t* pData, uint16_t data_len);

			/// <summary>
			/// 设置接收数据处理完成回调函数
			/// </summary>
			/// <param name="pFun"></param>
			void vSerialSetReceivCallbackFun(pReceivePointer^ pFun);


			/// <summary>
			/// 设置调试信息等级和调试信息回调显示函数
			/// </summary>
			/// <param name="debugLevel">调试信息等级</param>
			/// <param name="pFun">调试信息回调显示函数</param>
			void vSerialSetDebugCallbackFun(uint8_t debugLevel, pDebugPointer^ pFun);

		private:
			static bool		vSerialIsOpenIt = false;						//串口打开标志
			static bool     pSerialReceiveCppPointerIsEnable = false;		//是否初始化数据处理完成函数回调
			static bool     pSerialDebugIsEnable = false;					//是否初始化调试信息函数回调
			static vSerialPort* vSerialPortClass = new vSerialPort();		//Win Api串口实例
			static pReceivePointer^ pReceiveCallbackFun;					//数据处理完成函数回调指针
			static pDebugPointer^ pDebugCallbackFun;					//调试信息函数回调指针

			/// <summary>
			/// 接收数据处理完成中断
			/// </summary>
			void vReceiveCallBack(void);

			/// <summary>
			/// 接收调试信息信息打印函数
			/// </summary>
			/// <param name="pStr"></param>
			void vSerialDebugPrintf(char* pStr);
#endif
	};

#ifdef CLR_CONFIG
}
#endif

#endif


