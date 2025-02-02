#pragma once

#define ESP_RETURN_ON_ERROR(ret, tag, msg, ...) \
  if ((ret) != ESP_OK) { \
    ESP_LOGE((tag), (msg), ##__VA_ARGS__); \
    ESP_LOGE((tag), "ERR: %s", esp_err_to_name(ret)); \
    return (ret); \
  }

#define ESP_CLEANUP_ON_ERROR(ret, tag, msg, ...) \
  if ((ret) != ESP_OK) { \
    esp_err = (ret); \
    ESP_LOGE((tag), (msg), ##__VA_ARGS__); \
    ESP_LOGE((tag), "ERR: %s", esp_err_to_name(ret)); \
    goto cleanup; \
  }
