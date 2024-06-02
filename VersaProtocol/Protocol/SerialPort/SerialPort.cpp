#include <vSerial/vSerialPort.h>
#include <bsp_protocol_class.h>

/// <summary>
/// 同步方式打开串口,并配置默认信息
/// </summary>
/// <param name="serial_name">串口名称</param>
/// <param name="baud_rate">波特率</param>
/// <param name="parity">校验方式</param>
/// <param name="byte_size">数据位大小</param>
/// <param name="stop_bits">停止位  0, 1, 2对应的是1bit, 1.5bits, 2bits.</param>
/// <returns>正确返回为ture，错误返回为false</returns>
bool vSerialPort::openSyn(string vSerialName, DWORD vBaudRate, BYTE vParity, BYTE vByteSize, BYTE vStopBits)
{
    //hCom = CreateFileA(
    //    serial_name.data(),                             //普通文件名或者设备文件名，这里是串口名
    //    GENERIC_READ | GENERIC_WRITE,                   //访问模式,读和写   
    //    0,                                              //共享模式，独占模式
    //    NULL,                                           //指向安全属性的指针,不使用，传NULL
    //    OPEN_EXISTING,                                  //如何创建,在串口中必须设置为OPEN_EXISTING。表示不能创建新端口只能打开已有的端口。
    //    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,   //文件属性,使用默认属性FILE_ATTRIBUTE_NORMAL。
    //    NULL                                            //用于复制文件句柄,通常这个参数设置为NULL，为空表示不使用模板
    //);
    hCom = CreateFileA(
        vSerialName.data(),                             //普通文件名或者设备文件名，这里是串口名
        GENERIC_READ | GENERIC_WRITE,                   //访问模式,读和写
        0,                                              //共享模式，独占模式
        NULL,                                           //指向安全属性的指针,不使用，传NULL
        OPEN_EXISTING,                                  //如何创建,在串口中必须设置为OPEN_EXISTING。表示不能创建新端口只能打开已有的端口。
        0,                                              //文件属性,使用默认属性FILE_ATTRIBUTE_NORMAL。
        NULL                                            //用于复制文件句柄,通常这个参数设置为NULL，为空表示不使用模板
    );
    if (INVALID_HANDLE_VALUE == hCom)//出错判断
    {
        hCom = NULL;
        setSerialLastError(OpenError);
        return false;
    }
    DCB dcb;
    GetCommState(hCom, &dcb);
    dcb.DCBlength = sizeof(dcb);
    //BYTE ByteSize;        /* Number of bits/byte, 4-8        */
    //BYTE Parity;          /* 0-4=None,Odd,Even,Mark,Space    */
    //BYTE StopBits;        /* 0,1,2 = 1, 1.5, 2               */
    dcb.BaudRate = vBaudRate;
    dcb.Parity   = vParity;
    dcb.StopBits = vStopBits;
    dcb.ByteSize = vByteSize;
    if (false == SetCommState(hCom, &dcb))//用DCB结构重新配置串行端口信息
    {
        setSerialLastError(SetCommStateError);
        //cout << "SetCommStateError" << endl;
        //printf("SetCommStateError\n");
        //return false;
    }
    //清空缓冲区，为读写串口做准备
    if (false == PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_RXABORT))
    {
        setSerialLastError(PurgeCommError);
        //printf("PurgeCommError\n");
        //return false;
    }

    //超时处理
    COMMTIMEOUTS timeouts;
    //两字符之间最大的延时
    timeouts.ReadIntervalTimeout = MAXDWORD;    //读间隔超时
    //把间隔超时设为最大，把总超时设为0将导致ReadFile立即返回并完成操作 
    timeouts.ReadTotalTimeoutMultiplier = 0;    //读取每字符间的超时
    timeouts.ReadTotalTimeoutConstant   = 0;    //一次读取串口数据的固定超时
    timeouts.WriteTotalTimeoutMultiplier = 1;   // 写时间系数
    timeouts.WriteTotalTimeoutConstant =   5;   //写时间常量
    //总的读/写超时时间 = Read(Write)TotalTimeoutMultiplier x 要读/写的字节数 + Read(Write)TotalTimeoutConstant. 
    if (false == SetCommTimeouts(hCom, &timeouts))
    {
        setSerialLastError(SetCommTimeoutsError);
        //printf("SetCommTimeoutsError\n");
        //return false;
    }
    //mOsRead.hEvent = CreateEvent(NULL, false, false, NULL);
    //mOsWrite.hEvent = CreateEvent(NULL, false, false, NULL);//
    //mOsWait.hEvent = CreateEvent(NULL, false, false, NULL); //

    clearSerialError();
    //SetCommMask(hCom, EV_ERR | EV_RXCHAR); //设置接收事件
    //mThread = (HANDLE)_beginthreadex(NULL, 0, &vSerialPort::SerialRecv, this, 0, NULL);
    //printf("openSyn\n");
    flushBuff();
    isOpenSerial = true;
    return true;
}
void vSerialPort::clearSerialError()
{
    ClearCommError(hCom, &dwErrorFlags, &ComStat);
}

/// <summary>
/// 关闭串口
/// </summary>
//void vSerialPort::closeComm()
//{
//    if (NULL == hCom)
//        return;
//    CloseHandle(hCom);
//    hCom = NULL;
//}

/// <summary>
/// 判断串口是否打开
/// </summary>
/// <returns>正确返回为ture，错误返回为false</returns>
bool vSerialPort::isOpened()
{
    return isOpenSerial;
}


bool vSerialPort::clearRxBuffer()
{
    if (false == PurgeComm(hCom, PURGE_RXCLEAR  | PURGE_RXABORT))
    {
        setSerialLastError(PurgeCommError);
        //printf("PurgeCommError\n");
        return false;
    }
    return true;
}

