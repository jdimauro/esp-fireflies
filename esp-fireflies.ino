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
Firefly led1(13, 200, 600, 3000, 30000);
Firefly led2(12, 150, 700, 2500, 18000);
Firefly led3(11, 370, 800, 2000, 30000);
Firefly led4(10, 100, 670, 3000, 8700);
Firefly led5(9, 200, 870, 3000, 12500);
Firefly led6(8, 250, 900, 5000, 20000);
Firefly led7(7, 150, 860, 2600, 50000);

// Initialize global variables
const int buttonPin = 2;      // activation pushbutton before piezo sensor testing
int lastButtonState = HIGH;   // initial pushbutton state
int timerMax = 1000000;
int animateTimer = timerMax;


void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  randomSeed(analogRead(0));
}

void loop() {
  // TODO: change this to using piezo knock sensor on an analogRead(); function
  int buttonState = digitalRead(buttonPin); 
  if (buttonState != lastButtonState) {
    delay(3);                               // debounce delay
    if (buttonState == LOW) {
      // activate the LEDs for 30 seconds
      // led1.ResetTimer();                 // commented out so that I get feedback on a button event
      led2.ResetTimer();
      led3.ResetTimer();
      // led4.ResetTimer();
      led5.ResetTimer();
      led6.ResetTimer();
      led7.ResetTimer();
      animateTimer = 0;
    }
    lastButtonState = buttonState;          // save button state
  }

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
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
  }
}

