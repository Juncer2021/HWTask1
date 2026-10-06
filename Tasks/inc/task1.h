/*
 * @Author: Juncer Juncer2021@gmail.com
 * @Date: 2026-10-04 22:19:54
 * @LastEditors: Juncer Juncer2021@gmail.com
 * @LastEditTime: 2026-10-04 23:03:31
 * @FilePath: \HWTask1\Tasks\inc\task1.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __TASK1_H
#define __TASK1_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /* 全局计时变量:定时器更新中断里每 1ms 自增 1 */
    extern volatile uint32_t tick;

    /* 题目一:点亮板载 LED */
    void Task1_Init(void);

    /* 题目二:启动 1ms 定时器更新中断 */
    void Task2_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __TASK1_H */
