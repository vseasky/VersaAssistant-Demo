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
<member name="T:vSeaskyProtocol.vSeaskyPort" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol_class.h" line="40">
<summary>
vSeasky协议通信组件,基于Win-Api
</summary>
</member>
<member name="M:vSeaskyProtocol.vSeaskyPort.ProtocolInit(System.UInt16,System.UInt16)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol_class.h" line="53">
<summary>
初始化协议所需内存
</summary>
<param name="uTxLen">pTxProtocol的uint32_t数据长度</param>
<param name="uRxLen">pRxProtocol的uint32_t数据长度</param>
</member>
<member name="M:vSeaskyProtocol.vSeaskyPort.ProtocolCalcLen(System.UInt16)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol_class.h" line="60">
<summary>
通过动态 Data(uint32_t) 长度计算总数据Buffer(uint8_t) 的长度
</summary>
<param name="uLen"></param>
<returns></returns>
</member>
<member name="M:vSeaskyProtocol.vSeaskyPort.ProtocolAutoInitTx(System.UInt16)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol_class.h" line="67">
<summary>
自动初始化Tx所需动态内存，uLen需小于128，内部分配内存，在外部托管无法访问
</summary>
<param name="uLen">pTxProtocol的uint32_t数据长度</param>
</member>
<member name="M:vSeaskyProtocol.vSeaskyPort.ProtocolAutoInitRx(System.UInt16)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol_class.h" line="73">
<summary>
自动初始化Rx所需动态内存，uLen需小于128，内部分配内存，在外部托管无法访问
</summary>
<param name="uLen">pRxProtocol的uint32_t数据长度</param>
</member>
<member name="M:vSeaskyProtocol.vSeaskyPort.ProtocolTxPointGet(System.UInt32*,System.Byte*,System.UInt16*!System.Runtime.CompilerServices.IsImplicitlyDereferenced)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol_class.h" line="80">
<summary>
获取Tx动态内存地址和长度，返回长度为 pRxData长度，pRxBuffer长度为 uMaxLen*4+12
</summary>
<param name="pTxData"></param>
<param name="pTxBuffer"></param>
<param name="uMaxLen">pTxData的数据长度</param>
</member>
<member name="M:vSeaskyProtocol.vSeaskyPort.ProtocolRxPointGet(System.UInt32*,System.Byte*,System.UInt16*!System.Runtime.CompilerServices.IsImplicitlyDereferenced)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol_class.h" line="89">
<summary>
获取Rx动态内存地址和长度，返回长度为 pRxData长度，pRxBuffer长度为 uMaxLen*4+12
</summary>
<param name="pRxData"></param>
<param name="pRxBuffer"></param>
<param name="uMaxLen">pRxData的数据长度</param>
</member>
<member name="M:vSeaskyProtocol.vSeaskyPort.ProtocolInitTx(System.UInt32*,System.Byte*,System.UInt16)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol_class.h" line="97">
<summary>
初始化协议Tx所需内存，外部分配，uLen需小于128
</summary>
<param name="pTxData">预先分配的内存地址</param>
<param name="pTxBuffer">预先分配的内存地址</param>
<param name="uLen">pTxData的数据长度</param>
</member>
<member name="M:vSeaskyProtocol.vSeaskyPort.ProtocolInitRx(System.UInt32*,System.Byte*,System.UInt16)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol_class.h" line="105">
<summary>
初始化协议Rx所需内存,外部分配，uLen需小于128
</summary>
<param name="pRxData">预先分配的内存地址</param>
<param name="pRxBuffer">预先分配的内存地址</param>
<param name="uLen">pRxData的数据长度</param>
</member>
<member name="M:vSeaskyProtocol.vSeaskyPort.vSerialOpen(System.UInt32,System.UInt32,System.UInt32,System.UInt32,System.UInt32)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol_class.h" line="114">
<summary>
同步方式打开串口
</summary>
<param name="com_num">串口号</param>
<param name="baud_rate">波特率</param>
<param name="parity">奇偶校验位</param>
<param name="byte_size">数据位</param>
<param name="stop_bits">停止位</param>
<returns></returns>
</member>
<member name="M:vSeaskyProtocol.vSeaskyPort.vSerialClose" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol_class.h" line="125">
<summary>
关闭串口
</summary>
</member>
<member name="M:vSeaskyProtocol.vSeaskyPort.vSerialIsOpen" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol_class.h" line="130">
<summary>
判断串口是否打开
</summary>
<returns>正确返回为ture，错误返回为false</returns>
</member>
<member name="M:vSeaskyProtocol.vSeaskyPort.vSerialClearBuffer" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol_class.h" line="136">
<summary>
清除缓冲区
</summary>
</member>
<member name="M:vSeaskyProtocol.vSeaskyPort.vSerialGetLastError" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol_class.h" line="141">
<summary>
得到最后一次失败的错误信息
</summary>
<returns></returns>
</member>
<member name="M:vSeaskyProtocol.vSeaskyPort.vSerialReceiveTask" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol_class.h" line="147">
<summary>
将该函数放在一个独立的线程中，以实现串口消息的接收处理
</summary>
</member>
<member name="M:vSeaskyProtocol.vSeaskyPort.ProtocolTransmit(System.UInt16,System.UInt16,System.UInt16,System.UInt32*,System.UInt16)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol_class.h" line="152">
<summary>
协议计算，并发送数据
</summary>
</member>
<member name="M:vSeaskyProtocol.vSeaskyPort.vSerialSetReceivCallbackFun(vSeaskyProtocol.pReceivePointer)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol_class.h" line="157">
<summary>
设置接收数据处理完成回调函数
</summary>
<param name="pFun"></param>
</member>
<member name="M:vSeaskyProtocol.vSeaskyPort.vSerialSetDebugCallbackFun(System.Byte,vSeaskyProtocol.pDebugPointer)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol_class.h" line="164">
<summary>
设置调试信息等级和调试信息回调显示函数
</summary>
<param name="debugLevel">调试信息等级</param>
<param name="pFun">调试信息回调显示函数</param>
</member>
<member name="M:vSeaskyProtocol.vSeaskyPort.vReceiveCallBack" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol_class.h" line="179">
<summary>
接收数据处理完成中断
</summary>
</member>
<member name="M:vSeaskyProtocol.vSeaskyPort.vSerialDebugPrintf(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte*)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol_class.h" line="184">
<summary>
接收调试信息信息打印函数
</summary>
<param name="pStr"></param>
</member>
<member name="M:crc_8(System.Byte!System.Runtime.CompilerServices.IsConst*,System.UInt16)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\crc\bsp_crc8.h" line="14">
<summary>
计算输入字符串的8位宽crc
</summary>
<param name="input_str"></param>
<param name="num_bytes">给定长度</param>
<returns></returns>
</member>
<member name="M:update_crc_8(System.Byte,System.Byte)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\crc\bsp_crc8.h" line="22">
<summary>
给定一个数据字节和循环冗余校验值的前一个值
</summary>
<param name="crc"></param>
<param name="val"></param>
<returns>计算并返回数据的新的实际crc值</returns>
</member>
<member name="M:crc_16(System.Byte!System.Runtime.CompilerServices.IsConst*,System.UInt16)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\crc\bsp_crc16.h" line="17">
<summary>
函数crc_16()一次计算一个字节的16位CRC16
</summary>
<param name="input_str">字符串</param>
<param name="num_bytes">字节数</param>
<returns></returns>
</member>
<member name="M:crc_modbus(System.Byte!System.Runtime.CompilerServices.IsConst*,System.UInt16)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\crc\bsp_crc16.h" line="25">
<summary>
一次计算16位modbus循环冗余校验
</summary>
<param name="input_str">字符串</param>
<param name="num_bytes">字节数</param>
<returns></returns>
</member>
<member name="M:update_crc_16(System.UInt16,System.Byte)" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\crc\bsp_crc16.h" line="33">
<summary>
前一个循环冗余校验值和下一个要检查的数据字节。
</summary>
<param name="crc"></param>
<param name="c"></param>
<returns></returns>
</member>
<member name="M:init_crc16_tab" decl="true" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\crc\bsp_crc16.h" line="41">
<summary>
为了获得最佳性能，使用CRC16例程查找带有值的表
</summary>
<param name=""></param>
</member>
<member name="M:Get_CRC8_Check(System.Byte*,System.UInt16)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.cpp" line="29">
<summary>
获取CRC8校验码
</summary>
<param name="pchMessage"></param>
<param name="dwLength"></param>
<returns></returns>
</member>
<member name="M:CRC8_Check_Sum(System.Byte*,System.UInt16)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.cpp" line="40">
<summary>
检验CRC8数据段
</summary>
<param name="pchMessage"></param>
<param name="dwLength"></param>
<returns></returns>
</member>
<member name="M:Get_CRC16_Check(System.Byte*,System.UInt32)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.cpp" line="55">
<summary>
获取CRC16校验码
</summary>
<param name="pchMessage"></param>
<param name="dwLength"></param>
<returns></returns>
</member>
<member name="M:CRC16_Check_Sum(System.Byte*,System.UInt32)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.cpp" line="66">
<summary>
检验CRC16数据段
</summary>
<param name="pchMessage"></param>
<param name="dwLength"></param>
<returns></returns>
</member>
<member name="M:check_protocol_heade(System.Byte*)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.cpp" line="83">
<summary>
检查帧头
</summary>
<param name="pData"></param>
<returns></returns>
</member>
<member name="M:init_frame_struct(frame_struct*,System.UInt16)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.cpp" line="100">
<summary>
方式一-&gt;初始化frame_struct 自动分配地址
</summary>
<param name="pFrameStruct"></param>
<param name="uLen"></param>
<returns></returns>
</member>
<member name="M:init_frame_pointer(frame_struct*,System.Void*,System.UInt16)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.cpp" line="129">
<summary>
方式二-&gt;初始化frame_struct 外部分配地址 该方式更方便修改数据
</summary>
<param name="pFrameStruct"></param>
<param name="pFrameSt"></param>
<param name="uLen"></param>
<returns></returns>
</member>
<member name="M:init_message_struct(message_struct*,System.UInt16)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.cpp" line="160">
<summary>
方式一 -&gt;初始化message_struct
</summary>
<param name="pMessageStruct"></param>
<param name="uLen"></param>
<returns></returns>
</member>
<member name="M:init_message_pointer(message_struct*,System.Void*,System.UInt16)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.cpp" line="188">
<summary>
方式二 -&gt;初始化message_struct
</summary>
<param name="pMessageStruct"></param>
<param name="pMessageSt"></param>
<param name="uLen"></param>
<returns></returns>
</member>
<member name="M:get_protocol_size(System.UInt16)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.cpp" line="218">
<summary>
根据data_union长度计算数据帧长度，用于分配内存
</summary>
<param name="uLen"></param>
<returns></returns>
</member>
<member name="M:init_protocol(protocol_struct*,System.UInt16)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.cpp" line="228">
<summary>
方式一 初始化,并初始化内存
</summary>
<param name="pProtocol"></param>
<param name="uLen"></param>
</member>
<member name="M:init_protocol_pointer(protocol_struct*,System.Void*,System.Void*,System.UInt16)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.cpp" line="250">
<summary>
方式二 外部预先分配好内存空间
</summary>
<param name="pProtocol"></param>
<param name="pFrameSt"></param>
<param name="pMessageSt"></param>
<param name="uLen"></param>
<returns></returns>
</member>
<member name="M:make_protocol(protocol_struct*)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.cpp" line="278">
<summary>
生成带发送的数据内容
</summary>
<param name="pProtocol"></param>
<returns></returns>
</member>
<member name="M:parse_protocol(protocol_struct*,System.UInt16)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.cpp" line="335">
<summary>
解析接收到的数据
</summary>
<param name="pProtocol"></param>
<returns></returns>
</member>
<member name="M:deinit_protocol(protocol_struct*)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.cpp" line="409">
<summary>
反初始化，释放内存,如果是方式二创建然后反初始化，请注意规避野指针
</summary>
<param name="pProtocol"></param>
</member>
<member name="M:get_user_data_point(protocol_struct*)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.cpp" line="423">
<summary>
提供给用户，直接操作数据，但是请注意不要超过数据长度，避免内存越界
</summary>
<param name="pProtocol"></param>
<returns></returns>
</member>
<member name="M:get_message_point(protocol_struct*)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\bsp_protocol.cpp" line="433">
<summary>
提供给用户，直接操作数据，但是请注意不要超过数据长度，避免内存越界
</summary>
<param name="pProtocol"></param>
<returns></returns>
</member>
</members>
</doc>