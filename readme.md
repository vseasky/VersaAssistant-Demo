
## 说明

关于 `VersaAssistant` 上位机的使用说明

[VersaAssistant文档](https://docs.liuwei.vin/projects/VersaAssistant/)

```



## 本仓库案例

内部集成了 基于协议的串口通信、CDC通信、和HID通信的案例。

```c
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
```

方式的选择取决于连续短按 `KEY` 按键的次数

- 连续短按1次为串口通信
- 连续短按2次为CDC通信
- 连续短按3次为HID通信

```c
bsp_versa_type_transmit = key_t[0].key_info.click_state_num;
```

![image.png](https://www.liuwei.pub/pic/2024/10/21/6715e7e8d0ef8.png)

HID模式

![image.png](https://www.liuwei.pub/pic/2024/10/21/6715e8b4a9e13.png)

![image.png](https://www.liuwei.pub/pic/2024/10/21/6715e89cb6280.png)
