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
  if (!LoRa.begin(901E6)) {  // 915 MHz frequency (adjust for your region, 433E6 or 868E6)
    Serial.println("LoRa initialization failed. Check your connections.");
    while (true);
  }
  
  // Set the sync word
  // LoRa.setSyncWord(0x14);  // Private network sync word
  
  Serial.println("LoRa initialization successful. Waiting for packets...");
}

void loop() {
  // Try to parse the packet
  int packetSize = LoRa.parsePacket();
  
  // If a packet is received
  if (packetSize) {
    // Print the incoming packet
    Serial.print("Received packet: ");
    
    // Read and print the packet
    while (LoRa.available()) {
      String incoming = LoRa.readString();
      Serial.print(incoming);
    }
    
    // Print RSSI (Received Signal Strength Indicator)
    Serial.print(" with RSSI: ");
    Serial.println(LoRa.packetRssi());
  }
}
