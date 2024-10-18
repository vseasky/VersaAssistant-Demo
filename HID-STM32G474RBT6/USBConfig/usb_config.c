/*
 * Copyright (c) 2024, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "usbd_core.h"
#include "usbd_cdc.h"
#include "usbd_hid.h"
#include "bsp_versaassistant.h"

/*!< endpoint address */
#define CDC_IN_EP  0x81
#define CDC_OUT_EP 0x02
#define CDC_INT_EP 0x83
#define CDC_BUFFER_SIZE 128

/*!< hidraw in endpoint */
#define HIDRAW_IN_EP       0x84
#ifdef CONFIG_USB_HS
#define HIDRAW_IN_EP_SIZE  1024
#define HIDRAW_IN_INTERVAL 4
#else
#define HIDRAW_IN_EP_SIZE  64
#define HIDRAW_IN_INTERVAL 1
#endif
/*!< hidraw out endpoint */
#define HIDRAW_OUT_EP          0x05
#ifdef CONFIG_USB_HS
#define HIDRAW_OUT_EP_SIZE     1024
#define HIDRAW_OUT_EP_INTERVAL 4
#else
#define HIDRAW_OUT_EP_SIZE     64
#define HIDRAW_OUT_EP_INTERVAL 1
#endif


#define USBD_VID 0xd6e8
#define USBD_PID 0x3508
#define USBD_MAX_POWER     100
#define USBD_LANGID_STRING 1033

/*!< config descriptor size */
#define USB_CONFIG_SIZE (9 + CDC_ACM_DESCRIPTOR_LEN + 9 + 9 + 7 + 7 )
/*!< custom hid report descriptor size */
#define HID_CUSTOM_REPORT_DESC_SIZE 34

#ifdef CONFIG_USB_HS
#define CDC_MAX_MPS 512
#else
#define CDC_MAX_MPS 64
#endif


#define HID_STATE_IDLE 0
#define HID_STATE_BUSY 1

/*!< hid state ! Data can be sent only when state is idle  */
static volatile uint8_t custom_state;

