---
# 这是文章的标题
title: 通信协议 
# 这是页面的图标
icon: bookmarks_line
# 这是侧边栏的顺序
order: 3
# 如果你不希望该列表包含一些特定的文章
article: false
# 设置作者
author: vSeasky
# 一个页面可以有多个分类
category:
  - VersaAssistant 
  - 通信协议
# 一个页面可以有多个标签
tag:
  - VersaAssistant 
  - 通信协议
---

# 通信协议

## 接口协议说明

- 以下所有低位在前发送

### 通信协议格式

| **帧头**            | **数据段**                                   | **帧尾**                           |
|-------------------|-------------------------------------------|----------------------------------|
| *header*(*5byte*) | *frame_user*((*9*+(*uLen* \* *4*))*byte*) | *frame_tail*(*2byte*，crc16，整包校验) |

### 帧头 *header* 定义

字节偏移 [*0*]，字节大小 [*5byte*]

| *帧头*          | 偏移位置 | 字节大小 | 内容                                                 |
|---------------|------|------|----------------------------------------------------|
| *fixed*       | 0    | 2    | 数据帧起始字节，固定值为 `0x5A53`                              |
| *data_length* | 2    | 2    | 数据帧 *frame_user* 中 *data* 的长度 *uLen \*4*，单位 *byte* |
| *crc_check*   | 4    | 1    | 帧头crc校验                                            |

### 数据段 *frame_user* 定义

字节偏移 [*5*]，字节大小 [(*9*+(*uLen* \* *4*))*byte*]

| *数据*          | 偏移位置 | 字节大小      | 功能说明                                   |
|---------------|------|-----------|----------------------------------------|
| *cmd*         | 5    | 1         | 功能码。                                   |
| *device_type* | 6    | 2         | 模块类型，可以区分传感器、控制器、或产品类型，对于同一个分类，该值一般固定。 |
| *device_id*   | 8    | 4         | 模块ID，可以区分同一个传感器、控制器、或产品的不同个体，一般需要可以设置。 |
| *data_id*     | 12   | 2         | 数据ID，可以指定模块的某类数据，具体由模块本身决定。            |
| *data*        | 14   | *uLen\*4* | 数据                                     |

### 帧尾 *frame_tail* (crc16，整包校验)

字节偏移 [*5* + *9* + (*uLen* * *4byte*)]，字节大小 [*2*]

| *帧尾*         | 偏移位置                         | 字节大小 | 功能说明 |
|--------------|------------------------------|------|------|
| *frame_tail* | *5* + *9*+(*uLen* * *4byte*) | 2    | 帧尾校验 |

### 通信协议依赖文件(CRC8,CRC16算法)

- bsp_crc16.h
- bsp_crc16.cpp
- bsp_crc8.h
- bsp_crc8.cpp

### 协议处理文件

- bsp_protocol.cpp
- bsp_protocol.h

## 使用方法

### 流程

1. 需要自行实现物理意义的通信功能。
2. 使用 `bsp_protocol` 提供的接口进行数据处理，然后通过自己实现的接口进行数据收发。

初始化结构体

```cpp
// 定义数据发送处理结构体
protocol_struct protocol;
// 初始化协议处理结构体,txMaxCount为允许的最大float可变数据长度，不能大于 MAX_DATA_SIZE = 28
init_protocol(&protocol, txMaxCount);
```

发送数据

```c
// 设置数据类型
protocol.frame.frame_user.header.cmd = 
protocol.frame.frame_user.header.device_type = 
protocol.frame.frame_user.header.device_id = 
protocol.frame.frame_user.header.data_id = 
// data_len 需要小于 max_data_len 
protocol.frame.frame_user.data.data_len = 
// 设置数据
for(uint16_t row=0;row<protocol.frame.frame_user.data.data_len;row++)
{
    if (row < protocol.frame.frame_user.data.data_len)
    {
        protocol.frame.frame_user.data.pData[row] = 
    }
}
make_protocol(protocol);

// 需要自行实现通信接口，如果是串口，务必使用DMA提升稳定性，非DMA发送的数据时序一般不是连续的，容易断帧
transmit(protocol.message.pData,protocolStruct.message.data_len);
```

接收数据

```c
receive(your_data,current_len);
memcpy(&pProtocolVersaRx->message.pData[0], your_data, current_len);

// 可以用 result 的值进行数据断帧检查，在此不做演示，断帧检查需要重复遍历，较为消耗资源，建议使用DMA和空闲中断接收方式，实现第一层的数据帧处理。然后再利用 result 进行二次检验
result = parse_protocol(protocol, current_len);
if(result == PROTOCOL_RESULT_OK)
{
    // 使用数据
    protocol.frame.frame_user.header.cmd;
    protocol.frame.frame_user.header.device_type;
    protocol.frame.frame_user.header.device_id;
    protocol.frame.frame_user.header.data_id;
    protocol.frame.frame_user.data.data_len;
    protocol.frame.frame_user.data.pData[];
}
```

