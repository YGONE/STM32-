
#include "stm32f10x.h"
#include "delay.h"


/*****************************************************************************
 * ��  �� : LED״̬��ת
 * ��  �� : ��
 * ����ֵ : ��
 *****************************************************************************/
void LED_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->ODR ^= GPIO_Pin;
}
/*********************************END FILE************************************/

