#pragma once

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/gpio.h"

#define LED_1 GPIO_NUM_12
#define LED_2 GPIO_NUM_14
#define LED_3 GPIO_NUM_16


class Leds {
  public:
    static esp_err_t init();
    static esp_err_t toggle_led_1();
    static esp_err_t toggle_led_2();
    static esp_err_t toggle_led_3();

  private:
    static esp_err_t toggle_led(gpio_num_t gpio_num);
};
