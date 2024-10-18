/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-19 21:33:33
 * @FilePath     : \VersaAssistant-STM32\VersaAssistant\Peripheral\bsp_uart.h
 * @Description  : 
 */

#ifndef  __BSP_UART_H__
#define  __BSP_UART_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/**
 * @description: 初始化串口
 * @return {*}
 */
void bsp_uart_init(void);

/**
 * @description: ???????dma
 * @return {*}
 */
void bsp_dma_init(void);

/**
 * @description: uart ??????
 * @param {uint8_t} *pData
 * @param {uint16_t} len
 * @return {*}
 */
bool bsp_uart_transmit(uint8_t *pData,uint16_t len);

/**
 * @description: uart ???? 
 * @return {*}
 */
void bsp_uart_irq_callback(void);

#endif
