#ifndef _BSP_CRC16_H
#define _BSP_CRC16_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>


#define	CRC_START_16	 0xFFFF
#define	CRC_START_MODBUS 0xFFFF
#define	CRC_POLY_16		 0xA001


/// <summary>
/// ����crc_16()һ�μ���һ���ֽڵ�16λCRC16
/// </summary>
/// <param name="input_str">�ַ���</param>
/// <param name="num_bytes">�ֽ���</param>
/// <returns></returns>
uint16_t crc_16(const uint8_t *input_str,uint16_t num_bytes );

/// <summary>
/// һ�μ���16λmodbusѭ������У��
/// </summary>
/// <param name="input_str">�ַ���</param>
/// <param name="num_bytes">�ֽ���</param>
/// <returns></returns>
uint16_t crc_modbus(const uint8_t *input_str, uint16_t num_bytes );

/// <summary>
/// ǰһ��ѭ������У��ֵ����һ��Ҫ���������ֽڡ�
/// </summary>
/// <param name="crc"></param>
/// <param name="c"></param>
/// <returns></returns>
uint16_t update_crc_16(uint16_t crc,uint8_t c );

/// <summary>
/// Ϊ�˻��������ܣ�ʹ��CRC16���̲��Ҵ���ֵ�ı�
/// </summary>
/// <param name=""></param>

void init_crc16_tab( void );


#ifdef __cplusplus
}
#endif
#endif
