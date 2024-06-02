# 通信协议

## 接口协议说明

- 以下所有低位在前发送

1. 通信协议格式

| **帧头**          | **数据段**                                | **帧尾**                               |
| ----------------- | ----------------------------------------- | -------------------------------------- |
| *header*(*5byte*) | *frame_user*((*9*+(*uLen* \* *4*))*byte*) | *frame_tail*(*2byte*，crc16，整包校验) |

2. 帧头 *header* 定义

字节偏移 [*0*]，字节大小 [*5byte*]

| *帧头*        | 偏移位置 | 字节大小 | 内容                                                         |
| ------------- | -------- | -------- | ------------------------------------------------------------ |
| *fixed*       | 0        | 2        | 数据帧起始字节，固定值为 `0x5A53`                            |
| *data_length* | 2        | 2        | 数据帧 *frame_user* 中 *data* 的长度 *uLen \*4*，单位 *byte* |
| *crc_check*   | 4        | 1        | 帧头crc校验                                                  |

3. 数据段 *frame_user* 定义

字节偏移 [*5*]，字节大小 [(*9*+(*uLen* \* *4*))*byte*]

| *数据*        | 偏移位置 | 字节大小  | 功能说明                                                                     |
| ------------- | -------- | --------- | ---------------------------------------------------------------------------- |
| *cmd*         | 5        | 1         | 功能码。                                                                     |
| *device_type* | 6        | 2         | 模块类型，可以区分传感器、控制器、或产品类型，对于同一个分类，该值一般固定。 |
| *device_id*   | 8        | 4         | 模块ID，可以区分同一个传感器、控制器、或产品的不同个体，一般需要可以设置。   |
| *data_id*     | 12       | 2         | 数据ID，可以指定模块的某类数据，具体由模块本身决定。                         |
| *data*        | 14       | *uLen\*4* | 数据                                                                         |

4. 帧尾 *frame_tail* (crc16，整包校验)

字节偏移 [*5* + *9* + (*uLen* * *4byte*)]，字节大小 [*2*]

| *帧尾*       | 偏移位置                     | 字节大小 | 功能说明 |
| ------------ | ---------------------------- | -------- | -------- |
| *frame_tail* | *5* + *9*+(*uLen* * *4byte*) | 2        | 帧尾校验 |

