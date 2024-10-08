#include <esp_sleep.h>    // For sleep functions
#include <driver/rtc_io.h> // For controlling GPIO during sleep

const int ledPin = 2;  // Define the LED pin (GPIO 2)

// Deep sleep wake-up period (e.g., 2 seconds = 2,000,000 microseconds)
const uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for microseconds to seconds
const uint64_t TIME_TO_SLEEP = 1;         // Time ESP32 stays in deep sleep (in seconds)

void setup() {
  // Initialize serial communication for debugging (optional)
  Serial.begin(115200);
  delay(1000);  // Delay for serial monitor to initialize
  
  // Initialize the LED pin as an output
  pinMode(ledPin, OUTPUT);

  // Check the reason for wake-up
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  
  // Flash the LED only if ESP32 wakes up from deep sleep
  if (wakeup_reason == ESP_SLEEP_WAKEUP_TIMER) {
    // Flash LED before going back to deep sleep
    Serial.println("Woke up from deep sleep, flashing LED...");
    digitalWrite(ledPin, HIGH);  // Turn LED on
    delay(500);                  // LED stays on for 500 ms
    digitalWrite(ledPin, LOW);   // Turn LED off
    delay(500);                  // LED stays off for 500 ms
  }

  // Configure deep sleep with wake-up timer
  Serial.println("Entering deep sleep for 1 second...");
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);  // Wake up after 1 second
  esp_deep_sleep_start();  // Enter deep sleep
}

void loop() {
  // The loop will not be executed in deep sleep, so no need to implement anything here
}
