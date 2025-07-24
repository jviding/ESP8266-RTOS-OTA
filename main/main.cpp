#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "leds.h"
#include "storage.h"
#include "hotspot.h"

#include "tests.h"


static const char* TAG = "Main";


// Bundle up all update related to core.h

void setup() {
  ESP_LOGI(TAG, "Started");
  Leds::init();

  vTaskDelay(5000 / portTICK_PERIOD_MS);
  ESP_LOGI(TAG, "asd");

  //HotSpot::init();
  //Storage::init();
};

void loop() {
  ESP_LOGI(TAG, "Loop");

  Leds::toggle_led_1();
  Leds::toggle_led_2();
  Leds::toggle_led_3();

  Tests::test();

  while (true) {
    ;
  }


  ESP_LOGE(TAG, "Try no Passwd.");
  HotSpot::set_ssid("HelloSpot");
  HotSpot::start(false);
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  HotSpot::stop();
  vTaskDelay(2000 / portTICK_PERIOD_MS);

  ESP_LOGE(TAG, "Use random pwd.");
  HotSpot::start(true);
  ESP_LOGE(TAG, "PWD: %s", (char*)HotSpot::get_password);

  while (true) {
    ;
  }


  /*char* pwd = nullptr;
  char* ip = nullptr;
  HotSpot::start(&pwd, &ip);
  ESP_LOGI(TAG, "PWD: %s", pwd);
  delete[] pwd;
  delete[] ip;*/

  vTaskDelay(1000 / portTICK_PERIOD_MS);
};

extern "C" void app_main() {
  setup();
  while (true) {
    loop();
  }
};
