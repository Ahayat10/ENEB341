#include <esp_now.h>
#include <WiFi.h>

unsigned int led_pin = 2; //On Board LED - Do not change
 
// Declare an array named letters that holds addresses of string literals 
// (i.e. an array of pointers to strings composed of dots and dashes) 
// Done to preserve memory because strings are not equal in size. A 2D array
// would be a waste of space.
char *letters[] = {
  // The letters A-Z in Morse code  
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",    
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",  
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."         
};
 
char *numbers[] = {
  // The numbers 0-9 in Morse code  
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", 
  "--...", "---..", "----."
};
 
unsigned int dot_duration = 200;
bool done = false;


/*
// TODO: INSERT Structure to receive data
// Must match the sender structure
*/


/**
 *  Function runs only once, after each powerup or reset of the Arduino
 */
void setup() {
  // Set the LED to output
  pinMode(led_pin, OUTPUT);
   
  // Open the serial port and set the data transmission rate to 9600 bits 
  // per second. 9600 is the default baud rate for Arduino.
  Serial.begin(9600); 

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
   

}


// Callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  Serial.println(myData.a);

  /* 
  TODO: Insert code to copy incoming data
  */

  Serial.print("Message received: ");

  /*
  TODO: Print the copied data
  */
  
  // Now process the received message in Morse code
  char *ch = /*TODO: Set *ch to the data copied*/;
  while (*ch != '\0') {
    // Check for uppercase letters
    if (*ch >= 'A' && *ch <= 'Z') {
      flash_morse_code(letters[*ch - 'A']);
    }
    // Check for lowercase letters
    else if (*ch >= 'a' && *ch <= 'z') {
      flash_morse_code(letters[*ch - 'a']);
    }
    // Check for numbers
    else if (*ch >= '0' && *ch <= '9') {
      flash_morse_code(numbers[*ch - '0']);
    }
    // Check for space between words
    else if (*ch == ' ') {
      delay(dot_duration * 7); // Space between words equals seven dots
    }
    ch++; // Move to the next character
  }
}
 
 
/**
  *  Flashes the Morse code for the input letter or number
  *  @param morse_code pointer to the morse code
  */
void flash_morse_code(char *morse_code) {
    
  unsigned int i = 0;
    
  // Read the dots and dashes and flash accordingly
  while (morse_code[i] != NULL) {
    flash_dot_or_dash(morse_code[i]);
    i++;
  }
    
  // Space between two letters is equal to three dots
  delay(dot_duration * 3);    
}
 
/**
  *  Flashes the dot or dash in the Morse code
  *  @param dot_or_dash character that is a dot or a dash
  */
void flash_dot_or_dash(char dot_or_dash) {
   
  // Make the LED shine
  digitalWrite(led_pin, HIGH);
   
  if (dot_or_dash == '.') { // If it is a dot
    delay(dot_duration);           
  }
  else { // Has to be a dash...equal to three dots
    delay(dot_duration * 3);           
  }
   
  // Turn the LED off
  digitalWrite(led_pin, LOW);
 
  // Give space between parts of the same letter...equal to one dot
  delay(dot_duration); 
}


void loop() {
}