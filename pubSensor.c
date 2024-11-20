#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTClient.h>
#include "bmp180.h"
#include <unistd.h>

#define ADDRESS     "ssl://cbe77b9c7f3c47ad9fe9dd706eba633d.s1.eu.hivemq.cloud:8883"  // SSL port
#define CLIENTID    "paho-mqtt-publisher"
#define TOPIC       "test/topic"
#define QOS         1
#define TIMEOUT     10000L  // 10 seconds timeout for waiting for delivery
#define USERNAME    "testConnection"
#define PASSWORD    "mqttConn123!"



int main() {
    int rc;
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pub_msg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    // Set SSL options
    MQTTClient_SSLOptions ssl_opts = MQTTClient_SSLOptions_initializer;
    ssl_opts.enableServerCertAuth = 0; // Disable server certificate authentication (can enable if needed)
    
    // Initialize MQTT Client
    rc = MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("Failed to create client, return code %d\n", rc);
        return -1;
    }

    // Set connection options with username, password, and SSL options
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = USERNAME;
    conn_opts.password = PASSWORD;
    conn_opts.ssl = &ssl_opts;  // Set the SSL options to the connection options

    // Attempt to connect to the broker
    printf("Attempting to connect to the broker...\n");
    rc = MQTTClient_connect(client, &conn_opts);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect to broker, return code %d\n", rc);
        MQTTClient_destroy(&client);
        return -1;
    }

    printf("Connected to the broker successfully!\n");
    
   
	  // Initialize BMP180 sensor (make sure you have a BMP180 library or code for this)
    char *i2c_device = "/dev/i2c-1";
    int address = 0x77;
    void *bmp = bmp180_init(address, i2c_device);  // Initialize the BMP180 sensor

    if(bmp != NULL){
        int i;
        for(i = 0; i < 100; i++) {
            // Read sensor data
            float temperature = bmp180_temperature(bmp);
            long pressure = bmp180_pressure(bmp);
            float altitude = bmp180_altitude(bmp);

            // Create a formatted string message with sensor readings
            char message[256];  // Buffer to hold the message
            snprintf(message, sizeof(message), "Temperature: %.2f°C, Pressure: %ld Pa, Altitude: %.2f m", 
                    temperature, pressure, altitude);
            
            // Prepare the message for publishing
            pub_msg.payload = message;
            pub_msg.payloadlen = strlen(message);
            pub_msg.qos = QOS;
            pub_msg.retained = 0;

            // Publish the message to the topic
            printf("Publishing sensor data: %s\n", message);
            rc = MQTTClient_publishMessage(client, TOPIC, &pub_msg, &token);
            if (rc != MQTTCLIENT_SUCCESS) {
                printf("Failed to publish message, return code %d\n", rc);
                MQTTClient_disconnect(client, 1000);
                MQTTClient_destroy(&client);
                return -1;
            }

            // Wait for message delivery confirmation
            rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
            if (rc != MQTTCLIENT_SUCCESS) {
                printf("Failed to receive delivery confirmation, return code %d\n", rc);
            } else {
                printf("Message delivery confirmed!\n");
            }

            // Sleep for a while before taking the next reading (2 seconds)
            usleep(2 * 1000 * 1000);  // 2 seconds
        }
    } else {
        printf("Failed to initialize BMP180 sensor\n");
    }
    // Clean up and disconnect
    MQTTClient_disconnect(client, 1000);
    MQTTClient_destroy(&client);

    return 0;
}
