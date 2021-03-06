/*
 * UDP Fireflies
 * 
 * Based on example code by Tom Igoe
 */

#include <SPI.h>
//#include <WiFi101.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "config.h"         // Create a config.h file with chars ssid[] and pass[] for your wifi network

WiFiUDP Udp;                // instance of UDP library
IPAddress destination(10, 0, 1, 255); // UDP destination address for your network
const int port = 8888;      // port on which this client sends

class Firefly {
  // Class Member Variables
  int ledPin;
  long onLow;
  long onHigh;
  long offLow;
  long offHigh;
  unsigned long onTime;
  unsigned long offTime;

  // State Machine Variables
  int ledState;
  unsigned long previousMillis;

  //Constructor
  public:
  Firefly(int pin, long onl, long onh, long offl, long offh) {
    ledPin = pin;
    pinMode (ledPin, OUTPUT);

    onLow = onl;
    onHigh = onh;
    offLow = offl;
    offHigh = offh;
    
    onTime = random(onLow, onHigh);
    offTime = random(offLow, offHigh);

    ledState = LOW;
    previousMillis = 0;
  }

  void Update() {
    unsigned long currentMillis = millis();

    if ((ledState == HIGH) && (currentMillis - previousMillis >= onTime)) {
      ledState = LOW;
      previousMillis = currentMillis;
      onTime = random(onLow, onHigh);
      offTime = random(offLow, offHigh);
      digitalWrite(ledPin, ledState);
    }

    if ((ledState == LOW) && (currentMillis - previousMillis >= offTime)) {
      ledState = HIGH;
      previousMillis = currentMillis;
      onTime = random(onLow, onHigh);
      offTime = random(offLow, offHigh);
      digitalWrite(ledPin, ledState);
    }
  }
  void ResetTimer() {
    previousMillis = millis();    
  }
};

// Initialize LED objects

Firefly led1(5, 400, 1200, 3000, 30000);     // d1 Long, infrequent flashes
Firefly led2(4, 150, 400, 2500, 18000);      // d2 Medium flashes, medium frequency
Firefly led3(0, 500, 1500, 2000, 25000);     // d3 Long, infrequent flashes
Firefly led4(2, 75, 300, 1500, 8700);        // d4 Short, frequent flashes
Firefly led5(14, 200, 870, 3000, 12500);     // d5 Medium flashes, medium frequency
Firefly led6(12, 350, 500, 5000, 20000);     // d6 Medium flashes, medium frequency
Firefly led7(13, 50, 300, 8000, 40000);      // d7 Short, infrequent flashes

// Initialize global constants
const int timerMax = 1500000;
const int knockThreshold = 75;               // Takes a hard tap on the lid to activate

// Initialize global variables
int animateTimer = timerMax;                 // Set this to 0 if you want the lights to animate on boot

void setup() {

  Serial.begin(115200);       // use the serial port
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(5, HIGH);      // Flash the LED while connecting
    delay(500);
    Serial.print(".");
    digitalWrite(5, LOW);
    delay(500);
  }

  digitalWrite(4, HIGH);        // Turn another LED on high for 5 seconds when connected
  Serial.println("");
  Serial.println("WiFi connected");
  // When you're connected, print out the device's network status:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Udp.begin(port);
  delay(5000);

}

void loop() {

  readAnalogSensor();                 // Read analog sensor every 50 ms and trigger lights if above threshold

  // Check for UDP packets and animate LEDs if we received a packet which says "Flash"
  // if there's data available, read a packet
  if (Udp.parsePacket() > 0) {        // parse incoming packet
    String message = "";              
    Serial.print("From: ");           // print the sender
    Serial.print(Udp.remoteIP());
    Serial.print(" on port: ");       // and the port they sent on
    Serial.println(Udp.remotePort());
    while (Udp.available() > 0) {     // parse the body of the message
      message = Udp.readString();
    }
    Serial.println("msg: " + message);  // print it
    if (message == "Flash") {              // if the message is "Flash"
      // Blank out some of the LEDs
      led1.ResetTimer();
      // led2.ResetTimer();                 // commented out so that I get feedback on a button event
      led3.ResetTimer();
      led4.ResetTimer();
      led5.ResetTimer();
      // led6.ResetTimer();
      led7.ResetTimer();

      animateTimer = 0;                 // activate the LEDs

    }
  }

  // TODO: change this to using millis(); to get a precise control over timing
  if (animateTimer <= timerMax) {
    led1.Update();
    led2.Update();
    led3.Update();
    led4.Update();
    led5.Update();
    led6.Update();
    led7.Update();
    animateTimer++;
  }
  // If an LED was left on at the end of the animateTimer loop, turn it off
  if (animateTimer >= timerMax) {
    digitalWrite(5, LOW);
    digitalWrite(4, LOW);
    digitalWrite(0, LOW);
    digitalWrite(2, LOW);
    digitalWrite(14, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
  }
}

void readAnalogSensor() {
  if( millis() % 50 != 0 )            // If you poll the analogRead function at every loop it knocks the arduino off wifi; this calls analogRead only every 50 ms
    return;
  int knockState = analogRead(A0); 
  if (knockState >= knockThreshold) {
    delay(30);                        // debounce delay
    
    // start a new packet:
    Udp.beginPacket(destination, port);
    Udp.print("Flash");                 // add payload to it
    Udp.endPacket();                    // finish and send packet
    Serial.print("Sent packet to ");
    Serial.print(destination);
    Serial.println(" on port: " + String(port));
    
    // Blank out some of the LEDs
    // led1.ResetTimer();                 // commented out so that I get feedback on a knock event
    led2.ResetTimer();
    led3.ResetTimer();
    // led4.ResetTimer();
    led5.ResetTimer();
    led6.ResetTimer();
    led7.ResetTimer();

    // activate the LEDs
    animateTimer = 0;
  }
}
