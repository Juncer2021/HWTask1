#include "main.h"
#include "task1.h"

/* main.c 中定义的外设句柄 */
extern TIM_HandleTypeDef htim2;
extern IWDG_HandleTypeDef hiwdg;

/* 全局计时变量:定时器更新中断里每 1ms 自增 1。
   volatile 防止被编译器优化掉,也让 Ozone 能实时看到它 */
volatile uint32_t tick = 0;

/**
  * @brief  题目一:点亮板载 LED。
  *         F103 最小系统板 PC13 低电平点亮。
  */
void Task1_Init(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}

/**
  * @brief  题目二:启动 TIM2 的 1ms 更新中断。
  *         不调用此函数定时器不会开始计数,也就不会进回调。
  */
void Task2_Init(void)
{
    HAL_TIM_Base_Start_IT(&htim2);
}

/**
  * @brief  TIM2 更新中断回调,每 1ms 触发一次。
  *         题 2:tick 自增并喂狗;
  *         题 3:删掉/注释掉 HAL_IWDG_Refresh 这一行,重新编译下载。
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        tick++;
        /* HAL_IWDG_Refresh(&hiwdg);   题 3:注释掉喂狗,看门狗每 2s 复位,tick 归零 */
    }
}
