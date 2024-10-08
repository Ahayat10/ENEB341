#include <esp_sleep.h>  // Required for sleep functions

const int ledPin = 2;  // Define the LED pin (GPIO 2)

void setup() {
  // Initialize the LED pin as an output
  pinMode(ledPin, OUTPUT);
  
  // Initialize serial communication (optional, for debugging)
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  
  Serial.println("ESP32 is entering light sleep mode.");
}

void loop() {
  // Flash the LED on and off
  digitalWrite(ledPin, HIGH);   // Turn the LED on
  delay(500);                   // Wait for 500 milliseconds
  digitalWrite(ledPin, LOW);    // Turn the LED off
  delay(500);                   // Wait for 500 milliseconds
  
  // Set wake-up timer for light sleep (e.g., wake up after 2 seconds)
  esp_sleep_enable_timer_wakeup(2000000); // Wake up after 2,000,000 microseconds (2 seconds)
  
  // Enter light sleep
  Serial.println("Entering light sleep...");
  esp_light_sleep_start();  // Enter light sleep mode
  
  // Wake up and continue after light sleep
  Serial.println("Woke up from light sleep.");
}