/*!< global descriptor */
static const uint8_t cdc_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0xEF, 0x02, 0x01, USBD_VID, USBD_PID, 0x0100, 0x01),
    USB_CONFIG_DESCRIPTOR_INIT(USB_CONFIG_SIZE, 0x03, 0x01, USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),
    CDC_ACM_DESCRIPTOR_INIT(0x00, CDC_INT_EP, CDC_OUT_EP, CDC_IN_EP, CDC_MAX_MPS, 0x02),
    /************** Descriptor of Custom interface *****************/
    0x09,                          /* bLength: Interface Descriptor size */
    USB_DESCRIPTOR_TYPE_INTERFACE, /* bDescriptorType: Interface descriptor type */
    0x02,                          /* bInterfaceNumber: Number of Interface */
    0x00,                          /* bAlternateSetting: Alternate setting */
    0x02,                          /* bNumEndpoints */
    0x03,                          /* bInterfaceClass: HID */
    0x01,                          /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x00,                          /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,                             /* iInterface: Index of string descriptor */
    /******************** Descriptor of Custom HID ********************/
    0x09,                    /* bLength: HID Descriptor size */
    HID_DESCRIPTOR_TYPE_HID, /* bDescriptorType: HID */
    0x11,                    /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,                        /* bCountryCode: Hardware target country */
    0x01,                        /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,                        /* bDescriptorType */
    HID_CUSTOM_REPORT_DESC_SIZE, /* wItemLength: Total length of Report descriptor */
    0x00,
    /******************** Descriptor of Custom in endpoint ********************/
    0x07,                         /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT, /* bDescriptorType: */
    HIDRAW_IN_EP,                 /* bEndpointAddress: Endpoint Address (IN) */
    0x03,                         /* bmAttributes: Interrupt endpoint */
    WBVAL(HIDRAW_IN_EP_SIZE),     /* wMaxPacketSize: 4 Byte max */
    HIDRAW_IN_INTERVAL,           /* bInterval: Polling Interval */
    /******************** Descriptor of Custom out endpoint ********************/
    0x07,                         /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT, /* bDescriptorType: */
    HIDRAW_OUT_EP,                /* bEndpointAddress: Endpoint Address (IN) */
    0x03,                         /* bmAttributes: Interrupt endpoint */
    WBVAL(HIDRAW_OUT_EP_SIZE),    /* wMaxPacketSize: 4 Byte max */
    HIDRAW_OUT_EP_INTERVAL,       /* bInterval: Polling Interval */
    ///////////////////////////////////////
    /// string0 descriptor
    ///////////////////////////////////////
    USB_LANGID_INIT(USBD_LANGID_STRING),
    ///////////////////////////////////////
    /// string1 descriptor
    ///////////////////////////////////////
    0x14,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'C', 0x00,                  /* wcChar0 */
    'h', 0x00,                  /* wcChar1 */
    'e', 0x00,                  /* wcChar2 */
    'r', 0x00,                  /* wcChar3 */
    'r', 0x00,                  /* wcChar4 */
    'y', 0x00,                  /* wcChar5 */
    'U', 0x00,                  /* wcChar6 */
    'S', 0x00,                  /* wcChar7 */
    'B', 0x00,                  /* wcChar8 */
    ///////////////////////////////////////
    /// string2 descriptor
    ///////////////////////////////////////
    0x1e,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'V', 0x00,                  /* wcChar0 */
    'e', 0x00,                  /* wcChar1 */
    'r', 0x00,                  /* wcChar2 */
    's', 0x00,                  /* wcChar3 */
    'a', 0x00,                  /* wcChar4 */
    'A', 0x00,                  /* wcChar5 */
    's', 0x00,                  /* wcChar6 */
    's', 0x00,                  /* wcChar7 */
    'i', 0x00,                  /* wcChar8 */
    's', 0x00,                  /* wcChar9 */
    't', 0x00,                  /* wcChar10 */
    'a', 0x00,                  /* wcChar11 */
    'n', 0x00,                  /* wcChar12 */
    't', 0x00,                  /* wcChar13 */
    ///////////////////////////////////////
    /// string3 descriptor
    ///////////////////////////////////////
    0x16,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    '2', 0x00,                  /* wcChar0 */
    '0', 0x00,                  /* wcChar1 */
    '2', 0x00,                  /* wcChar2 */
    '2', 0x00,                  /* wcChar3 */
    '1', 0x00,                  /* wcChar4 */
    '2', 0x00,                  /* wcChar5 */
    '3', 0x00,                  /* wcChar6 */
    '4', 0x00,                  /* wcChar7 */
    '5', 0x00,                  /* wcChar8 */
    '6', 0x00,                  /* wcChar9 */
#ifdef CONFIG_USB_HS
    ///////////////////////////////////////
    /// device qualifier descriptor
    ///////////////////////////////////////
    0x0a,
    USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x02,
    0x02,
    0x01,
    0x40,
    0x01,
    0x00,
#endif
    0x00
};

/*!< custom hid report descriptor */
static const uint8_t hid_custom_report_desc[HID_CUSTOM_REPORT_DESC_SIZE] = {
#ifdef CONFIG_USB_HS
    0x06, 0x00, 0xff,              // USAGE_PAGE (Vendor Defined Page 1)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x96, 0x00, 0x01,              //   REPORT_COUNT (256)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x96, 0x00, 0x01,              //   REPORT_COUNT (256)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x91, 0x00,                    //   OUTPUT (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
#else
    0x06, 0x00, 0xff,              // USAGE_PAGE (Vendor Defined Page 1)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x95, 0x40,                    //   REPORT_COUNT (64)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x95, 0x40,                    //   REPORT_COUNT (64)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x91, 0x00,                    //   OUTPUT (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
#endif
};




USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t cdc_read_buffer[CDC_BUFFER_SIZE]; /* 2048 is only for test speed , please use CDC_MAX_MPS for common*/
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t cdc_send_buffer[CDC_BUFFER_SIZE];
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t hid_read_buffer[HIDRAW_OUT_EP_SIZE];
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t hid_send_buffer[HIDRAW_IN_EP_SIZE];

volatile bool ep_tx_busy_flag = false;


void usbd_cdc_acm_bulk_out(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
        bsp_versa_receive(cdc_read_buffer,nbytes,3);
//    USB_LOG_RAW("actual out len:%d\r\n", nbytes);
    usbd_ep_start_read(busid, CDC_OUT_EP, cdc_read_buffer, CDC_BUFFER_SIZE);
}

