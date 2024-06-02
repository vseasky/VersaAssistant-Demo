#ifndef _BSP_CRC8_H
#define _BSP_CRC8_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>


#define	CRC_START_8	0x00

/// <summary>
/// ���������ַ�����8λ��crc
/// </summary>
/// <param name="input_str"></param>
/// <param name="num_bytes">��������</param>
/// <returns></returns>
uint8_t crc_8( const uint8_t *input_str,uint16_t num_bytes);

/// <summary>
/// ����һ�������ֽں�ѭ������У��ֵ��ǰһ��ֵ
/// </summary>
/// <param name="crc"></param>
/// <param name="val"></param>
/// <returns>���㲢�������ݵ��µ�ʵ��crcֵ</returns>
uint8_t update_crc_8(uint8_t crc, uint8_t val );


#ifdef __cplusplus
}
#endif

#endif
