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
  
  Serial.println("LoRa initialization successful. Waiting for packets...");
}

void loop() {
  // Try to parse the packet
  int packetSize = /*TODO: Parse the packet*/;
  
  // If a packet is received
  if (packetSize) {
    // Print the incoming packet
    Serial.print("Received packet: ");
    
    // Read and print the packet
    while (/*TODO: Make sure bytes are available for reading*/) {
      String incoming = /*TODO: Read the string. this is not in the api, use LoRa.readString()*/;
      Serial.print(incoming);
    }
    
    // Print RSSI (Received Signal Strength Indicator)
    Serial.print(" with RSSI: ");
    Serial.println(/*TODO: Find the RSSI of the packet*/);
  }
}