void usbd_cdc_acm_bulk_in(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
//    USB_LOG_RAW("actual in len:%d\r\n", nbytes);

    if ((nbytes % usbd_get_ep_mps(busid, ep)) == 0 && nbytes) {
        /* send zlp */
        usbd_ep_start_write(busid, CDC_IN_EP, NULL, 0);
    } else {
        ep_tx_busy_flag = false;
    }
}


void cdc_acm_data_send(uint8_t busid,uint8_t *txBuffer,uint16_t ulen)
{
    ep_tx_busy_flag = true;
    usbd_ep_start_write(busid, CDC_IN_EP, txBuffer, ulen);
//    while (ep_tx_busy_flag) {}
}

/*!< endpoint call back */
struct usbd_endpoint cdc_out_ep = {
    .ep_addr = CDC_OUT_EP,
    .ep_cb = usbd_cdc_acm_bulk_out
};

struct usbd_endpoint cdc_in_ep = {
    .ep_addr = CDC_IN_EP,
    .ep_cb = usbd_cdc_acm_bulk_in
};


static void usbd_hid_custom_in_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    (void)busid;
    (void)ep;
//    USB_LOG_RAW("actual in len:%d\r\n", nbytes);
    custom_state = HID_STATE_IDLE;
}

static void usbd_hid_custom_out_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    
    bsp_versa_receive(hid_read_buffer,nbytes,2);
//    USB_LOG_RAW("actual out len:%d\r\n", nbytes);
    usbd_ep_start_read(busid, ep, hid_read_buffer, HIDRAW_IN_EP_SIZE);
    
//    hid_read_buffer[0] = 0x02; /* IN: report id */
//    usbd_ep_start_write(busid, HIDRAW_IN_EP, hid_read_buffer, nbytes);
}

void hid_data_send(uint8_t busid,uint8_t *txBuffer,uint16_t ulen)
{
    ep_tx_busy_flag = true;
    usbd_ep_start_write(busid, HIDRAW_IN_EP, txBuffer, ulen);
//    while (ep_tx_busy_flag) {}
}

static struct usbd_endpoint custom_in_ep = {
    .ep_cb = usbd_hid_custom_in_callback,
    .ep_addr = HIDRAW_IN_EP
};

static struct usbd_endpoint custom_out_ep = {
    .ep_cb = usbd_hid_custom_out_callback,
    .ep_addr = HIDRAW_OUT_EP
};


static void usbd_event_handler(uint8_t busid, uint8_t event)
{
    switch (event) {
        case USBD_EVENT_RESET:
            break;
        case USBD_EVENT_CONNECTED:
            break;
        case USBD_EVENT_DISCONNECTED:
            break;
        case USBD_EVENT_RESUME:
            break;
        case USBD_EVENT_SUSPEND:
            break;
        case USBD_EVENT_CONFIGURED:
            ep_tx_busy_flag = false;
            /* setup first out ep read transfer */
            usbd_ep_start_read(busid, CDC_OUT_EP, cdc_read_buffer, 128);
            usbd_ep_start_read(busid, HIDRAW_OUT_EP, hid_read_buffer, HIDRAW_OUT_EP_SIZE);
            break;
        case USBD_EVENT_SET_REMOTE_WAKEUP:
            break;
        case USBD_EVENT_CLR_REMOTE_WAKEUP:
            break;

        default:
            break;
    }
}

struct usbd_interface intf0;
struct usbd_interface intf1;
struct usbd_interface intf2;

void usb_config_init(uint8_t busid, uint32_t reg_base)
{
    usbd_desc_register(busid, cdc_descriptor);
    usbd_add_interface(busid, usbd_cdc_acm_init_intf(busid, &intf0));
    usbd_add_interface(busid, usbd_cdc_acm_init_intf(busid, &intf1));
    usbd_add_endpoint(busid, &cdc_out_ep);
    usbd_add_endpoint(busid, &cdc_in_ep);

    usbd_add_interface(busid, usbd_hid_init_intf(busid, &intf2, hid_custom_report_desc, HID_CUSTOM_REPORT_DESC_SIZE));
    usbd_add_endpoint(busid, &custom_in_ep);
    usbd_add_endpoint(busid, &custom_out_ep);
    usbd_initialize(busid, reg_base, usbd_event_handler);
}



