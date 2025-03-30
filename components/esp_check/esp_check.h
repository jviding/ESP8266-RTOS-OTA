#pragma once

#define ESP_RETURN_ON_ERROR(ret, tag, msg, ...) \
  do { \
    esp_err_t __err = (ret); \
    if (__err != ESP_OK) { \
      ESP_LOGE((tag), (msg), ##__VA_ARGS__); \
      ESP_LOGE((tag), "ERR: %s", esp_err_to_name(__err)); \
      return (__err); \
    } \
  } while (0);

#define ESP_CLEANUP_ON_ERROR(ret, esp_err_var, tag, msg, ...) \
  do { \
    esp_err_t __err = (ret); \
    if (__err != ESP_OK) { \
        (esp_err_var) = (__err); \
        ESP_LOGE((tag), (msg), ##__VA_ARGS__); \
        ESP_LOGE((tag), "ERR: %s", esp_err_to_name(__err)); \
        goto cleanup; \
      } \
  } while (0);
