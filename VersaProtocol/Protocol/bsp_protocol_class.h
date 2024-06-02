#ifndef _BSP_PROTOCOL_CLASS_H_
#define _BSP_PROTOCOL_CLASS_H_

#include "bsp_protocol.h"

#ifdef CLR_CONFIG
//CLR_CONFIG ���ڻ�࣬C++����C# �ɵ���Dll
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
/// Seasky ͨ�ſ� ����Win-Api
/// </summary>
namespace vSeaskyProtocol
{
#endif

#ifdef CLR_CONFIG
	// ������SeaskyЭ�����ݷ���
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
	/// vSeaskyЭ��ͨ�����,����Win-Api
	/// </summary>
	PROTOCOL_CLASS_REF vSeaskyPort
	{
		public:
			vSeaskyPort(void);
			~vSeaskyPort();
			//Э������
			protocol_struct* pTxProtocol = new protocol_struct();
			protocol_struct* pRxProtocol = new protocol_struct();
			bool GetStorageMethodIsSmall(void);

			/// <summary>
			/// ��ʼ��Э�������ڴ�
			/// </summary>
			/// <param name="uTxLen">pTxProtocol��uint32_t���ݳ���</param>
			/// <param name="uRxLen">pRxProtocol��uint32_t���ݳ���</param>
			void ProtocolInit(uint16_t uTxLen, uint16_t uRxLen);

			/// <summary>
			/// ͨ����̬ Data(uint32_t) ���ȼ���������Buffer(uint8_t) �ĳ���
			/// </summary>
			/// <param name="uLen"></param>
			/// <returns></returns>
			uint16_t ProtocolCalcLen(uint16_t uLen);

			/// <summary>
			/// �Զ���ʼ��Tx���趯̬�ڴ棬uLen��С��128���ڲ������ڴ棬���ⲿ�й��޷�����
			/// </summary>
			/// <param name="uLen">pTxProtocol��uint32_t���ݳ���</param>
			void ProtocolAutoInitTx(uint16_t uLen);

			/// <summary>
			/// �Զ���ʼ��Rx���趯̬�ڴ棬uLen��С��128���ڲ������ڴ棬���ⲿ�й��޷�����
			/// </summary>
			/// <param name="uLen">pRxProtocol��uint32_t���ݳ���</param>
			void ProtocolAutoInitRx(uint16_t uLen);


			/// <summary>
			/// ��ȡTx��̬�ڴ��ַ�ͳ��ȣ����س���Ϊ pRxData���ȣ�pRxBuffer����Ϊ uMaxLen*4+12
			/// </summary>
			/// <param name="pTxData"></param>
			/// <param name="pTxBuffer"></param>
			/// <param name="uMaxLen">pTxData�����ݳ���</param>
			void ProtocolTxPointGet(uint32_t* pTxData, uint8_t* pTxBuffer, uint16_t& uMaxLen);


			/// <summary>
			/// ��ȡRx��̬�ڴ��ַ�ͳ��ȣ����س���Ϊ pRxData���ȣ�pRxBuffer����Ϊ uMaxLen*4+12
			/// </summary>
			/// <param name="pRxData"></param>
			/// <param name="pRxBuffer"></param>
			/// <param name="uMaxLen">pRxData�����ݳ���</param>
			void ProtocolRxPointGet(uint32_t* pTxData, uint8_t* pTxBuffer, uint16_t& uMaxLen);

			/// <summary>
			/// ��ʼ��Э��Tx�����ڴ棬�ⲿ���䣬uLen��С��128
			/// </summary>
			/// <param name="pTxData">Ԥ�ȷ�����ڴ��ַ</param>
			/// <param name="pTxBuffer">Ԥ�ȷ�����ڴ��ַ</param>
			/// <param name="uLen">pTxData�����ݳ���</param>
			void ProtocolInitTx(uint32_t* pTxData, uint8_t* pTxBuffer, uint16_t uLen);

			/// <summary>
			/// ��ʼ��Э��Rx�����ڴ�,�ⲿ���䣬uLen��С��128
			/// </summary>
			/// <param name="pRxData">Ԥ�ȷ�����ڴ��ַ</param>
			/// <param name="pRxBuffer">Ԥ�ȷ�����ڴ��ַ</param>
			/// <param name="uLen">pRxData�����ݳ���</param>
			void ProtocolInitRx(uint32_t* pRxData, uint8_t* pRxBuffer, uint16_t uLen);
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
			bool vSerialOpen(uint32_t com_num, uint32_t baud_rate, uint32_t parity, uint32_t byte_size, uint32_t stop_bits);

			/// <summary>
			/// �رմ���
			/// </summary>
			bool vSerialClose();

			/// <summary>
			/// �жϴ����Ƿ��
			/// </summary>
			/// <returns>��ȷ����Ϊture�����󷵻�Ϊfalse</returns>
			bool vSerialIsOpen();

			/// <summary>
			/// ���������
			/// </summary>
			void vSerialClearBuffer(void);

			/// <summary>
			/// �õ����һ��ʧ�ܵĴ�����Ϣ
			/// </summary>
			/// <returns></returns>
			uint8_t vSerialGetLastError();

			/// <summary>
			/// ���ú�������һ���������߳��У���ʵ�ִ�����Ϣ�Ľ��մ���
			/// </summary>
			void vSerialReceiveTask();

			/// <summary>
			/// Э����㣬����������
			/// </summary>
			void ProtocolTransmit(uint16_t equipment_type, uint16_t equipment_id, uint16_t data_id, uint32_t* pData, uint16_t data_len);

			/// <summary>
			/// ���ý������ݴ�����ɻص�����
			/// </summary>
			/// <param name="pFun"></param>
			void vSerialSetReceivCallbackFun(pReceivePointer^ pFun);


			/// <summary>
			/// ���õ�����Ϣ�ȼ��͵�����Ϣ�ص���ʾ����
			/// </summary>
			/// <param name="debugLevel">������Ϣ�ȼ�</param>
			/// <param name="pFun">������Ϣ�ص���ʾ����</param>
			void vSerialSetDebugCallbackFun(uint8_t debugLevel, pDebugPointer^ pFun);

		private:
			static bool		vSerialIsOpenIt = false;						//���ڴ򿪱�־
			static bool     pSerialReceiveCppPointerIsEnable = false;		//�Ƿ��ʼ�����ݴ�����ɺ����ص�
			static bool     pSerialDebugIsEnable = false;					//�Ƿ��ʼ��������Ϣ�����ص�
			static vSerialPort* vSerialPortClass = new vSerialPort();		//Win Api����ʵ��
			static pReceivePointer^ pReceiveCallbackFun;					//���ݴ�����ɺ����ص�ָ��
			static pDebugPointer^ pDebugCallbackFun;					//������Ϣ�����ص�ָ��

			/// <summary>
			/// �������ݴ�������ж�
			/// </summary>
			void vReceiveCallBack(void);

			/// <summary>
			/// ���յ�����Ϣ��Ϣ��ӡ����
			/// </summary>
			/// <param name="pStr"></param>
			void vSerialDebugPrintf(char* pStr);
#endif
	};

#ifdef CLR_CONFIG
}
#endif

#endif


