<?xml version="1.0"?><doc>
<members>
<member name="M:crc_16(System.Byte!System.Runtime.CompilerServices.IsConst*,System.UInt16)" decl="true" source="E:\projects\VersaAssistant\VersaProtocol\Protocol\crc\bsp_crc16.h" line="17">
<summary>
函数crc_16()一次计算一个字节的16位CRC16
</summary>
<param name="input_str">字符串</param>
<param name="num_bytes">字节数</param>
<returns></returns>
</member>
<member name="M:crc_modbus(System.Byte!System.Runtime.CompilerServices.IsConst*,System.UInt16)" decl="true" source="E:\projects\VersaAssistant\VersaProtocol\Protocol\crc\bsp_crc16.h" line="25">
<summary>
一次计算16位modbus循环冗余校验
</summary>
<param name="input_str">字符串</param>
<param name="num_bytes">字节数</param>
<returns></returns>
</member>
<member name="M:update_crc_16(System.UInt16,System.Byte)" decl="true" source="E:\projects\VersaAssistant\VersaProtocol\Protocol\crc\bsp_crc16.h" line="33">
<summary>
前一个循环冗余校验值和下一个要检查的数据字节。
</summary>
<param name="crc"></param>
<param name="c"></param>
<returns></returns>
</member>
<member name="M:init_crc16_tab" decl="true" source="E:\projects\VersaAssistant\VersaProtocol\Protocol\crc\bsp_crc16.h" line="41">
<summary>
为了获得最佳性能，使用CRC16例程查找带有值的表
</summary>
<param name=""></param>
</member>
<!-- 丢弃成员“M:crc_16(System.Byte!System.Runtime.CompilerServices.IsConst*,System.UInt16)”的格式错误的 XML 文档注释。-->
<!-- 丢弃成员“M:crc_modbus(System.Byte!System.Runtime.CompilerServices.IsConst*,System.UInt16)”的格式错误的 XML 文档注释。-->
<member name="M:update_crc_16(System.UInt16,System.Byte)" decl="false" source="E:\projects\VersaAssistant\VersaProtocol\Protocol\crc\bsp_crc16.cpp" line="55">
<summary>
鍓嶄竴涓惊鐜啑浣欐牎楠屽€煎拰涓嬩竴涓妫€鏌ョ殑鏁版嵁瀛楄妭銆?
</summary>
<param name="crc"></param>
<param name="c"></param>
<returns></returns>
</member>
<member name="M:init_crc16_tab" decl="false" source="E:\projects\VersaAssistant\VersaProtocol\Protocol\crc\bsp_crc16.cpp" line="68">
<summary>
涓轰簡鑾峰緱鏈€浣虫€ц兘锛屼娇鐢–RC16渚嬬▼鏌ユ壘甯︽湁鍊肩殑琛?
</summary>
<param name=""></param>
</member>
</members>
</doc>