#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


class Tests {
  public:
    // Functions
    static void test();

  private:
    // Test HotSpot
    static void test_hotspot();
};

#define TEST_WAIT_MS(milliseconds) vTaskDelay(milliseconds / portTICK_PERIOD_MS);

#define RUN_TEST(func) \
  do { \
    ESP_LOGI("Run", #func); \
    func(); \
  } while (0);

#define TEST_PRINT_RESULT(res) \
  do { \
    if (!!(res)) { \
      ESP_LOGI("Test result", "PASS"); \
    } else { \
      ESP_LOGE("Test result", "FAIL"); \
      while (true) { \
        ; \
      } \
    } \
  } while(0);


#define TEST_ASSERT_EQUAL(val_1, val_2) TEST_PRINT_RESULT(((val_1) == (val_2)))
#define TEST_ASSERT_NOT_EQUAL(val_1, val_2) TEST_PRINT_RESULT(((val_1) != (val_2)))

#define TEST_ASSERT_OK(val) TEST_PRINT_RESULT(((ESP_OK) == (val)))
#define TEST_ASSERT_NOT_OK(val) TEST_PRINT_RESULT(((ESP_OK) != (val)))
