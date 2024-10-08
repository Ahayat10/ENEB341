#include <WiFi.h>
#include <esp_bt.h> 

const int ledPin = 2;

void setModemSleep();

void setup() {
    Serial2.begin(115200);
    pinMode(ledPin, OUTPUT);

    while (!Serial2) {
        delay(500);
    }

    setModemSleep();  
    Serial2.println("MODEM SLEEP ENABLED FOR 5secs");
    digitalWrite(ledPin, LOW);
}

void loop() {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
}

void disableWiFi() {
    WiFi.mode(WIFI_OFF);
    Serial2.println("WiFi disconnected!");
}

void disableBluetooth() {
    btStop();
    Serial2.println("Bluetooth stopped!");
}

void setModemSleep() {
    disableWiFi();
    disableBluetooth();
    setCpuFrequencyMhz(40);
}

