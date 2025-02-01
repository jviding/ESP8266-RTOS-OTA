#pragma once

#define ESP_RETURN_ON_ERROR(ret, tag, msg, ...) \
  if ((ret) != ESP_OK) { \
    ESP_LOGE((tag), (msg), ##__VA_ARGS__); \
    return (ret); \
  }
