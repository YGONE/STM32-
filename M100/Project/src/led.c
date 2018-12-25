
#include "stm32f10x.h"
#include "delay.h"


/*****************************************************************************
 * 描  述 : LED状态翻转
 * 入  参 : 无
 * 返回值 : 无
 *****************************************************************************/
void LED_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->ODR ^= GPIO_Pin;
}
/*********************************END FILE************************************/

