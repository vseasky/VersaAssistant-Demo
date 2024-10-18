/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-19 21:33:33
 * @FilePath     : \MDK-ARMe:\Projects\KmdFoc-G4\software\KmdFocG4\KmdFoc\Peripheral\bsp_systick.c
 * @Description  : 
 */

#include "bsp_systick.h"
#include "main.h"

#define SYSTICK_US_TICK (SystemCoreClock / 1000000U)
#define SYSTICK_MS_TICK (SystemCoreClock / 1000U)

// volatile uint64_t SysTickCnt = 0;

/**
 * @description: 初始化Systick
 * @return {*}
 */
void bsp_systick_init(void)
{
}

/**
 * @description: 获取系统时间us
 * @return {*}
 */
uint64_t bsp_systick_get_tick_us(void)
{
    uint64_t tSysTick, tSysTickUs;
    tSysTick = SysTick->LOAD + 1 - SysTick->VAL;
    tSysTickUs = (uint64_t)(tSysTick / SYSTICK_US_TICK + HAL_GetTick() * 1000);
    return tSysTickUs;
}

/**
 * @description: 获取系统时间差us
 * @param {uint64_t} tick_us
 * @return {*}
 */
uint64_t bsp_systick_get_us_since(uint64_t tick_us)
{
    return (bsp_systick_get_tick_us() - tick_us);
}

/**
 * @description: 获取系统时间ms
 * @return {*}
 */
uint64_t bsp_systick_get_tick_ms(void)
{
    return HAL_GetTick();
}

/**
 * @description: 计算系统时间差ms
 * @param {uint32_t} tick
 * @return {*}
 */
uint64_t bsp_systick_get_ms_since(uint64_t tick_ms)
{
    return (uint64_t)(HAL_GetTick() - tick_ms);
}

/**
 * @description: systick us延时函数
 * @param {uint32_t} uS
 * @return {*}
 */
void bsp_systick_delay_us(uint64_t us)
{
    uint64_t elapsed = 0;
    uint64_t last_count = SysTick->VAL;
    for (;;)
    {
        uint64_t current_count = SysTick->VAL;
        uint64_t elapsed_uS;
        // 测量自我们上次检查以来经过的时间
        if (last_count > current_count)
        {
            elapsed += last_count - current_count;
        }
        else if (last_count < current_count)
        {
            elapsed += last_count + (SYSTICK_MS_TICK - current_count);
        }
        last_count = current_count;
        // 转换为微秒
        elapsed_uS = elapsed / SYSTICK_US_TICK;
        if (elapsed_uS >= us)
        {
            break;
        }
        // 通过经过的时间减少延迟
        us -= elapsed_uS;
        // 为下一次迭代保留小数微秒
        elapsed %= SYSTICK_US_TICK;
    }
}

/**
 * @description: ms延时函数
 * @param {uint32_t} mS
 * @return {*}
 */
void bsp_systick_delay_ms(uint64_t ms)
{
    while (ms--)
    {
        bsp_systick_delay_us(1000);
    }
}

