#include <SPI.h>
#include <LoRa.h>

// Define the pins for SPI and LoRa
#define SCK     18  // GPIO18 - SCK pin
#define MISO    19  // GPIO19 - MISO pin
#define MOSI    23  // GPIO23 - MOSI pin
#define SS      5   // GPIO5 - CS (NSS) pin
#define RST     14  // GPIO14 - RESET pin
// No DIO0 pin needed

void setup() {
  // Start Serial communication for debugging
  Serial.begin(9600);
  
  // Initialize LoRa module
  Serial.println("Initializing LoRa...");
  LoRa.setPins(SS, RST);  // Set LoRa pins without DIO0
  
  // Check if LoRa module initializes successfully
  if (!/*TODO: Begin and set frequency*/) {  
    Serial.println("LoRa initialization failed. Check your connections.");
    while (true);
  }
  
  // Set the sync word
  // LoRa.setSyncWord(0x12);  // Private network sync word
  
  Serial.println("LoRa initialization successful.");
}

void loop() {
  // Send a packet
  Serial.println("Sending packet: Hello, LoRa with Sync Word");
  
  
  /*TODO begin packet*/
  /*TODO print the message. this command is not on the API, use lora.print()*/
  /*TODO end packet*/

  // Wait 5 seconds before sending the next packet
  delay(5000);
}
