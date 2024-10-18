/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-07-01 19:59:07
 * @FilePath     : \VersaAssistant-STM32\VersaAssistant\Peripheral\bsp_versaassistant.c
 * @Description  : 
 */
#include "bsp_versaassistant.h"

#include "bsp_protocol.h"
#include "main.h"
#include "bsp_versaassistant.h"

#define BSP_PROTOCOL_LEN 12

uint32_t pTxData[BSP_PROTOCOL_LEN];
uint8_t  pTxBuffer[BSP_PROTOCOL_LEN * 4 + DATAS_MIN_SIZE];
uint32_t pRxData[BSP_PROTOCOL_LEN];
uint8_t  pRxBuffer[BSP_PROTOCOL_LEN * 4 + DATAS_MIN_SIZE];

protocol_struct pTxProtocol;
protocol_struct pRxProtocol;

uint8_t bsp_versa_type_transmit = 0;


extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

void bsp_versa_init(void)
{
    pTxProtocol.frame.frame_user.data.pData = NULL;
    pTxProtocol.message.pData = NULL;
    init_protocol_pointer(&pTxProtocol, pTxData, pTxBuffer, BSP_PROTOCOL_LEN);
    pRxProtocol.frame.frame_user.data.pData = NULL;
    pRxProtocol.message.pData = NULL;
    init_protocol_pointer(&pRxProtocol, pRxData, pRxBuffer, BSP_PROTOCOL_LEN);
}



void bsp_versa_transmit(uint8_t *pData, uint16_t len)
{
    switch(bsp_versa_type_transmit)
    {
        case 0:
        {
            
        }break;
        case 1:
        {
                HAL_UART_Transmit_DMA(&huart1, pData, len);
        }break;
        case 2:
        {
            extern void cdc_acm_data_send(uint8_t busid,uint8_t *txBuffer,uint16_t ulen);
            cdc_acm_data_send(0,pData,len);
        }break;
        case 3:
        {
            extern void hid_data_send(uint8_t busid,uint8_t *txBuffer,uint16_t ulen);
            hid_data_send(0,pData,len);
            
        }break;
    }
}

void bsp_versa_receive(uint8_t *rxData, uint8_t this_rx_buff_len,uint8_t type)
{
    memcpy(pRxBuffer,rxData,this_rx_buff_len);
    // 进行数据解析
    parse_protocol(&pRxProtocol,this_rx_buff_len);
}