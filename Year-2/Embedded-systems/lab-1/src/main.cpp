#include "hal.h"

#define LAB_LEDS_COUNT 8
#define LAB_SW_COUNT 4

#define LAB_CONDITION_SW  0b01
#define LAB_CONDITION_BTN 0b10

int check_sw_condition(unsigned int *sw, GPIO_PinState *cond);
void animation_iterate_and_change(unsigned int *leds, GPIO_PinState *leds_state);
void set_led_sw(unsigned int *leds, unsigned int *sw);

int umain(){
  unsigned int leds_num[] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_8, 
          GPIO_PIN_9, GPIO_PIN_11, GPIO_PIN_12};
  GPIO_PinState leds_state[] = {GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET, 
          GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET};
  // switches GPIOE
  unsigned int sw_num[] = {GPIO_PIN_4, GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_12};
  GPIO_PinState sw_condition[] = {GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET};

  int delay = 500;
  unsigned int flags = 0b10;

  while (1) {
    // bitwise flag set
    if (check_sw_condition(sw_num, sw_condition)){ 
      flags |=  0b01;
      // restoring previous session
      for (int i = 0; i < LAB_LEDS_COUNT; ++i) {
        HAL_GPIO_WritePin(GPIOD, leds_num[i], leds_state[i]);
      }
    }
    else { flags &= ~0b01; }
    
    if (flags & 0b01) {
      if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == GPIO_PIN_RESET) flags ^=  0b10;

      if (flags & 0b10) {
        // set color green
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
        animation_iterate_and_change(leds_num, leds_state);
      } else {
        // set color red
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
      }
    } else {
      // set color yellow
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
      set_led_sw(leds_num, sw_num);
    }
    HAL_Delay(delay);
  }
  return 0;
}

// count <= sizeof(unsigned int) * 8 
void animation_iterate_and_change(unsigned int *leds, GPIO_PinState *leds_state) {
  unsigned int flags = 0;
  // set leds on this iteration
  for (int i = 0; i < LAB_LEDS_COUNT; ++i) {
    HAL_GPIO_WritePin(GPIOD, leds[i], leds_state[i]);
    flags = (flags << 1) | (leds_state[i] == GPIO_PIN_SET);
  }
  // next iteration 
  flags = (flags >> 1) | ((flags % 2) << 7);
  // change state array
  for (int i = 0; i < LAB_LEDS_COUNT; ++i) {
    if (flags % 2) {
      leds_state[LAB_LEDS_COUNT - 1 - i] = GPIO_PIN_SET;
    } else {
      leds_state[LAB_LEDS_COUNT - 1 - i] = GPIO_PIN_RESET;
    }
    flags /= 2;
  }
}

int check_sw_condition(unsigned int *sw, GPIO_PinState *cond) {
  for (int i = 0; i < LAB_SW_COUNT; ++i) 
    if (HAL_GPIO_ReadPin(GPIOE, sw[i]) != cond[i]) return 0; 
  return 1;
}


void set_led_sw(unsigned int *leds, unsigned int *sw) {
  int count = LAB_LEDS_COUNT > LAB_SW_COUNT ? LAB_SW_COUNT : LAB_LEDS_COUNT;
  for (int i = 0; i < count; ++i) {
    GPIO_PinState state = HAL_GPIO_ReadPin(GPIOE, sw[i]);
    HAL_GPIO_WritePin(GPIOD, leds[i], state);
  }
  for (int i = count; i < LAB_LEDS_COUNT; ++i) {
    HAL_GPIO_WritePin(GPIOD, leds[i], GPIO_PIN_RESET);
  }
}

#undef LAB_CONDITION_SW
#undef LAB_CONDITION_BTN

#undef LAB_LEDS_COUNT
#undef LAB_SW_COUNT


