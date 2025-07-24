#include "storage.h"
#include "esp_log.h"
#include "esp_check.h"
#include "nvs_flash.h"

static const char* TAG = "Storage";

static const char* NVS_NAME = "storage";
static const char* KEY_SSID = "SSID";
static const char* KEY_PWD  = "pwd";


esp_err_t Storage::init() {
  ESP_RETURN_ON_ERROR(nvs_flash_init(), TAG, 
    "Failed initializing NVS partition.");
  return ESP_OK;
};

esp_err_t Storage::read_wifi_credentials(char** ssid, char** password) {

  esp_err_t res_ssid = read_value(KEY_SSID, ssid);
  esp_err_t res_pwd = read_value(KEY_PWD, password);
  
  if ((res_ssid == ESP_OK) && (res_pwd == ESP_OK)) {
    return ESP_OK;
  } else if (res_ssid == ESP_ERR_NVS_NOT_FOUND) {
    ESP_LOGE(TAG, "Failed reading WiFi SSID, not found.");
    return ESP_ERR_NVS_NOT_FOUND;
  } else if (res_pwd == ESP_ERR_NVS_NOT_FOUND) {
    ESP_LOGE(TAG, "Failed reading WiFi password, not found.");
    return ESP_ERR_NVS_NOT_FOUND;
  } else {
    ESP_LOGE(TAG, "Failed reading WiFi credentials, something went wrong.");
    return ESP_FAIL;
  }
};

esp_err_t Storage::save_wifi_credentials(char* ssid, char* password) {

  esp_err_t esp_err = ESP_OK;
  nvs_handle my_handle;

  ESP_CLEANUP_ON_ERROR(nvs_open(NVS_NAME, NVS_READWRITE, &my_handle), esp_err, TAG, 
    "Write failed, error opening NVS RW handle.");
  ESP_CLEANUP_ON_ERROR(nvs_set_str(my_handle, KEY_SSID, ssid), esp_err, TAG, 
    "Write failed, error setting WiFi SSID in NVS RW handle.");
  ESP_CLEANUP_ON_ERROR(nvs_set_str(my_handle, KEY_PWD, password), esp_err, TAG, 
    "Write failed, error setting WiFi password in NVS RW handle.");
  ESP_CLEANUP_ON_ERROR(nvs_commit(my_handle), esp_err, TAG, 
    "Write failed, error committing NVS RW handle.");

cleanup:
  nvs_close(my_handle);
  return esp_err;
};

esp_err_t Storage::read_value(const char* key, char** value) {

  esp_err_t esp_err = ESP_OK;
  nvs_handle my_handle;
  char* temp_val = nullptr;
  size_t val_length;

  ESP_CLEANUP_ON_ERROR(nvs_open(NVS_NAME, NVS_READONLY, &my_handle), esp_err, TAG, 
    "Error opening NVS RO handle.");
  ESP_CLEANUP_ON_ERROR(nvs_get_str(my_handle, key, NULL, &val_length), esp_err, TAG, 
    "Error reading %s length.", key);

  temp_val = new char[val_length];
  
  ESP_CLEANUP_ON_ERROR(nvs_get_str(my_handle, key, temp_val, &val_length), esp_err, TAG, 
    "Error reading %s value.", key);
  
  delete[] *value;
  *value = temp_val;
  temp_val = nullptr;

cleanup:
  nvs_close(my_handle);
  delete[] temp_val;
  return esp_err;
};
