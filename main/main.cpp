#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "leds.h"

static const char* TAG = "Main";


void setup() {
  ESP_LOGI(TAG, "Started");
  Leds::init();
};

void loop() {
  ESP_LOGI(TAG, "Toggle Leds");
  Leds::toggle_led_1();
  Leds::toggle_led_2();
  Leds::toggle_led_3();
  vTaskDelay(1000 / portTICK_PERIOD_MS);
};

extern "C" void app_main() {
  setup();
  while (true) {
    loop();
  }
};