DWORD vSerialPort::readData(char* buffer, uint16_t length)
{
    DWORD readDataSize = 0;
    bool ret = false;
    clearSerialError();
    if (isOpenSerial)
    {
//        readDataSize = 0;
//        mOsRead.Offset = 0;
//      ret = ReadFile(hCom, buffer, length, &readDataSize, &mOsRead);
        ret = ReadFile(hCom, buffer, length, &readDataSize,NULL);
        //printf("readDataSize[%d]\n", readDataSize);

    }
    if (false == ret)
    {
        return 0;
    }
    return readDataSize;
}


DWORD vSerialPort::writeData(char* buffer, uint16_t length)
{
    DWORD writeSize = 0;
    bool ret = false;
    //PurgeComm(hCom, PURGE_TXCLEAR | PURGE_TXABORT);
    if (isOpenSerial)
    {
        //mOsWrite.Offset = 0;
        //ret = WriteFile(hCom, buffer, length, &writeSize, &mOsWrite);
        ret = WriteFile(hCom, buffer, length, &writeSize, NULL);
    }
    if (false == ret)
    {
        return 0;
    }
    return writeSize;

}


DWORD vSerialPort::writeStr(string str)
{
    bool ret = false;
    DWORD writeSize = 0;
    //PurgeComm(hCom, PURGE_TXCLEAR | PURGE_TXABORT);
    if (isOpenSerial)
    {
        //mOsWrite.Offset = 0;
        //ret = WriteFile(hCom, str.data(), str.length(), &writeSize, &mOsWrite);
        ret = WriteFile(hCom, str.data(), str.length(), &writeSize, NULL);
    }
    if (0 == ret)
    {
        last_error = WriteStrError;
        return 0;
    }
    last_error = vSerialPortNull;
    return writeSize;

}


/// <summary>
/// 设置超时
/// </summary>
/// <param name="timeouts">超时配置的COMMTIMEOUTS结构体</param>
/// <returns>确返回为ture，错误返回为false</returns>
bool vSerialPort::setTimeouts(COMMTIMEOUTS& timeouts)
{

    if (false == SetCommTimeouts(hCom, &timeouts))
    {
        setSerialLastError(SetCommTimeoutsError);
        return false;
    }
    return true;
}


/// <summary>
/// 设置串口信息
/// </summary>
/// <param name="dcb">串口信息配置的DCB结构体</param>
/// <returns>正确返回为ture，错误返回为false</returns>
bool vSerialPort::setDCB(DCB& dcb)
{
    if (false == SetCommState(hCom, &dcb))
    {
        setSerialLastError(SetCommStateError);
        return false;
    }
    return true;
}


/// <summary>
/// 刷新缓冲区
/// </summary>
/// <param name="flags">    
///         flags：需要完成的操作，取值如下
///                 PURGE_RXABORT 终止所有未完成的重叠读取操作并立即返回，即使读取操作尚未完成。
///                 PURGE_RXCLEAR 清除输入缓冲区（如果设备驱动程序有一个）。
///                 PURGE_TXABORT 终止所有未完成的重叠写操作并立即返回，即使写操作尚未完成。
///                 PURGE_TXCLEAR 清除输出缓冲区（如果设备驱动程序有一个）。
///</param>
/// <returns>正确返回为ture，错误返回为false</returns>
bool vSerialPort::purgeBuff(DWORD flags)
{

    if (false == PurgeComm(hCom, flags))
    {
        setSerialLastError(PurgeCommError);
        return false;
    }
    return true;
}


/// <summary>
/// 刷新缓冲区
/// </summary>
/// <returns>正确返回为ture，错误返回为false</returns>
bool vSerialPort::flushBuff()
{
    if (FlushFileBuffers(hCom))
    {
        setSerialLastError(FlushFileBuffersError);
        return false;
    }
    return true;
}

/// <summary>
/// 设置推荐的缓冲大小
/// </summary>
/// <param name="inputBuff">输入缓冲大小</param>
/// <param name="outBuffer">输出缓冲大小</param>
/// <returns>正确返回为ture，错误返回为false</returns>
bool vSerialPort::setBufferSize(DWORD inputBuff, DWORD outBuffer)
{
    if (inputBuff <= 0 || outBuffer <= 0)
        return false;

    return SetupComm(hCom, inputBuff, outBuffer);
}

/// <summary>
/// 输入缓冲区中的字节数
/// </summary>
/// <returns></returns>
uint32_t  vSerialPort::getSerialReceiveLen()
{
    //输入缓冲区中的字节数
    return ComStat.cbInQue;
}

/// <summary>
/// 输出缓冲区字节数
/// </summary>
/// <returns></returns>
uint32_t vSerialPort::getSerialTransmitLen()
{
    return ComStat.cbOutQue;
}

/// <summary>
/// 得到最后一次失败的错误信息
/// </summary>
/// <returns>错误信息</returns>
uint8_t vSerialPort::getSerialLastError()
{
    return last_error;
}
void vSerialPort::setSerialLastError(uint8_t error_msg)
{
    last_error = error_msg;
}
void vSerialPort::clearSerialLastError()
{
    last_error = vSerialPortNull;
}

bool vSerialPort::closeSerial()
{
    bool ret = false;
    clearSerialError();
    if (false == PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_RXABORT))
    {
        setSerialLastError(PurgeCommError);
    }
    flushBuff();
    if (INVALID_HANDLE_VALUE != hCom)
    {
        ret = CloseHandle(hCom);
        if (ret == true)
        {
            hCom = INVALID_HANDLE_VALUE;
            isOpenSerial = false;
        }
    }
    return ret;
}
vSerialPort::vSerialPort()
{
    hCom = NULL;
}
vSerialPort::~vSerialPort()
{

}

