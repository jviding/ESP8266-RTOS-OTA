#pragma once

#include "freertos/FreeRTOS.h"


class HotSpot {
  public:
    // Getters
    static const char* get_ssid();
    static const char* get_password();
    static const char* get_ip_address();

    // Setters
    static esp_err_t set_ssid(const char* new_ssid);
    static esp_err_t set_password(const char* password);

    // Functions
    static esp_err_t start(bool use_password);
    static esp_err_t stop();

  private:
    // Variables
    static char* ssid;
    static char* password;
    static char* ip_address;

    // Setters
    static void set_8_digit_random_password();

    // Functions
    static esp_err_t start_networking();
    static esp_err_t init_wifi_ap_mode(bool use_password);
};
