#include "tests.h"
#include "hotspot.h"
#include "esp_log.h"

static void test_hotspot_cannot_set_ssid_nullptr() {
  TEST_ASSERT_NOT_OK(HotSpot::set_ssid(nullptr));
};

static void test_hotspot_cannot_set_ssid_too_short() {
  TEST_ASSERT_NOT_OK(HotSpot::set_ssid(""));
};

static void test_hotspot_cannot_set_ssid_too_long() {
  TEST_ASSERT_NOT_OK(HotSpot::set_ssid("01234567890123456789012345678901")); // 32 char
};

static void test_hotspot_cannot_set_password_nullptr() {
  TEST_ASSERT_NOT_OK(HotSpot::set_password(nullptr));
}

static void test_hotspot_cannot_set_password_too_short() {
  TEST_ASSERT_NOT_OK(HotSpot::set_password("1234567")); // 7 char
};

static void test_hotspot_cannot_set_password_too_long() {
  TEST_ASSERT_NOT_OK(HotSpot::set_password("0123456789012345678901234567890123456789012345678901234567890123")); // 64 char
};

static void test_hotspot_cannot_start_without_ssid_set() {
  TEST_ASSERT_NOT_OK(HotSpot::start(false));
  TEST_ASSERT_NOT_OK(HotSpot::start(true));
};

static void test_hotspot_can_start_with_no_auth() {
  TEST_ASSERT_OK(HotSpot::start(false));
};

static void test_hotspot_can_start_with_random_pwd_auth() {
  TEST_ASSERT_OK(HotSpot::start(true));
};

static void test_hotspot_can_start_with_pwd_auth() {
  TEST_ASSERT_OK(HotSpot::start(true));
};

static void test_hotspot_can_stop() {
  TEST_ASSERT_OK(HotSpot::stop());
};

void Tests::test_hotspot() {
  // Test values SSID
  RUN_TEST(test_hotspot_cannot_set_ssid_nullptr);
  RUN_TEST(test_hotspot_cannot_set_ssid_too_short);
  RUN_TEST(test_hotspot_cannot_set_ssid_too_long);
  // Test values Password
  RUN_TEST(test_hotspot_cannot_set_password_nullptr);
  RUN_TEST(test_hotspot_cannot_set_password_too_short);
  RUN_TEST(test_hotspot_cannot_set_password_too_long);
  // Test start/stop
  RUN_TEST(test_hotspot_cannot_start_without_ssid_set);
  HotSpot::set_ssid("Test");
  RUN_TEST(test_hotspot_can_start_with_no_auth);
  TEST_WAIT_MS(5000);
  RUN_TEST(test_hotspot_can_stop);
  TEST_WAIT_MS(5000);
  RUN_TEST(test_hotspot_can_start_with_random_pwd_auth);
  TEST_WAIT_MS(5000);
  RUN_TEST(test_hotspot_can_stop);
  TEST_WAIT_MS(5000);
  HotSpot::set_password("0123456789");
  RUN_TEST(test_hotspot_can_start_with_pwd_auth);
  TEST_WAIT_MS(5000);
  RUN_TEST(test_hotspot_can_stop);
  // Done
  ESP_LOGI("TEST", "HOTSPOT TESTS COMPLETED.");
};
