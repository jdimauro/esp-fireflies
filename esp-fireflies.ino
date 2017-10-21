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
Firefly led1(5, 200, 600, 3000, 30000);     // d1 
Firefly led2(4, 150, 700, 2500, 18000);     // d2
Firefly led3(0, 370, 800, 2000, 30000);     // d3
Firefly led4(2, 100, 670, 3000, 8700);      // d4
Firefly led5(14, 200, 870, 3000, 12500);    // d5 
Firefly led6(12, 250, 900, 5000, 20000);    // d6
Firefly led7(13, 150, 860, 2600, 50000);    // d7

// Initialize global constants

const int timerMax = 1000000;
const int knockThreshold = 15;

// Initialize global variables

int animateTimer = timerMax;


void setup() {
  Serial.begin(115200);       // use the serial port

}

void loop() {
  // TODO: change this to using piezo knock sensor on an analogRead(); function
  int knockState = analogRead(A0); 
  if (knockState >= knockThreshold) {
    // Blank out some of the LEDs
    // led1.ResetTimer();                 // commented out so that I get feedback on a button event
    led2.ResetTimer();
    led3.ResetTimer();
    // led4.ResetTimer();
    led5.ResetTimer();
    led6.ResetTimer();
    led7.ResetTimer();

    // TODO: Send out a broadcast UDP ping that says "knock"
    Serial.println("Knock!");

    // activate the LEDs
    animateTimer = 0;
  }

  // TODO: Check for UDP packets and animate LEDs if we received a packet which says "knock"

  // TODO: change this to using millis(); to get a precise control over timing
  while(animateTimer <= timerMax) {
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
    //Serial.println("Switch off");
    digitalWrite(5, LOW);
    digitalWrite(4, LOW);
    digitalWrite(0, LOW);
    digitalWrite(2, LOW);
    digitalWrite(14, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
  }
}

