#include "leds.h"

static const char* TAG = "Leds";

static const gpio_num_t LED_1 = GPIO_NUM_12;
static const gpio_num_t LED_2 = GPIO_NUM_14;
static const gpio_num_t LED_3 = GPIO_NUM_16;


esp_err_t Leds::init() {
  gpio_config_t io_conf = {};
  io_conf.pin_bit_mask = (1ULL << LED_1) | (1ULL << LED_2) | (1ULL << LED_3);
  io_conf.mode         = GPIO_MODE_OUTPUT;
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf.pull_up_en   = GPIO_PULLUP_DISABLE;
  io_conf.intr_type    = GPIO_INTR_DISABLE;
  ESP_RETURN_ON_ERROR(gpio_config(&io_conf), TAG, "IO config failed.");
  ESP_LOGI(TAG, "IO config ok.");
  return ESP_OK;
};

esp_err_t Leds::toggle_led_1() {
  ESP_RETURN_ON_ERROR(toggle_led(LED_1), TAG, "Failed to toggle Led 1.");
  ESP_LOGI(TAG, "Toggled Led 1.");
  return ESP_OK;
};

esp_err_t Leds::toggle_led_2() {
  ESP_RETURN_ON_ERROR(toggle_led(LED_2), TAG, "Failed to toggle Led 2.");  
  ESP_LOGI(TAG, "Toggled Led 2.");
  return ESP_OK;
};

esp_err_t Leds::toggle_led_3() {
  ESP_RETURN_ON_ERROR(toggle_led(LED_3), TAG, "Failed to toggle Led 3.");  
  ESP_LOGI(TAG, "Toggled Led 3.");
  return ESP_OK;
};

esp_err_t Leds::toggle_led(gpio_num_t gpio_num) {
  ESP_RETURN_ON_ERROR(gpio_set_level(gpio_num, 1), TAG, "Set gpio%d high failed.", (int)gpio_num);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  ESP_RETURN_ON_ERROR(gpio_set_level(gpio_num, 0), TAG, "Set gpio%d low failed.", (int)gpio_num);
  vTaskDelay(500 / portTICK_PERIOD_MS);
  return ESP_OK;
};
