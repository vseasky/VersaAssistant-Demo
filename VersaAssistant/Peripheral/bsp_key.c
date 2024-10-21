/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-07-01 20:14:21
 * @FilePath     : \VersaAssistant-STM32\VersaAssistant\Peripheral\bsp_key.c
 * @Description  : 
 */
#include "bsp_key.h"
#include "main.h"

extern uint8_t bsp_versa_type;

mx_key_task_info key_t[MAX_KEY_NUM];
/** 
 * @Description  : 初始化按键所需Gpio
 * @return        (*)
 */
void bsp_key_init(void)
{
//	rcu_periph_clock_enable(RCU_GPIOB);
//	gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ,GPIO_PIN_1);
//	key_t[0].GPIO_Group = GPIOB;
//	key_t[0].GPIO_Pin = GPIO_PIN_1;
}

/** 
 * @Description  : 获取按键电平状态，未消抖
 * @param         (mx_key_task_info) key_scan_t
 * @return        (*)
 */
uint8_t key_task_scan(mx_key_task_info key_scan_t)
{
    if (HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin) == KEY_TASK_PRESS_LEVEL)
    {
        return KEY_TASK_PRESS_S;
    }
    return KEY_TASK_UPSPRING_S;
}

/** 
 * @Description  : 按键扫描算法，应定时2ms周期调用
 * @return        (*)
 */
void key_enc_scanf(void)
{
	uint8_t key_count;
	uint8_t key[MAX_KEY_NUM];
    for (key_count = 0; key_count < MAX_KEY_NUM; key_count++)
    {
        // 读取按键电平
        key[key_count] = key_task_scan(key_t[key_count]);
        if (key[key_count] == KEY_TASK_PRESS_S)
        {
            // 按键按下计数
            if (key_t[key_count].key_info.time_count < KEY_PRESS_MAX_COUNT)
            {
                key_t[key_count].key_info.time_count++;
            }
        }
        else
        {
            // 按键弹起计数
            if (key_t[key_count].key_info.res_count < KEY_RES_MAX_COUNT)
            {
                key_t[key_count].key_info.res_count++;
            }
        }
        // 到达长按时间
        if (key_t[key_count].key_info.time_count >= KEY_TASK_TIME_L_COUNT)
        {
            if (key_t[key_count].key_info.states == KEY_TASK_PRESS)
            {
                key_t[key_count].key_info.states = KEY_TASK_PRESS_L_TIME;
                key_t[key_count].key_info.click_num = 0;
                key_t[key_count].key_info.click_state_num = 0;
                // 按键复位计数清零
                key_t[key_count].key_info.res_count = 0;
            }
        }
        // 短按时间
        else if (key_t[key_count].key_info.time_count >= KEY_TASK_TIME_P_COUNT)
        {
            // 首次更新按键事件
            if (key_t[key_count].key_info.states == KEY_TASK_UPSPRING)
            {
                key_t[key_count].key_info.states = KEY_TASK_PRESS;
                // 记录按键按下次数
                if (key_t[key_count].key_info.click_num < KEY_MAX_NUM)
                {
                    key_t[key_count].key_info.click_num++;
                }
                // 按键复位计数清零
                key_t[key_count].key_info.res_count = 0;
            }
        }
        // 连续按超时
        if (key_t[key_count].key_info.res_count >= KEY_RES_NUM_COUNT)
        {
            if (key_t[key_count].key_info.click_num != 0)
            {
                key_t[key_count].key_info.click_state_num =
                    key_t[key_count].key_info.click_num;
                key_t[key_count].key_info.click_num = 0;
            }
        }
        // 按键复位弹起
        else if (key_t[key_count].key_info.res_count >= KEY_RES_COUNT)
        {
            if (key_t[key_count].key_info.states != KEY_TASK_UPSPRING)
            {
                key_t[key_count].key_info.states = KEY_TASK_UPSPRING; // 按键弹起复位
                // 按键按下计数清零
                key_t[key_count].key_info.time_count = 0; // 按键计数清零
            }
        }
    }
}