释放资源

```c
deinit_protocol(&protocol);
memset(&protocol, 0, sizeof(protocol));
protocol.frame.frame_user.data.pData = NULL;
protocol.message.pData = NULL;
```

注意:为了避免数据冲突，如需发送和接收，建议定义两个结构体分别处理数据。

```c
protocol_struct protocolTx;
protocol_struct protocolRx;
init_protocol(&protocolTx, txMaxCount);
init_protocol(&protocolRx, txMaxCount);
```

### 案例

Qt中的案例,初始化和数据赋值不做演示。

接收

```cpp
void VersaPhysicalCom::parseProtocol(QByteArray &data)
{
	int result;
	if (pProtocolVersaRx != nullptr)
	{
		if (lastProtocolStartIndex > 0)
		{
			data.remove(0, lastProtocolStartIndex);
			lastProtocolStartIndex = 0;
		}
		// 获取帧头索引
		startProtocolIndex = data.indexOf(PROTOCOL_HEAD_ID, lastProtocolStartIndex);
		if (startProtocolIndex != -1)
		{
			const char *pParseData = data.constData() + startProtocolIndex;
			int dataSize = data.size();
			
			int currentParseLength = qMin((int) (dataSize - startProtocolIndex),
			                              (int) (pProtocolVersaRx->message.max_data_len));
			
			if ((lastProtocolStartIndex <= startProtocolIndex) &&
			    (currentParseLength >= DATAS_MIN_SIZE))
			{
				memcpy(&pProtocolVersaRx->message.pData[0], pParseData + startProtocolIndex, currentParseLength);
				result = parse_protocol(pProtocolVersaRx, currentParseLength);
				switch (result)
				{
					// 解析成功
					case PROTOCOL_RESULT_OK:
					{
						// 记录成功的位置，下一次的位置在此之后
						lastProtocolStartIndex = startProtocolIndex + pProtocolVersaRx->message.data_len;
						emit refreshProtocolRx();
						// 还可以继续解析
						if (lastProtocolStartIndex + DATAS_MIN_SIZE < dataSize)
						{
							parseProtocol(data);
						}
						break;
					}
					case PROTOCOL_RESULT_ERR:
					{
						// 转到下一个帧头检测
						lastProtocolStartIndex = startProtocolIndex + 2;
						parseProtocol(data);
						break;
					}
					case PROTOCOL_RESULT_CHECK_HEAD_ERR:
					{
						// 转到下一个帧头检测
						lastProtocolStartIndex = startProtocolIndex + 2;
						parseProtocol(data);
						break;
					}
					case PROTOCOL_RESULT_CHECK_FRAME_ERR:
					{
						// 数据帧头检查成功，但是数据损坏，移动到帧头之后检查新数据包，损坏数据包丢弃
						lastProtocolStartIndex = startProtocolIndex + DATAS_OFFSET_ADDR;
						parseProtocol(data);
						break;
					}
					case PROTOCOL_RESULT_OUT_OF_MSG_LEN:
					{
						// 超出解析长度，数据包出错，解析下一个数据包
						lastProtocolStartIndex = startProtocolIndex + DATAS_OFFSET_ADDR;
						parseProtocol(data);
						break;
					}
					case PROTOCOL_RESULT_OUT_OF_DATA_LEN:
					{
						// 通过包头计算，还未收到完整的数据包，等待下一个数据包来即可。
						break;
					}
					default:
					{
						// 转到下一个帧头检测
						lastProtocolStartIndex = startProtocolIndex + 2;
						parseProtocol(data);
					}
				}
			}
		}
	}
}
```

发送

```cpp
// const QString &address, quint16 port 是网络调试模式需要的参数，和协议无关
void VersaPhysicalCom::protocolVersaWrite(const QString &address, quint16 port)
{
	if (pProtocolVersaTx != nullptr)
	{
		make_protocol(pProtocolVersaTx);
		QByteArray writeData(reinterpret_cast<char *>(&pProtocolVersaTx->message.pData[0]),
		                     pProtocolVersaTx->message.data_len);
		// 124 字节只能收到 62字节，感觉是WIN的bug
		if (pProtocolVersaTx->message.data_len == 124)
		{
			writeData.append('\0');
		}
		if ((mVersaProtocolIndex == eProtocolVersa) &&
		    (writeData.size() > 0))
		{
			startWrite(address, port, writeData);
		}
	}
}
```