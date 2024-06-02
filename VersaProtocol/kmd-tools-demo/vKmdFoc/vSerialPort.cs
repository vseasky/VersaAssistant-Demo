using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Threading;
using System.Threading;
using System.IO.Ports;

using vSeaskyProtocol;


namespace vKmdFocApi
{
    /// <summary>
    /// KmdFocApi的串口通信实现
    /// </summary>
    public partial class vSerialPort
    {
        // 实例化 vSeaskyPort
        public unsafe static vSeaskyPort vcSeaskyPort = new vSeaskyPort();
        public bool vSerialState = false;   //串口打开状态
        public bool vFirstInit = false;     // 只需要初始化一次

        Thread vReceivedThread = null;

        public String vSerialPortName;
        public UInt32 vSerialPortNameNum;
        public UInt32 vSerialBaudRate;
        public UInt32 vSerialDataBits;      /* Number of bits/byte, 4-8        */
        public UInt32 vSerialParity;        /* 0-4=None,Odd,Even,Mark,Space    */
        public UInt32 vSerialStopBits;      /* 0,1,2 = 1, 1.5, 2               */
        public vSerialPort()
        {
            try
            {
                // 关闭串口
                vcSeaskyPort.vSerialClose();
                // 创建一个接收线程
                vReceivedThread = new Thread(new ThreadStart(ThreadReceivedFun));
                vReceivedThread.IsBackground = true;
                vSerialState = false;
                // 线程先挂起,现在串口未打开
                vReceivedThread.Start();
                vReceivedThread.Suspend();
            }
            catch
            {

            }
        }

        /// <summary>
        /// 初始化 vSeasky协议，动态分配内存方式，对于 C# 不能在程序刚起来就调用此函数 unsafe 不安全代码太早调用很容易崩溃，需要等待程序加载一定时间后在初始化。
        /// </summary>
        public unsafe void vSeaskyPortInit(UInt16 uTxLne, UInt16 uRxLne, pReceivePointer pReceiveFunPointer, pDebugPointer pDebugFunPointer, byte debug_level)
        {
            unsafe
            {
                if (vFirstInit == false)
                {
                    // 初始化协议所需内存
                    vcSeaskyPort.ProtocolInit(uTxLne, uRxLne);
                    vFirstInit = true;
                    vcSeaskyPort.vSerialSetReceivCallbackFun(pReceiveFunPointer);
                    vcSeaskyPort.vSerialSetDebugCallbackFun((byte)(debug_level), pDebugFunPointer);
                }
            }
        }


        /// <summary>
        /// 用C#自带的串口类扫描串口设备
        /// </summary>
        /// <returns></returns>
        public string[] vSerialScanf()
        {
            return SerialPort.GetPortNames();
        }

        /// <summary>
        /// 打开串口，打开串口前应当提前设置串口参数。
        /// </summary>
        /// <returns></returns>
        public bool vSerialOpen()
        {
            if (vFirstInit == true)
            {
                unsafe
                {
                    try
                    {
                        // 需要提前设置参数
                        vcSeaskyPort.vSerialOpen(vSerialPortNameNum, vSerialBaudRate, vSerialParity, vSerialDataBits, vSerialStopBits);
                        Thread.Sleep(5);
                        // 获取串口打开状态
                        vSerialState = vcSeaskyPort.vSerialIsOpen();
                        if (vSerialState == true)
                        {
                            vReceivedThread.Resume();
                        }
                    }
                    catch (Exception)
                    {

                    };
                }
            }
            return vSerialState;
        }

        /// <summary>
        /// 关闭串口。
        /// </summary>
        /// <returns></returns>
        public bool vSerialClose()
        {
            vSerialState = false;
            Thread.Sleep(10);
            vcSeaskyPort.vSerialClose();
            Thread.Sleep(10);
            vReceivedThread.Suspend();
            return (vcSeaskyPort.vSerialIsOpen() == false);
        }

        /// <summary>
        /// 接收线程，在初始化之后就不用管了，提供给 vSeaskyPort 内部循环检测接收数据用。
        /// </summary>
        public void ThreadReceivedFun()
        {
            while (true)
            {
                vcSeaskyPort.vSerialReceiveTask();
            }
        }
    }
}
