#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define TAG "main"

#define LED_1 GPIO_NUM_12
#define LED_2 GPIO_NUM_14

extern "C" void app_main() {
  while (true) {
    ESP_LOGI("main", "Hello!");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
};
