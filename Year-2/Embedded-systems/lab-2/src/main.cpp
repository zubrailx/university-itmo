#include "hal.h"

#define LAB_LEDS_COUNT 8
#define LAB_SW_COUNT 4

void animation_iterate_and_change(unsigned int *leds, GPIO_PinState *leds_state);
int get_sw_value(unsigned int *sw);
void TIM6_IRQ_Handler();


// add static
unsigned int delay = 500;
unsigned int delay_h = 250;

int delay_prev = delay;
int delay_cur;

unsigned int led_num[] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_8, 
        GPIO_PIN_9, GPIO_PIN_11, GPIO_PIN_12};

int to_lower = 1;

GPIO_PinState led_state[] = {GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, 
        GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET};

unsigned int sw_num[] = {GPIO_PIN_4, GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_12};

int umain(){
  // switches GPIOE
  //GPIO_PinState sw_condition[] = {GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET};
  registerTIM6_IRQHandler(TIM6_IRQ_Handler);
  __enable_irq();
  WRITE_REG(TIM6_ARR, 500);
  WRITE_REG(TIM6_DIER, TIM_DIER_UIE);
  WRITE_REG(TIM6_PSC, 0);

  WRITE_REG(TIM6_CR1, TIM_CR1_CEN);

  while (1) {
    delay_cur = delay + get_sw_value(sw_num) * delay_h;
    if (delay_cur != delay_prev) {
      WRITE_REG(TIM6_CNT, 0);
      WRITE_REG(TIM6_ARR, delay_cur);
      delay_prev = delay_cur;
    }
    HAL_Delay(1);
  }
  return 0;
}

// count <= sizeof(unsigned int) * 8 
void animation_iterate_and_change(unsigned int *leds, GPIO_PinState *leds_state) {
  unsigned int flags = 0;
  // set leds on this iteration
  for (int i = 0; i < LAB_LEDS_COUNT; ++i) {
    HAL_GPIO_WritePin(GPIOD, leds[LAB_LEDS_COUNT - 1 - i], leds_state[LAB_LEDS_COUNT - 1 - i]);
    flags = (flags << 1) | (leds_state[LAB_LEDS_COUNT - 1 - i] == GPIO_PIN_SET);
  }
  if (flags == 0b11111111 or flags == 0b00000001) {
    to_lower = !to_lower;
  }
  // next iteration 
  flags = to_lower ? flags >> 1 : (flags << 1 | 0b1);

  // change state array
  for (int i = 0; i < LAB_LEDS_COUNT; ++i) {
    if (flags % 2) {
      leds_state[i] = GPIO_PIN_SET;
    } else {
      leds_state[i] = GPIO_PIN_RESET;
    }
    flags /= 2;
  }
}

void TIM6_IRQ_Handler(){
  animation_iterate_and_change(led_num, led_state);
}

// sw <= 32
int get_sw_value(unsigned int *sw) {
  unsigned int value = 0;
  for (int i = 0; i < LAB_SW_COUNT; ++i) {
    value = (value << 1) | (HAL_GPIO_ReadPin(GPIOE, sw[i]) == GPIO_PIN_SET);
  }
  return value;
}


#undef LAB_LEDS_COUNT
#undef LAB_SW_COUNT


