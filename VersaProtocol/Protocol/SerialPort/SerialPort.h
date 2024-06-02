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
	/// 同步方式打开串口,并配置默认信息
	/// </summary>
	/// <param name="serial_name"></param>
	/// <param name="baud_rate"></param>
	/// <param name="parity"></param>
	/// <param name="byte_size"></param>
	/// <param name="stop_bits"></param>
	/// <returns></returns>
	bool openSyn(string vSerialName, DWORD vBaudRate, BYTE vParity, BYTE vByteSize, BYTE vStopBits);

	/// <summary>
	/// 设置推荐的缓冲大小
	/// </summary>
	/// <param name="inputBuff"></param>
	/// <param name="outBuffer"></param>
	/// <returns></returns>
	bool setBufferSize(DWORD inputBuff, DWORD outBuffer);

	/// <summary>
	/// 设置超时
	/// </summary>
	/// <param name="timeouts"></param>
	/// <returns></returns>
	bool setTimeouts(COMMTIMEOUTS& timeouts);

	/// <summary>
	/// 设置串口信息
	/// </summary>
	/// <param name="dcb"></param>
	/// <returns></returns>
	bool setDCB(DCB& dcb);

	/// <summary>
	/// 刷新缓冲区
	/// </summary>
	/// <param name="flags"></param>
	/// <returns></returns>
	bool purgeBuff(DWORD flags);

	/// <summary>
	/// 刷新缓冲区
	/// </summary>
	/// <returns></returns>
	bool flushBuff();

	/// <summary>
	/// 清空接收缓冲区
	/// </summary>
	bool clearRxBuffer();

	/// <summary>
	/// 写数据
	/// </summary>
	/// <param name="buffer"></param>
	/// <param name="length"></param>
	/// <returns></returns>
	DWORD writeData(char* buffer, uint16_t length);

	/// <summary>
	/// 读数据
	/// </summary>
	/// <param name="buffer"></param>
	/// <param name="length"></param>
	/// <returns></returns>
	DWORD readData(char* buffer, uint16_t length);

	/// <summary>
	/// 写字符串
	/// </summary>
	/// <param name="str"></param>
	/// <returns></returns>
	DWORD writeStr(string str);

	/// <summary>
	/// 关闭串口
	/// </summary>
	//void closeComm();

	/// <summary>
	/// 判断串口是否打开
	/// </summary>
	/// <returns></returns>
	bool isOpened();

	/// <summary>
	/// 得到最后一次失败的错误信息
	/// </summary>
	/// <returns></returns>
	uint8_t getSerialLastError();

	/// <summary>
	/// 清除串口错误信息
	/// </summary>
	void clearSerialError();
	bool closeSerial();
	//static unsigned int __stdcall SerialRecv(void* LPParam);

	uint32_t getSerialReceiveLen(void);
	uint32_t getSerialTransmitLen(void);
private:
	//OVERLAPPED mOsWrite;//用于写入数据
	//OVERLAPPED mOsRead; //用于读取数据
	//OVERLAPPED mOsWait; //用于等待数据
	//HANDLE      mThread;//读取线程句柄
	//设置最后一次的错误信息
	void setSerialLastError(uint8_t error_msg);
	//清chu最后一次的错误信息
	void clearSerialLastError();
};

#endif // !_VSERIALPORT_H_