5. 通信协议接口 

    > bsp_protocol.h
    >
    > bsp_protocol.cpp

    ```c++
    #include <stdio.h>
    
    #ifndef __MICROLIB
    
    #include <malloc.h >
    #include <memory.h>
    // #include "bsp_protocol_class.h"
    
    #endif
    
    #include "bsp_protocol.h"
    #include "./crc/bsp_crc8.h"
    #include "./crc/bsp_crc16.h"
    
    // #define PROTOCOL_CPP_CLR_DEBUG
    // #define PROTOCOL_C_DEBUG 1
    
    #ifdef PROTOCOL_CPP_CLR_DEBUG
    
    #elif PROTOCOL_C_DEBUG
    #define bsp_debug_c(lev, Format, ...)   printf("%s:%d: " Format, __FILE__, __LINE__, ##__VA_ARGS__)
    #else
    #define bsp_debug_c(...)
    #endif // !CPP_DEBUG
    
    #define PROTOCOL_DEBUG_PRINTF(Format, ...)         bsp_debug_c(LOG_LEVEL_DEBUG,Format, ##__VA_ARGS__)
    #define PROTOCOL_INFO__PRINTF(Format, ...)         bsp_debug_c(LOG_LEVEL_INFO_,Format, ##__VA_ARGS__)
    #define PROTOCOL_WARN__PRINTF(Format, ...)         bsp_debug_c(LOG_LEVEL_WARN_,Format, ##__VA_ARGS__)
    #define PROTOCOL_ERROR_PRINTF(Format, ...)         bsp_debug_c(LOG_LEVEL_ERROR,Format, ##__VA_ARGS__)
    #define PROTOCOL_FATAL_PRINTF(Format, ...)         bsp_debug_c(LOG_LEVEL_FATAL,Format, ##__VA_ARGS__)
    
    /// <summary>
    /// 获取CRC8校验码
    /// </summary>
    /// <param name="pchMessage"></param>
    /// <param name="dwLength"></param>
    /// <returns></returns>
    static uint8_t Get_CRC8_Check(uint8_t *pchMessage, uint16_t dwLength)
    {
    	return crc_8(pchMessage, dwLength);
    }
    
    /// <summary>
    /// 检验CRC8数据段
    /// </summary>
    /// <param name="pchMessage"></param>
    /// <param name="dwLength"></param>
    /// <returns></returns>
    static uint8_t CRC8_Check_Sum(uint8_t *pchMessage, uint16_t dwLength)
    {
    	uint8_t ucExpected = 0;
    	if ((pchMessage == 0) || (dwLength <= 2))
    	{ return 0; }
    	ucExpected = Get_CRC8_Check(pchMessage, dwLength - 1);
    	return (ucExpected == pchMessage[dwLength - 1]);
    }
    
    
    /// <summary>
    /// 获取CRC16校验码
    /// </summary>
    /// <param name="pchMessage"></param>
    /// <param name="dwLength"></param>
    /// <returns></returns>
    static uint16_t Get_CRC16_Check(uint8_t *pchMessage, uint32_t dwLength)
    {
    	return crc_16(pchMessage, dwLength);
    }
    
    /// <summary>
    /// 检验CRC16数据段
    /// </summary>
    /// <param name="pchMessage"></param>
    /// <param name="dwLength"></param>
    /// <returns></returns>
    static uint16_t CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength)
    {
    	uint16_t wExpected = 0;
    	if ((pchMessage == 0) || (dwLength <= 2))
    	{
    		return 0;
    	}
    	wExpected = Get_CRC16_Check(pchMessage, dwLength - 2);
    	return (((wExpected & 0xff) == pchMessage[dwLength - 2]) &&
    	        (((wExpected >> 8) & 0xff) == pchMessage[dwLength - 1]));
    }
    
    /// <summary>
    /// 检查帧头
    /// </summary>
    /// <param name="pData"></param>
    /// <returns></returns>
    static int check_protocol_heade(uint8_t *pData)
    {
    	uint16_t frame_head;
    	frame_head = (pData[1] << 8) |
    	             (pData[0]);
    	if (frame_head == PROTOCOL_HEAD_ID)
    	{
    		if (CRC8_Check_Sum(&pData[0], FRAME_HEADER_LEN))
    		{
    			return PROTOCOL_RESULT_OK;
    		}
    	}
    	return PROTOCOL_RESULT_CHECK_HEAD_ERR;
    }
    
    /// <summary>
    /// 方式一->初始化frame_struct 自动分配地址
    /// </summary>
    /// <param name="pFrameStruct"></param>
    /// <param name="uLen"></param>
    /// <returns></returns>
    static int init_frame_struct(frame_data_struct *pFrameStruct, uint16_t uLen)
    {
    	user_data_struct *pUserDataStruct = &pFrameStruct->frame_user;
    	if ((pUserDataStruct->data.pData == NULL))
    	{
    		if (uLen <= MAX_DATA_SIZE)
    		{
    			pUserDataStruct->data.max_data_len = uLen;
    			// 为不定长数据包分配内存
    			pUserDataStruct->data.pData = (uint32_t *) malloc(
    					pUserDataStruct->data.max_data_len * (sizeof(uint32_t)));
    			// 初始化内存
    			memset(pUserDataStruct->data.pData, 0,
    			       pUserDataStruct->data.max_data_len * (sizeof(uint32_t)));
    			return PROTOCOL_RESULT_OK;
    		}
    		else
    		{
    			return PROTOCOL_RESULT_OUT_OF_MSG_LEN;
    		}
    	}
    	else
    	{
    		return PROTOCOL_RESULT_ERR;
    	}
    }
    
    /// <summary>
    /// 方式二->初始化frame_struct 外部分配地址 该方式更方便修改数据
    /// </summary>
    /// <param name="pFrameStruct"></param>
    /// <param name="pFrameSt"></param>
    /// <param name="uLen"></param>
    /// <returns></returns>
    static int init_frame_pointer(frame_data_struct *pFrameStruct, void *pFrameSt, uint16_t uLen)
    {
    	user_data_struct *pUserDataStruct = &pFrameStruct->frame_user;
    	if ((pUserDataStruct->data.pData == NULL))
    	{
    		if (uLen <= MAX_DATA_SIZE)
    		{
    			pUserDataStruct->data.max_data_len = uLen;
    			pUserDataStruct->data.pData = (uint32_t *) pFrameSt;
    			// pUserDataStruct->cmd_data.pData = (uint32_t*) malloc(pUserDataStruct->cmd_data.max_data_len * (sizeof(uint32_t)));
    			memset(pUserDataStruct->data.pData, 0, pUserDataStruct->data.max_data_len * (sizeof(uint32_t)));
    			return PROTOCOL_RESULT_OK;
    		}
    		else
    		{
    			return PROTOCOL_RESULT_OUT_OF_MSG_LEN;
    		}
    	}
    	else
    	{
    		return PROTOCOL_RESULT_ERR;
    	}
    }
    
    /// <summary>
    /// 方式一 ->初始化message_struct
    /// </summary>
    /// <param name="pMessageStruct"></param>
    /// <param name="uLen">数据包总长度</param>
    /// <returns></returns>
    static int init_message_struct(message_data_struct *pMessageStruct, uint16_t uLen)
    {
    	if ((pMessageStruct->pData == NULL))
    	{
    		if (uLen <= get_protocol_size(MAX_BUFFER_SIZE))
    		{
    			pMessageStruct->max_data_len = uLen;
    			pMessageStruct->pData = (uint8_t *) malloc(pMessageStruct->max_data_len * (sizeof(uint8_t)));
    			memset(pMessageStruct->pData, 0, pMessageStruct->max_data_len * (sizeof(uint8_t)));
    			return PROTOCOL_RESULT_OK;
    		}
    		else
    		{
    			return PROTOCOL_RESULT_OUT_OF_MSG_LEN;
    		}
    	}
    	else
    	{
    		return PROTOCOL_RESULT_ERR;
    	}
    }
    
    /// <summary>
    /// 方式二 ->初始化message_struct
    /// </summary>
    /// <param name="pMessageStruct"></param>
    /// <param name="pMessageSt"></param>
    /// <param name="uLen"></param>
    /// <returns></returns>
    static int init_message_pointer(message_data_struct *pMessageStruct, void *pMessageSt, uint16_t uLen)
    {
    	if ((pMessageStruct->pData == NULL))
    	{
    		if (uLen <= get_protocol_size(MAX_BUFFER_SIZE))
    		{
    			pMessageStruct->max_data_len = uLen;
    			pMessageStruct->pData = (uint8_t *) pMessageSt;
    			// pMessageStruct->pData = (uint8_t *)malloc(pMessageStruct->max_data_len * (sizeof(uint8_t)));
    			memset(pMessageStruct->pData, 0, pMessageStruct->max_data_len * (sizeof(uint8_t)));
    			return PROTOCOL_RESULT_OK;
    		}
    		else
    		{
    			return PROTOCOL_RESULT_OUT_OF_MSG_LEN;
    		}
    	}
    	else
    	{
    		return PROTOCOL_RESULT_ERR;
    	}
    }
    
    /// <summary>
    /// 根据data_union长度计算数据帧长度，用于分配内存
    /// </summary>
    /// <param name="uLen">数据长度</param>
    /// <returns></returns>
    uint16_t get_protocol_size(uint16_t uLen)
    {
    	return (uLen * sizeof(uint32_t) + DATAS_OFFSET_ADDR + 2);
    }
    
    /// <summary>
    /// 方式一 初始化,并初始化内存
    /// </summary>
    /// <param name="pProtocol"></param>
    /// <param name="uLen">数据长度</param>
    void init_protocol(protocol_struct *pProtocol, uint16_t uLen)
    {
    	int result = 0;
    	/*初始化crc*/
    	init_crc16_tab();
    	/*分配内存空间*/
    	result = init_frame_struct(&pProtocol->frame, uLen);
    	if (result != 0)
    	{
    		PROTOCOL_ERROR_PRINTF("pProtocol init_frame_struct err [%d]!\n", result);
    	}
    	else
    	{
    		PROTOCOL_DEBUG_PRINTF("pProtocol init_frame_struct ok [%d]!\n", result);
    	}
    	/*分配带解析数据的空间*/
    	result = init_message_struct(&pProtocol->message, get_protocol_size(uLen));
    	if (result != 0)
    	{
    		PROTOCOL_ERROR_PRINTF("pProtocol init_message_struct err [%d]!\n", result);
    	}
    	else
    	{
    		PROTOCOL_DEBUG_PRINTF("pProtocol init_message_struct ok [%d]!\n", result);
    	}
    }
    
    /// <summary>
    /// 方式二 外部预先分配好内存空间
    /// </summary>
    /// <param name="pProtocol"></param>
    /// <param name="pFrameSt"></param>
    /// <param name="pMessageSt"></param>
    /// <param name="uLen">数据长度</param>
    /// <returns></returns>
    int init_protocol_pointer(protocol_struct *pProtocol, void *pFrameSt, void *pMessageSt, uint16_t uLen)
    {
    	/*初始化crc*/
    	init_crc16_tab();
    	/*分配内存空间*/
    	if (init_frame_pointer(&pProtocol->frame, pFrameSt, uLen) != 0)
    	{
    		PROTOCOL_ERROR_PRINTF("pProtocol init_frame_struct err!\n");
    		return PROTOCOL_RESULT_ERR;
    	}
    	PROTOCOL_DEBUG_PRINTF("pProtocol init_frame_struct ok!\n");
    	if (init_message_pointer(&pProtocol->message, pMessageSt, get_protocol_size(uLen)) != 0)
    	{
    		PROTOCOL_ERROR_PRINTF("pProtocol init_message_struct err!\n");
    		return PROTOCOL_RESULT_ERR;
    	}
    	PROTOCOL_DEBUG_PRINTF("pProtocol init_message_struct ok!\n");
    	return PROTOCOL_RESULT_OK;
    }
    
    /// <summary>
    /// 生成带发送的数据内容
    /// </summary>
    /// <param name="pProtocol"></param>
    /// <returns></returns>
    int make_protocol(protocol_struct *pProtocol)
    {
    	int ret = -1;
    	uint16_t pos_offset_len;
    	frame_data_struct *pFrameStruct = &pProtocol->frame;
    	message_data_struct *pMessageStruct = &pProtocol->message;
    	// 更新头部数据
    	pFrameStruct->header.fixed_header = PROTOCOL_HEAD_ID;
    	pFrameStruct->header.data_length = pFrameStruct->frame_user.data.data_len * sizeof(uint32_t);
    	
    	// 帧头
    	pMessageStruct->pData[0] = (pFrameStruct->header.fixed_header >> 0) & 0xff;
    	pMessageStruct->pData[1] = (pFrameStruct->header.fixed_header >> 8) & 0xff;
    	
    	// 数据长度
    	pMessageStruct->pData[2] = (pFrameStruct->header.data_length) & 0xff;
    	pMessageStruct->pData[3] = (pFrameStruct->header.data_length >> 8) & 0xff;
    	
    	// 校验
    	pFrameStruct->header.crc_check = Get_CRC8_Check(&pMessageStruct->pData[0], FRAME_HEADER_LEN - 1);
    	pMessageStruct->pData[4] = pFrameStruct->header.crc_check;
    	
    	// 更新功能码
    	pMessageStruct->pData[5] = pFrameStruct->frame_user.header.cmd;
    	// 更新设备类型
    	pMessageStruct->pData[6] = (pFrameStruct->frame_user.header.device_type >> 0) & 0xff;
    	pMessageStruct->pData[7] = (pFrameStruct->frame_user.header.device_type >> 8) & 0xff;
    	// 更新设备ID
    	pMessageStruct->pData[8] = (pFrameStruct->frame_user.header.device_id >> 0) & 0xff;
    	pMessageStruct->pData[9] = (pFrameStruct->frame_user.header.device_id >> 8) & 0xff;
    	pMessageStruct->pData[10] = (pFrameStruct->frame_user.header.device_id >> 0) & 0xff;
    	pMessageStruct->pData[11] = (pFrameStruct->frame_user.header.device_id >> 8) & 0xff;
    	// 更新设备ID
    	pMessageStruct->pData[12] = (pFrameStruct->frame_user.header.data_id >> 0) & 0xff;
    	pMessageStruct->pData[13] = (pFrameStruct->frame_user.header.data_id >> 8) & 0xff;
    	
    	// 检查数据长度
    	if (pFrameStruct->frame_user.data.data_len <= pFrameStruct->frame_user.data.max_data_len)
    	{
    		// 计算数据长度
    		pos_offset_len = pFrameStruct->frame_user.data.data_len * sizeof(uint32_t);
    		
    		// 复制内存
    		// memset(&pMessageStruct->pData[DATAS_OFFSET_ADDR],
    		//        0,
    		//        pMessageStruct->max_data_len);
    		
    		// 复制内存
    		memcpy(&pMessageStruct->pData[DATAS_OFFSET_ADDR],
    		       &pFrameStruct->frame_user.data.pData[0],
    		       pos_offset_len);
    		
    		pos_offset_len = pFrameStruct->header.data_length + DATAS_OFFSET_ADDR;
    		//帧尾校验值
    		pFrameStruct->frame_tail = Get_CRC16_Check(&pMessageStruct->pData[0], pos_offset_len);
    		pMessageStruct->pData[pos_offset_len] = (pFrameStruct->frame_tail) & 0XFF;
    		pMessageStruct->pData[pos_offset_len + 1] = (pFrameStruct->frame_tail >> 8) & 0XFF;
    		pMessageStruct->data_len = pos_offset_len + 2;
    		
    		return PROTOCOL_RESULT_OK;
    	}
    	else
    	{
    		PROTOCOL_ERROR_PRINTF("make_protocol->>data_len[%d] > max_data_len[%d]!\n",
    		                      pFrameStruct->frame_user.data.data_len,
    		                      pFrameStruct->frame_user.data.max_data_len);
    		return PROTOCOL_RESULT_OUT_OF_MSG_LEN;
    	}
    	PROTOCOL_DEBUG_PRINTF("make_protocol->>data_len[%d],max_data_len[%d]!\n",
    	                      pFrameStruct->frame_user.data.data_len,
    	                      pFrameStruct->frame_user.data.max_data_len);
    	return PROTOCOL_RESULT_ERR;
    }
    
    /// <summary>
    /// 解析接收到的数据
    /// </summary>
    /// <param name="pProtocol"></param>
    /// <returns></returns>
    int parse_protocol(protocol_struct *pProtocol, uint16_t parseDataLen)
    {
    	// 解析数据，使用前需提前缓冲 pProtocol->message_st.pData
    	int ret = -1;
    	uint16_t pos_offset_len;
    	frame_data_struct *pFrameStruct = &pProtocol->frame;
    	message_data_struct *pMessageStruct = &pProtocol->message;
    	
    	if (check_protocol_heade(pMessageStruct->pData) == PROTOCOL_RESULT_OK)
    	{
    		// 更新帧头数据
    		pFrameStruct->header.fixed_header =
    				(pMessageStruct->pData[1] << 8) |
    				(pMessageStruct->pData[0]);
    		
    		// 获取data段的数据长度
    		pFrameStruct->header.data_length = (pMessageStruct->pData[3] << 8) |
    		                                   (pMessageStruct->pData[2] << 0);
    
    		pFrameStruct->header.crc_check = pMessageStruct->pData[4];
    		// 获取此次数据包长度
    		pMessageStruct->data_len = pFrameStruct->header.data_length + DATAS_OFFSET_ADDR + 2;
    		// 计算解析后得到的 data_union 数据长度
    		pFrameStruct->frame_user.data.data_len = (pFrameStruct->header.data_length) / sizeof(uint32_t);
    		// 检查数据包是否完整
    		if (pMessageStruct->data_len <= parseDataLen)
    		{
    			if (pMessageStruct->data_len <= pMessageStruct->max_data_len)
    			{
    				// 检查校验和
    				if (CRC16_Check_Sum(&pMessageStruct->pData[0], pMessageStruct->data_len) != 0)
    				{
    					pFrameStruct->frame_user.header.cmd = pMessageStruct->pData[5];
    					pFrameStruct->frame_user.header.device_type =
    							(pMessageStruct->pData[7] << 8) |
    							(pMessageStruct->pData[6] << 0);
    					
    					pFrameStruct->frame_user.header.device_id =
    							(pMessageStruct->pData[11] << 24) |
    							(pMessageStruct->pData[10] << 16) |
    							(pMessageStruct->pData[9] << 8) |
    							(pMessageStruct->pData[8] << 0);
    					
    					pFrameStruct->frame_user.header.data_id =
    							(pMessageStruct->pData[13] << 8) |
    							(pMessageStruct->pData[12] << 0);
    					
    					// 拷贝 data段 指定长度数据
    					memcpy(&pFrameStruct->frame_user.data.pData[0],
    					       &pMessageStruct->pData[DATAS_OFFSET_ADDR],
    					       pFrameStruct->header.data_length);
    					
    					// 计算不包含帧尾校验的数据长度
    					pos_offset_len = pFrameStruct->header.data_length + DATAS_OFFSET_ADDR;
    					
    					pFrameStruct->frame_tail =
    							(pMessageStruct->pData[pos_offset_len + 1] << 8) | (pMessageStruct->pData[pos_offset_len]);
    					return PROTOCOL_RESULT_OK;
    				}
    				else
    				{
    					// 检查校验和失败
    					PROTOCOL_ERROR_PRINTF("parse_protocol->>CRC16_Check_Sum err!\n");
    					return PROTOCOL_RESULT_CHECK_FRAME_ERR;
    				}
    			}
    			else
    			{
    				// 待解析BUFF超过预定解析数据容量，避免内存越界
    				PROTOCOL_ERROR_PRINTF("parse_protocol->>data_len[%d] > max_data_len[%d]!\n",
    				                      pMessageStruct->data_len,
    				                      pMessageStruct->max_data_len);
    				return PROTOCOL_RESULT_OUT_OF_MSG_LEN;
    			}
    		}
    		else
    		{
    			// 通过包头计算，还未收到完整的数据包
    			PROTOCOL_ERROR_PRINTF("parse_protocol->>data_len[%d] > max_data_len[%d]!\n",
    			                      pMessageStruct->data_len,
    			                      pMessageStruct->max_data_len);
    			return PROTOCOL_RESULT_OUT_OF_DATA_LEN;
    		}
    	}
    	else
    	{
    		// 帧头检测失败
    		PROTOCOL_ERROR_PRINTF("parse_protocol->>check_protocol_heade err!\n");
    		return PROTOCOL_RESULT_CHECK_HEAD_ERR;
    	}
    	PROTOCOL_DEBUG_PRINTF("parse_protocol->>check_protocol_heade ok!\n");
    	return PROTOCOL_RESULT_ERR;
    }
    
    
    /// <summary>
    /// 反初始化，释放内存,如果是方式二创建然后反初始化，请注意规避野指针
    /// </summary>
    /// <param name="pProtocol"></param>
    void deinit_protocol(protocol_struct *pProtocol)
    {
    	//指针为空不需要判断
    	free(pProtocol->frame.frame_user.data.pData);
    	free(pProtocol->message.pData);
    	//memset(pProtocol, 0, sizeof(protocol_struct));
    	pProtocol->frame.frame_user.data.pData = NULL;
    	pProtocol->message.pData = NULL;
    }
    
    /// <summary>
    /// 提供给用户，直接操作数据，但是请注意不要超过数据长度，避免内存越界
    /// </summary>
    /// <param name="pProtocol"></param>
    /// <returns></returns>
    const user_data_struct *get_user_data_point(protocol_struct *pProtocol)
    {
    	return &pProtocol->frame.frame_user;
    }
    
    /// <summary>
    /// 提供给用户，直接操作数据，但是请注意不要超过数据长度，避免内存越界
    /// </summary>
    /// <param name="pProtocol"></param>
    /// <returns></returns>
    const message_data_struct *get_message_point(protocol_struct *pProtocol)
    {
    	return &pProtocol->message;
    }
    ```

    ```c++
    #ifndef _BSP_PROTOCOL_H_
    #define _BSP_PROTOCOL_H_
    
    
    #ifdef __cplusplus
    extern "C" {
    #endif
    
    #include <stdio.h>
    #include <stdint.h>
    #include <string.h>
    
    
    #define PROTOCOL_HEAD_ID   0x5A53
    
    #define FRAME_HEADER_LEN  5 // 总数据包帧头数据长度
    #define DATAS_HEADER_LEN  9 // 不定长数据包头数据长度
    
    // 产品类型
    #define FRAME_HEADER_ADDR 0
    #define DATAS_HEADER_ADDR FRAME_HEADER_LEN
    #define DATAS_OFFSET_ADDR (FRAME_HEADER_LEN+DATAS_HEADER_LEN)  //不定长数据包偏移地址
    #define DATAS_MIN_SIZE (FRAME_HEADER_LEN+DATAS_HEADER_LEN+2)
    #define MAX_DATA_SIZE     28
    #define MAX_BUFFER_SIZE   (MAX_DATA_SIZE*4+16)
    
    #define LOG_LEVEL_DEBUG (1<<0)
    #define LOG_LEVEL_INFO_ (1<<1)
    #define LOG_LEVEL_WARN_ (1<<2)
    #define LOG_LEVEL_ERROR (1<<3)
    #define LOG_LEVEL_FATAL (1<<4)
    
    typedef enum
    {
    	PROTOCOL_RESULT_OK = 0
    	, PROTOCOL_RESULT_ERR = 1
    	, PROTOCOL_RESULT_CHECK_HEAD_ERR = 2
    	, PROTOCOL_RESULT_CHECK_FRAME_ERR = 3
    	, PROTOCOL_RESULT_OUT_OF_MSG_LEN = 4,
    	PROTOCOL_RESULT_OUT_OF_DATA_LEN = 5
    	, } protocol_result;
    //32位
    typedef union
    {
    	char data_char[4];
    	uint8_t data_u8[4];
    	int16_t data_int16[2];
    	uint16_t data_u16[2];
    	int32_t data_int32;
    	uint32_t data_u32;
    	float data_float;
    } data_union;
    
    // 用户可以使用的数据
    typedef struct
    {
    	struct
    	{
    		/*字节偏移 7 -->> 字节大小 1*/
    		uint8_t cmd;   // 功能码
    		/*字节偏移 8 -->> 字节大小 2*/
    		uint16_t device_type;  // 设备类型
    		/*字节偏移 10 -->> 字节大小 2*/
    		uint32_t device_id;    // 设备ID
    		/*字节偏移 12 -->> 字节大小 2*/
    		uint16_t data_id;      // 数据ID
    	} header;
    	struct
    	{
    		uint16_t data_len;     // data_union数据长度
    		uint16_t max_data_len; // 数据最长长度,单位 sizeof(uint32_t)
    		uint32_t *pData;       // 数据(data_union),推荐使用 data_union * pDataUnion = &pData[];
    	} data;
    } user_data_struct; //数据内容
    
    // 数据接收和发送都是以这种格式
    typedef struct
    {
    	uint8_t *pData;        //数据
    	uint16_t data_len;     //数据总长度
    	uint16_t max_data_len; //数据最长长度
    } message_data_struct;     //消息数据
    
    typedef struct
    {
    	/*字节偏移 0 -->> 字节大小 7*/
    	struct
    	{
    		uint16_t fixed_header;      //固定帧头
    		uint16_t data_length;       //数据长度,单位byte
    		uint8_t crc_check;          //帧头CRC校验
    	} header;                       //数据帧头
    	user_data_struct frame_user;    //用户数据
    	uint16_t frame_tail;//帧尾CRC校验
    } frame_data_struct;
    
    typedef struct
    {
    	frame_data_struct frame;           //原始数据或解析后的数据
    	message_data_struct message;  //有待解析或生成的数据
    } protocol_struct;
    
    
    /// <summary>
    /// 根据data_union长度计算数据帧长度，用于分配内存
    /// </summary>
    /// <param name="uLen">数据长度</param>
    /// <returns></returns>
    uint16_t get_protocol_size(uint16_t uLen);
    
    /// <summary>
    /// 方式一 初始化,并初始化内存
    /// </summary>
    /// <param name="pProtocol"></param>
    /// <param name="uLen">数据长度</param>
    void init_protocol(protocol_struct *pProtocol, uint16_t uLen);
    
    /// <summary>
    /// 方式二 外部预先分配好内存空间
    /// </summary>
    /// <param name="pProtocol"></param>
    /// <param name="pFrameSt"></param>
    /// <param name="pMessageSt"></param>
    /// <param name="uLen">数据长度</param>
    /// <returns></returns>
    int init_protocol_pointer(protocol_struct *pProtocol, void *pFrameSt, void *pMessageSt, uint16_t uLen);
    
    /// <summary>
    /// 生成带发送的数据内容
    /// </summary>
    /// <param name="pProtocol"></param>
    /// <returns></returns>
    int make_protocol(protocol_struct *pProtocol);
    
    /// <summary>
    /// 解析接收到的数据
    /// </summary>
    /// <param name="pProtocol"></param>
    /// <returns></returns>
    int parse_protocol(protocol_struct *pProtocol, uint16_t parseDataLen);
    
    /// <summary>
    /// 反初始化，释放内存,如果是方式二创建然后反初始化，请注意规避野指针
    /// </summary>
    /// <param name="pProtocol"></param>
    void deinit_protocol(protocol_struct *pProtocol);
    
    
    /// <summary>
    /// 提供给用户，直接操作数据，但是请注意不要超过数据长度，避免内存越界
    /// </summary>
    /// <param name="pProtocol"></param>
    /// <returns></returns>
    const user_data_struct *get_user_data_point(protocol_struct *pProtocol);
    
    
    /// <summary>
    /// 提供给用户，直接操作数据，但是请注意不要超过数据长度，避免内存越界
    /// </summary>
    /// <param name="pProtocol"></param>
    /// <returns></returns>
    const message_data_struct *get_message_point(protocol_struct *pProtocol);
    
    #ifdef __cplusplus
    }
    #endif
    
    #endif
    ```

6. 通信协议依赖文件(CRC8,CRC16算法)

> bsp_crc16.h
> bsp_crc16.cpp
> bsp_crc8.h
> bsp_crc8.cpp

7. 跨平台

> cpp改为c可直接支持c语言

