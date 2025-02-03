#include "storage.h"

#include "esp_log.h"
#include "esp_check.h"
#include "nvs_flash.h"


static const char* TAG = "Storage";

static const char* NVS_NAME = "storage";
static const char* KEY_SSID = "SSID";
static const char* KEY_PWD  = "pwd";


esp_err_t Storage::init() {
  ESP_RETURN_ON_ERROR(nvs_flash_init(), TAG, "Error initializing NVS partition.");
  return ESP_OK;
};

esp_err_t Storage::read_wifi_credentials(char** ssid, char** password) {
  esp_err_t res_ssid = read_value(KEY_SSID, ssid);
  esp_err_t res_pwd = read_value(KEY_PWD, password);
  // Check for ok
  bool ssid_ok = (res_ssid == ESP_OK);
  bool pwd_ok = (res_pwd == ESP_OK);
  if (ssid_ok && pwd_ok) {
    ESP_LOGI(TAG, "WiFi credentials read.");
    return ESP_OK;
  }
  // Check for not found
  bool ssid_nf = ssid_ok || (res_ssid == ESP_ERR_NVS_NOT_FOUND);
  bool pwd_nf = pwd_ok || (res_pwd == ESP_ERR_NVS_NOT_FOUND);
  if (ssid_nf && pwd_nf) {
    ESP_LOGI(TAG, "WiFi credentials not found.");
    return ESP_ERR_NVS_NOT_FOUND;
  }  
  // Something went wrong
  delete[] *ssid;
  delete[] *password;
  ESP_RETURN_ON_ERROR(ESP_FAIL, TAG, "Failed reading WiFi credentials.");
};

esp_err_t Storage::save_wifi_credentials(char* ssid, char* password) {
  esp_err_t esp_err = ESP_OK;
  nvs_handle my_handle;
  ESP_CLEANUP_ON_ERROR(nvs_open(NVS_NAME, NVS_READWRITE, &my_handle), TAG, "Error opening NVS RW handle.");
  ESP_CLEANUP_ON_ERROR(nvs_set_str(my_handle, KEY_SSID, ssid), TAG, "Error setting SSID.");
  ESP_CLEANUP_ON_ERROR(nvs_set_str(my_handle, KEY_PWD, password), TAG, "Error setting Password.");
  ESP_CLEANUP_ON_ERROR(nvs_commit(my_handle), TAG, "Error committing NVS RW handle.");
  ESP_LOGI(TAG, "WiFi credentials saved.");
cleanup:
  nvs_close(my_handle);
  return esp_err;
};

esp_err_t Storage::read_value(const char* key, char** value) {
  esp_err_t esp_err = ESP_OK;
  nvs_handle my_handle;
  size_t len;
  char* tmp = nullptr;
  // Read value length
  ESP_CLEANUP_ON_ERROR(nvs_open(NVS_NAME, NVS_READONLY, &my_handle), TAG, "Error opening NVS RO handle.");
  ESP_CLEANUP_ON_ERROR(nvs_get_str(my_handle, key, NULL, &len), TAG, "Error reading %s length.", key);
  // Allocate and read value
  tmp = new char[len];
  ESP_CLEANUP_ON_ERROR(nvs_get_str(my_handle, key, tmp, &len), TAG, "Error reading %s value.", key)
  // Set value
  delete[] *value;
  *value = tmp;
  tmp = nullptr;
cleanup:
  nvs_close(my_handle);
  delete[] tmp;
  return esp_err;
};
