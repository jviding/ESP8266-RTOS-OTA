#pragma once

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_check.h"
#include "nvs_flash.h"
#include "nvs.h"


class Storage {
  public:
    static esp_err_t init();
    static esp_err_t read_wifi_credentials(char** ssid, char** password);
    static esp_err_t save_wifi_credentials(char* ssid, char* password);

  private:
    static esp_err_t read_value(const char* key, char** value);
};
