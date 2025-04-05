#include "hotspot.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include <stdio.h>
//#include "sdkconfig.h"
//#include <string.h>
//#include <iostream>
#include <cstring>
//#include <cstdlib>

//#define WIFI_SSID   CONFIG_HOTSPOT_AP_WIFI_SSID

static const char* TAG = "HotSpot";

char* HotSpot::ssid = nullptr;
char* HotSpot::password = nullptr;
char* HotSpot::ip_address = nullptr;


const char* HotSpot::get_ssid() {
  return ssid;
};

const char* HotSpot::get_password() {
  return password;
};

const char* HotSpot::get_ip_address() {
  return ip_address;
};


esp_err_t HotSpot::set_ssid(const char* new_ssid) {
  ESP_RETURN_ON_ERROR((new_ssid == nullptr) ? ESP_ERR_INVALID_ARG : ESP_OK, TAG,
    "SSID was nullptr.");
  ESP_RETURN_ON_ERROR((strlen(new_ssid) == 0) ? ESP_ERR_INVALID_ARG : ESP_OK, TAG,
    "SSID was empty.");
  ESP_RETURN_ON_ERROR((strlen(new_ssid) > 31) ? ESP_ERR_INVALID_ARG : ESP_OK, TAG,
    "SSID exceeds 31 chars.");

  delete[] ssid;
  size_t len = strlen(new_ssid) + 1; // +1 for null terminator
  ssid = new char[len];
  strcpy(ssid, new_ssid);
  return ESP_OK;
};

esp_err_t HotSpot::set_password(const char* new_password) {
  ESP_RETURN_ON_ERROR((new_password == nullptr) ? ESP_ERR_INVALID_ARG : ESP_OK, TAG,
    "Password was nullptr");
  ESP_RETURN_ON_ERROR((strlen(new_password) <= 8) ? ESP_ERR_INVALID_ARG : ESP_OK, TAG,
    "Password was less than 8 chars.");
  ESP_RETURN_ON_ERROR((strlen(new_password) > 63) ? ESP_ERR_INVALID_ARG : ESP_OK, TAG,
    "Password exceeds 63 chars.");

  delete[] password;
  size_t len = strlen(new_password) + 1; // +1 for null terminator
  password = new char[len];
  strcpy(password, new_password);
  return ESP_OK;
};

void HotSpot::set_8_digit_random_password() {
  delete[] password;
  password = new char[9];
  password[8] = '\0';
  for (int i = 0; i < 8; i++) {
    password[i] = '0' + (esp_random() % 10); // ASCII conversion
  }
};


void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
  if (event_base == WIFI_EVENT) {
    switch (event_id) {
      case WIFI_EVENT_AP_START:
        ESP_LOGI(TAG, "WiFi AP started.");
        break;
      case WIFI_EVENT_AP_STOP:
        ESP_LOGI(TAG, "WiFi AP stopped.");
        break;
      case WIFI_EVENT_AP_STACONNECTED:
        ESP_LOGI(TAG, "Client connected.");
        break;
      case WIFI_EVENT_AP_STADISCONNECTED:
        ESP_LOGI(TAG, "Client disconnected.");
        break;
      default:
        break;
    }
  }
};

esp_err_t HotSpot::start_networking() {
  // Enable TCP/IP stack
  tcpip_adapter_init();
  // Stop DHCP server if running
  ESP_RETURN_ON_ERROR(tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP), TAG, 
    "Failed stopping DHCP server.");
  // Configure iface
  tcpip_adapter_ip_info_t ip_info;
  IP4_ADDR(&ip_info.ip, 10, 0, 0, 1);
  IP4_ADDR(&ip_info.gw, 10, 0, 0, 1);
  IP4_ADDR(&ip_info.netmask, 255, 255, 255, 0);
  ESP_RETURN_ON_ERROR(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &ip_info), TAG,
    "Failed configuring network interface.");
  // Start DHCP server
  ESP_RETURN_ON_ERROR(tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP), TAG,
    "Failed starting DHCP server.");
  return ESP_OK;
};

esp_err_t HotSpot::init_wifi_ap_mode(bool use_password) {
  // Initialize
  wifi_init_config_t init_conf = WIFI_INIT_CONFIG_DEFAULT();
  ESP_RETURN_ON_ERROR(esp_wifi_init(&init_conf), TAG,
    "Failed initializing WiFi.");
  // Set mode
  ESP_RETURN_ON_ERROR(esp_wifi_set_mode(WIFI_MODE_AP), TAG,
    "Failed setting WiFi mode to AP.");
  // Configure
  wifi_config_t wifi_conf;
  strcpy(reinterpret_cast<char*>(wifi_conf.ap.ssid), ssid);
  wifi_conf.ap.ssid_len = strlen(ssid);

  if (use_password) {
    ESP_LOGI(TAG, "Authmode: Use password.");
    wifi_conf.ap.authmode = WIFI_AUTH_WPA2_PSK;
    strcpy(reinterpret_cast<char*>(wifi_conf.ap.password), password);
  } else {
    ESP_LOGI(TAG, "Authmode: No password.");
    wifi_conf.ap.authmode = WIFI_AUTH_OPEN;
  }

  ESP_RETURN_ON_ERROR(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_conf), TAG,
    "Failed setting WiFi AP configuration.");
  return ESP_OK;
};

esp_err_t HotSpot::start(bool use_password) {
  
  ESP_RETURN_ON_ERROR((ssid == nullptr) ? ESP_ERR_INVALID_ARG : ESP_OK, TAG, "SSID not set.");

  if (use_password == true && password == nullptr) {
    ESP_LOGI(TAG, "Generating random password.");
    set_8_digit_random_password();
  }

  ESP_RETURN_ON_ERROR(start_networking(), TAG, 
    "Failed starting network stack.");
  ESP_RETURN_ON_ERROR(esp_event_loop_create_default(), TAG,
    "Failed creating event loop.");
  ESP_RETURN_ON_ERROR(init_wifi_ap_mode(use_password), TAG,
    "Failed initializing WiFi AP mode.");
  ESP_RETURN_ON_ERROR(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL), TAG,
    "Failed registering event handler.");
  ESP_RETURN_ON_ERROR(esp_wifi_start(), TAG,
    "Failed starting WiFi AP.");

  // Set IP address
  tcpip_adapter_ip_info_t ip_info;
  ESP_RETURN_ON_ERROR(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_AP, &ip_info), TAG,
    "Failed reading WiFi AP IP address.");
  ip_address = ip4addr_ntoa(&ip_info.ip);  
  return ESP_OK;
};

esp_err_t HotSpot::stop() {
  ESP_RETURN_ON_ERROR(esp_wifi_stop(), TAG,
    "Failed stopping WiFi AP.");
  ESP_RETURN_ON_ERROR(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler), TAG,
    "Failed unregistering WiFi event handler.");
  ESP_RETURN_ON_ERROR(tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP), TAG,
    "Failed stopping DHCP server.");
  ESP_RETURN_ON_ERROR(esp_wifi_deinit(), TAG,
    "Failed deinitializing WiFi.");
  return ESP_OK;
};
