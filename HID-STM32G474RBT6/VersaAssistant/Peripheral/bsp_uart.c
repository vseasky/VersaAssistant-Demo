/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-19 21:33:33
 * @FilePath     : \MDK-ARMe:\Projects\KmdFoc-G4\software\KmdFocG4\KmdFoc\Peripheral\bsp_uart.c
 * @Description  : 
 */
#include "bsp_uart.h"
#include "bsp_versaassistant.h"
#include "main.h"

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;


#define USART_TX_BUFF_SIZE 128U
#define USART_RX_BUFF_SIZE 128U

// #define MX_UART2_BAUDRATE (256000)
// #define USART2_DATA_ADDRESS ((uint32_t)&USART_DATA(USART2))

// 分配串口收发所需内存
uint8_t UartRxBuffer[USART_RX_BUFF_SIZE];
uint8_t UartTxBuffer[USART_TX_BUFF_SIZE];

uint16_t usart_tx_len = 0;

/*依赖buff*/

/*待发送数据*/
uint16_t usart_tx_flag = 0;
float usart_tx[10];

/*接收原始数据*/
static uint16_t usart_rx_pos = 0;
/*按协议处理后的数据*/
uint16_t usart_rx_id = 0;
uint16_t usart_rx_flag = 0;
float usart_rx[10];

/**
 * @description: 初始化串口
 * @return {*}
 */
void bsp_uart_init(void)
{
    HAL_UART_Receive_DMA(&huart1, UartRxBuffer, USART_RX_BUFF_SIZE);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
}

/**
 * @description: 初始化串口所需dma
 * @return {*}
 */
void bsp_dma_init(void)
{
}

/**
 * @description: uart 就收函数
 * @param {uint8_t} *rxData
 * @param {uint8_t} this_rx_buff_len
 * @return {*}
 */
bool bsp_uart_receive(uint8_t *rxData, uint8_t this_rx_buff_len)
{
    bsp_versa_receive(rxData,this_rx_buff_len,1);
//    return bsp_kmd_callback(rxData, this_rx_buff_len, KMD_MSG_FRAME_UART);
    return false;
}

/**
 * @description: uart 发送所需函数
 * @param {uint8_t} *pData
 * @param {uint16_t} len
 * @return {*}
 */
bool bsp_uart_transmit(uint8_t *pData, uint16_t len)
{
    HAL_UART_Transmit_DMA(&huart1, pData, len);
    return true;
}

/**
 * @description: uart 中断回调
 * @return {*}
 */
void bsp_uart_irq_callback(void)
{
    bool ret = false;
    static uint8_t this_rx_buff_len;
    if (USART1 == huart1.Instance) // 判断是否是串口1（！此处应写(huart->Instance == USART1)
    {
        if (RESET != __HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE)) // 判断是否是空闲中断
        {
            __HAL_UART_CLEAR_IDLEFLAG(&huart1); // 清楚空闲中断标志（否则会一直不断进入中断）
            HAL_UART_DMAStop(&huart1);
            this_rx_buff_len = USART_RX_BUFF_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
            ret = bsp_uart_receive(UartRxBuffer, this_rx_buff_len);
            hdma_usart1_rx.Instance->CNDTR = USART_RX_BUFF_SIZE;
            HAL_UART_Receive_DMA(&huart1, UartRxBuffer, USART_RX_BUFF_SIZE);
        }
    }
    return;
}
