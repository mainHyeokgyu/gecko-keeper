#include <Arduino.h>
#include <unity.h>

#define RELAY_PIN 5

// Active HIGH 릴레이: HIGH = ON, LOW = OFF
void test_relay_initial_state_is_off() {
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);
    delay(10);
    TEST_ASSERT_EQUAL(LOW, digitalRead(RELAY_PIN));
}

void test_relay_turns_on() {
    digitalWrite(RELAY_PIN, HIGH);
    delay(1000);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(RELAY_PIN));
}

void test_relay_turns_off() {
    digitalWrite(RELAY_PIN, LOW);
    delay(1000);
    TEST_ASSERT_EQUAL(LOW, digitalRead(RELAY_PIN));
}

void setup() {
    delay(1000);
    UNITY_BEGIN();
    RUN_TEST(test_relay_initial_state_is_off);
    RUN_TEST(test_relay_turns_on);
    RUN_TEST(test_relay_turns_off);
    UNITY_END();
}

void loop() {}
