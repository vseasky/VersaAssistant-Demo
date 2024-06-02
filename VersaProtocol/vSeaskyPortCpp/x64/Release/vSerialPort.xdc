<?xml version="1.0"?><doc>
<members>
<member name="M:get_protocol_size(System.UInt16)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.h" line="90">
<summary>
根据data_union长度计算数据帧长度，用于分配内存
</summary>
<param name="uLen"></param>
<returns></returns>
</member>
<member name="M:init_protocol(protocol_struct*,System.UInt16)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.h" line="97">
<summary>
方式一 初始化,并初始化内存
</summary>
<param name="pProtocol"></param>
<param name="uLen"></param>
</member>
<member name="M:init_protocol_pointer(protocol_struct*,System.Void*,System.Void*,System.UInt16)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.h" line="104">
<summary>
方式二 外部预先分配好内存空间
</summary>
<param name="pProtocol"></param>
<param name="pFrameSt"></param>
<param name="pMessageSt"></param>
<param name="uLen"></param>
<returns></returns>
</member>
<member name="M:make_protocol(protocol_struct*)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.h" line="114">
<summary>
生成带发送的数据内容
</summary>
<param name="pProtocol"></param>
<returns></returns>
</member>
<member name="M:parse_protocol(protocol_struct*,System.UInt16)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.h" line="121">
<summary>
解析接收到的数据
</summary>
<param name="pProtocol"></param>
<returns></returns>
</member>
<member name="M:deinit_protocol(protocol_struct*)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.h" line="128">
<summary>
反初始化，释放内存,如果是方式二创建然后反初始化，请注意规避野指针
</summary>
<param name="pProtocol"></param>
</member>
<member name="M:get_user_data_point(protocol_struct*)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.h" line="136">
<summary>
提供给用户，直接操作数据，但是请注意不要超过数据长度，避免内存越界
</summary>
<param name="pProtocol"></param>
<returns></returns>
</member>
<member name="M:get_message_point(protocol_struct*)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.h" line="144">
<summary>
提供给用户，直接操作数据，但是请注意不要超过数据长度，避免内存越界
</summary>
<param name="pProtocol"></param>
<returns></returns>
</member>
<member name="M:vSerialPort.openSyn(std.basic_string&lt;System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte,std.char_traits{System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte},std.allocator&lt;System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte&gt;&gt;,System.UInt32!System.Runtime.CompilerServices.IsLong,System.Byte,System.Byte,System.Byte)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\vSerial\vSerialPort.cpp" line="4">
<summary>
同步方式打开串口,并配置默认信息
</summary>
<param name="serial_name">串口名称</param>
<param name="baud_rate">波特率</param>
<param name="parity">校验方式</param>
<param name="byte_size">数据位大小</param>
<param name="stop_bits">停止位  0, 1, 2对应的是1bit, 1.5bits, 2bits.</param>
<returns>正确返回为ture，错误返回为false</returns>
</member>
<member name="M:vSerialPort.isOpened" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\vSerial\vSerialPort.cpp" line="97">
<summary>
关闭串口
</summary>
<summary>
判断串口是否打开
</summary>
<returns>正确返回为ture，错误返回为false</returns>
</member>
<member name="M:vSerialPort.setTimeouts(_COMMTIMEOUTS*!System.Runtime.CompilerServices.IsImplicitlyDereferenced)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\vSerial\vSerialPort.cpp" line="193">
<summary>
设置超时
</summary>
<param name="timeouts">超时配置的COMMTIMEOUTS结构体</param>
<returns>确返回为ture，错误返回为false</returns>
</member>
<member name="M:vSerialPort.setDCB(_DCB*!System.Runtime.CompilerServices.IsImplicitlyDereferenced)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\vSerial\vSerialPort.cpp" line="210">
<summary>
设置串口信息
</summary>
<param name="dcb">串口信息配置的DCB结构体</param>
<returns>正确返回为ture，错误返回为false</returns>
</member>
<member name="M:vSerialPort.purgeBuff(System.UInt32!System.Runtime.CompilerServices.IsLong)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\vSerial\vSerialPort.cpp" line="226">
<summary>
刷新缓冲区
</summary>
<param name="flags">    
        flags：需要完成的操作，取值如下
                PURGE_RXABORT 终止所有未完成的重叠读取操作并立即返回，即使读取操作尚未完成。
                PURGE_RXCLEAR 清除输入缓冲区（如果设备驱动程序有一个）。
                PURGE_TXABORT 终止所有未完成的重叠写操作并立即返回，即使写操作尚未完成。
                PURGE_TXCLEAR 清除输出缓冲区（如果设备驱动程序有一个）。
</param>
<returns>正确返回为ture，错误返回为false</returns>
</member>
<member name="M:vSerialPort.flushBuff" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\vSerial\vSerialPort.cpp" line="249">
<summary>
刷新缓冲区
</summary>
<returns>正确返回为ture，错误返回为false</returns>
</member>
<member name="M:vSerialPort.setBufferSize(System.UInt32!System.Runtime.CompilerServices.IsLong,System.UInt32!System.Runtime.CompilerServices.IsLong)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\vSerial\vSerialPort.cpp" line="263">
<summary>
设置推荐的缓冲大小
</summary>
<param name="inputBuff">输入缓冲大小</param>
<param name="outBuffer">输出缓冲大小</param>
<returns>正确返回为ture，错误返回为false</returns>
</member>
<member name="M:vSerialPort.getSerialReceiveLen" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\vSerial\vSerialPort.cpp" line="277">
<summary>
输入缓冲区中的字节数
</summary>
<returns></returns>
</member>
<member name="M:vSerialPort.getSerialTransmitLen" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\vSerial\vSerialPort.cpp" line="287">
<summary>
输出缓冲区字节数
</summary>
<returns></returns>
</member>
<member name="M:vSerialPort.getSerialLastError" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\vSerial\vSerialPort.cpp" line="296">
<summary>
得到最后一次失败的错误信息
</summary>
<returns>错误信息</returns>
</member>
</members>
</doc>