#ifndef _VSERIALPORT_H_
#define _VSERIALPORT_H_
#include<Windows.h>
#include<string>
using namespace std;
typedef enum
{
	vSerialPortNull,
	FlushFileBuffersError,
	PurgeCommError,
	SetCommStateError,
	GetCommStateError,
	SetCommTimeoutsError,
	WriteStrError,
	OpenError,
}vSerialPortError;
class vSerialPort
{
private:
	/* data */
	HANDLE hCom;
	uint8_t last_error;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
public:
	vSerialPort();
	~vSerialPort();
	bool isOpenSerial;
	/// <summary>
	/// ͬ����ʽ�򿪴���,������Ĭ����Ϣ
	/// </summary>
	/// <param name="serial_name"></param>
	/// <param name="baud_rate"></param>
	/// <param name="parity"></param>
	/// <param name="byte_size"></param>
	/// <param name="stop_bits"></param>
	/// <returns></returns>
	bool openSyn(string vSerialName, DWORD vBaudRate, BYTE vParity, BYTE vByteSize, BYTE vStopBits);

	/// <summary>
	/// �����Ƽ��Ļ����С
	/// </summary>
	/// <param name="inputBuff"></param>
	/// <param name="outBuffer"></param>
	/// <returns></returns>
	bool setBufferSize(DWORD inputBuff, DWORD outBuffer);

	/// <summary>
	/// ���ó�ʱ
	/// </summary>
	/// <param name="timeouts"></param>
	/// <returns></returns>
	bool setTimeouts(COMMTIMEOUTS& timeouts);

	/// <summary>
	/// ���ô�����Ϣ
	/// </summary>
	/// <param name="dcb"></param>
	/// <returns></returns>
	bool setDCB(DCB& dcb);

	/// <summary>
	/// ˢ�»�����
	/// </summary>
	/// <param name="flags"></param>
	/// <returns></returns>
	bool purgeBuff(DWORD flags);

	/// <summary>
	/// ˢ�»�����
	/// </summary>
	/// <returns></returns>
	bool flushBuff();

	/// <summary>
	/// ��ս��ջ�����
	/// </summary>
	bool clearRxBuffer();

	/// <summary>
	/// д����
	/// </summary>
	/// <param name="buffer"></param>
	/// <param name="length"></param>
	/// <returns></returns>
	DWORD writeData(char* buffer, uint16_t length);

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="buffer"></param>
	/// <param name="length"></param>
	/// <returns></returns>
	DWORD readData(char* buffer, uint16_t length);

	/// <summary>
	/// д�ַ���
	/// </summary>
	/// <param name="str"></param>
	/// <returns></returns>
	DWORD writeStr(string str);

	/// <summary>
	/// �رմ���
	/// </summary>
	//void closeComm();

	/// <summary>
	/// �жϴ����Ƿ��
	/// </summary>
	/// <returns></returns>
	bool isOpened();

	/// <summary>
	/// �õ����һ��ʧ�ܵĴ�����Ϣ
	/// </summary>
	/// <returns></returns>
	uint8_t getSerialLastError();

	/// <summary>
	/// ������ڴ�����Ϣ
	/// </summary>
	void clearSerialError();
	bool closeSerial();
	//static unsigned int __stdcall SerialRecv(void* LPParam);

	uint32_t getSerialReceiveLen(void);
	uint32_t getSerialTransmitLen(void);
private:
	//OVERLAPPED mOsWrite;//����д������
	//OVERLAPPED mOsRead; //���ڶ�ȡ����
	//OVERLAPPED mOsWait; //���ڵȴ�����
	//HANDLE      mThread;//��ȡ�߳̾��
	//�������һ�εĴ�����Ϣ
	void setSerialLastError(uint8_t error_msg);
	//��chu���һ�εĴ�����Ϣ
	void clearSerialLastError();
};

#endif // !_VSERIALPORT_H_


