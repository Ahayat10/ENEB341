#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTClient.h>

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

    // Prepare and publish a message
    char *message = "Hello from the Paho MQTT Client with SSL/TLS!";
    pub_msg.payload = message;
    pub_msg.payloadlen = strlen(message);
    pub_msg.qos = QOS;
    pub_msg.retained = 0;

    // Publish the message
    printf("Publishing message...\n");
    rc = MQTTClient_publishMessage(client, TOPIC, &pub_msg, &token);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("Failed to publish message, return code %d\n", rc);
        MQTTClient_disconnect(client, 1000);
        MQTTClient_destroy(&client);
        return -1;
    }

    printf("Message published to topic: %s\n", TOPIC);

    // Wait for the message to be delivered
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("Failed to receive delivery confirmation, return code %d\n", rc);
    } else {
        printf("Message delivery confirmed!\n");
    }

    // Clean up and disconnect
    MQTTClient_disconnect(client, 1000);
    MQTTClient_destroy(&client);

    return 0;
}
