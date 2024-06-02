<?xml version="1.0"?><doc>
<members>
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
<member name="M:crc_16(System.Byte!System.Runtime.CompilerServices.IsConst*,System.UInt16)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\crc\bsp_crc16.cpp" line="9">
<summary>
函数crc_16()一次计算一个字节的16位CRC16
</summary>
<param name="input_str">字符串</param>
<param name="num_bytes">字节数</param>
<returns></returns>
</member>
<member name="M:crc_modbus(System.Byte!System.Runtime.CompilerServices.IsConst*,System.UInt16)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\crc\bsp_crc16.cpp" line="32">
<summary>
一次计算16位modbus循环冗余校验
</summary>
<param name="input_str">字符串</param>
<param name="num_bytes">字节数</param>
<returns></returns>
</member>
<member name="M:update_crc_16(System.UInt16,System.Byte)" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\crc\bsp_crc16.cpp" line="58">
<summary>
前一个循环冗余校验值和下一个要检查的数据字节。
</summary>
<param name="crc"></param>
<param name="c"></param>
<returns></returns>
</member>
<member name="M:init_crc16_tab" decl="false" source="E:\KmdFoc\Tools\NET Framework 4.8\vSeaskyPort\Protocol\crc\bsp_crc16.cpp" line="71">
<summary>
为了获得最佳性能，使用CRC16例程查找带有值的表
</summary>
<param name=""></param>
</member>
</members>
</doc>