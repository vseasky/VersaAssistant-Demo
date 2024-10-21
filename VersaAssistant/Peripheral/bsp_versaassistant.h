/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-07-01 19:58:59
 * @FilePath     : \VersaAssistant-STM32\VersaAssistant\Peripheral\bsp_versaassistant.h
 * @Description  : 
 */
#ifndef  __BSP_VERSA_ASSISTANT_H__
#define  __BSP_VERSA_ASSISTANT_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>


void bsp_versa_init(void);

void bsp_versa_transmit(uint8_t *pData, uint16_t len);

void bsp_versa_receive(uint8_t *rxData, uint8_t this_rx_buff_len,uint8_t type);

extern uint8_t bsp_versa_type_transmit;
#endif