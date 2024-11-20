#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTClient.h>
#include <unistd.h> // For sleep()
#include <signal.h>
#include <wiringPi.h>

#define ADDRESS     "ssl://YOURURL:8883" // Broker address
#define CLIENTID    "paho-mqtt-subscriber" // Unique client ID
#define TOPIC       "test/topic"          // Subscription topic
#define QOS         1                     // Quality of Service
#define USERNAME    "YOURUSERNAME"      // MQTT username
#define PASSWORD    "YOURPASSWORD"        // MQTT password

#define LedPin   // TODO: DEFINE YOUR LED PIN

void setup()
{
    if (wiringPiSetup() == -1)
    {
        printf("setup wiringPi failed!\n");
        return;
    }

    pinMode(LedPin, OUTPUT);  // Set LedPin as output
}

void on(){
    digitalWrite(LedPin, HIGH);  // Turn on the LED
}

void off(){
    digitalWrite(LedPin, LOW);   // Turn off the LED
}


volatile int running = 1; // Flag to handle graceful shutdown

// Signal handler for Ctrl+C
void handle_signal(int signal) {
    running = 0;
}

// Callback function for handling received messages
int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    // Print the received message details
    printf("\nMessage arrived:\n");
    printf("Topic: %s\n", topicName);
    printf("Message: %.*s\n", message->payloadlen, (char *)message->payload); // Use payload length

    // TODO: If the message received is "on", turn on the LED
    
    // TODO: If the message received is "off", turn off the LED
    
    // TODO: If the message is not "on" or "off", print an the message received. 
    

    // Free the message and topic memory
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

    return 1; // Indicate message successfully processed
}

int main() {
	setup();
    signal(SIGINT, handle_signal); // Handle Ctrl+C to exit gracefully

    int rc;
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_SSLOptions ssl_opts = MQTTClient_SSLOptions_initializer;

    // Initialize MQTT client
    printf("Creating MQTT Client...\n");
    rc = MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("Failed to create MQTT client, return code %d\n", rc);
        return EXIT_FAILURE;
    }

    // Configure connection options
    conn_opts.keepAliveInterval = 20; // Keep-alive interval
    conn_opts.cleansession = 1;       // Start with a clean session
    conn_opts.username = USERNAME;   // Set username
    conn_opts.password = PASSWORD;   // Set password

    // Configure SSL/TLS
    ssl_opts.enableServerCertAuth = 1; // Enable server certificate verification
    ssl_opts.trustStore = "/etc/ssl/certs/ca-certificates.crt"; // Path to CA certificates
    conn_opts.ssl = &ssl_opts; // Attach SSL options to connection options

    // Set the callback function for incoming messages
    printf("Setting callback function...\n");
    rc = MQTTClient_setCallbacks(client, NULL, NULL, messageArrived, NULL);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("Failed to set callbacks, return code %d\n", rc);
        MQTTClient_destroy(&client);
        return EXIT_FAILURE;
    }

    // Connect to the broker
    printf("Attempting to connect to the broker...\n");
    rc = MQTTClient_connect(client, &conn_opts);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect to broker, return code %d\n", rc);
        switch (rc) {
            case MQTTCLIENT_FAILURE:
                printf("General failure.\n");
                break;
            case MQTTCLIENT_SSL_NOT_SUPPORTED:
                printf("SSL not supported.\n");
                break;
            default:
                printf("Unknown error.\n");
        }
        MQTTClient_destroy(&client);
        return EXIT_FAILURE;
    }
    printf("Connected to the broker successfully!\n");

    // Subscribe to the topic
    printf("Subscribing to topic: %s\n", TOPIC);
    rc = MQTTClient_subscribe(client, TOPIC, QOS);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("Failed to subscribe to topic %s, return code %d\n", TOPIC, rc);
        MQTTClient_disconnect(client, 1000);
        MQTTClient_destroy(&client);
        return EXIT_FAILURE;
    }
    printf("Successfully subscribed to topic: %s\n", TOPIC);

    // Wait for incoming messages
    printf("Waiting for messages... Press Ctrl+C to exit.\n");
    while (running) {
        sleep(1); // Keep the subscriber running
    }

    // Disconnect and clean up
    printf("Disconnecting from the broker...\n");
    MQTTClient_disconnect(client, 1000);
    MQTTClient_destroy(&client);
    printf("Disconnected and cleaned up.\n");

    return EXIT_SUCCESS;
}

