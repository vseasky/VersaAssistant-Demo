/*
 * Copyright (c) 2024, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "usbd_core.h"
#include "usbd_cdc.h"


// USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t cdc_read_buffer[2048]; /* 2048 is only for test speed , please use CDC_MAX_MPS for common*/
// USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t cdc_send_buffer[2048];

// void usbd_cdc_acm_bulk_out(uint8_t busid, uint8_t ep, uint32_t nbytes)
// {
//     USB_LOG_RAW("actual out len:%d\r\n", nbytes);
//     // for (int i = 0; i < 100; i++) {
//     //     printf("%02x ", cdc_read_buffer[i]);
//     // }
//     // printf("\r\n");
//     /* setup next out ep read transfer */
//     usbd_ep_start_read(busid, CDC_OUT_EP, cdc_read_buffer, 2048);
// }

// void usbd_cdc_acm_bulk_in(uint8_t busid, uint8_t ep, uint32_t nbytes)
// {
//     USB_LOG_RAW("actual in len:%d\r\n", nbytes);

//     if ((nbytes % usbd_get_ep_mps(busid, ep)) == 0 && nbytes) {
//         /* send zlp */
//         usbd_ep_start_write(busid, CDC_IN_EP, NULL, 0);
//     } else {
//         ep_tx_busy_flag = false;
//     }
// }

// /*!< endpoint call back */
// struct usbd_endpoint cdc_out_ep = {
//     .ep_addr = CDC_OUT_EP,
//     .ep_cb = usbd_cdc_acm_bulk_out
// };

// struct usbd_endpoint cdc_in_ep = {
//     .ep_addr = CDC_IN_EP,
//     .ep_cb = usbd_cdc_acm_bulk_in
// };

// static struct usbd_interface intf0;
// static struct usbd_interface intf1;

// void cdc_acm_init(uint8_t busid, uint32_t reg_base)
// {
//     const uint8_t data[10] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30 };

//     memcpy(&cdc_send_buffer[0], data, 10);
//     memset(&cdc_send_buffer[10], 'a', 2038);

//     usbd_desc_register(busid, cdc_descriptor);
//     usbd_add_interface(busid, usbd_cdc_acm_init_intf(busid, &intf0));
//     usbd_add_interface(busid, usbd_cdc_acm_init_intf(busid, &intf1));
//     usbd_add_endpoint(busid, &cdc_out_ep);
//     usbd_add_endpoint(busid, &cdc_in_ep);
//     usbd_initialize(busid, reg_base, usbd_event_handler);
// }

// volatile uint8_t dtr_enable = 0;

// void usbd_cdc_acm_set_dtr(uint8_t busid, uint8_t intf, bool dtr)
// {
//     if (dtr) {
//         dtr_enable = 1;
//     } else {
//         dtr_enable = 0;
//     }
// }

// void cdc_acm_data_send_with_dtr_test(uint8_t busid)
// {
//     if (dtr_enable) {
//         ep_tx_busy_flag = true;
//         usbd_ep_start_write(busid, CDC_IN_EP, cdc_send_buffer, 2048);
//         while (ep_tx_busy_flag) {
//         }
//     }
// }