// pin assignments
int pinButton = 10;  // defines D10 for pullup button switch
int pinRelay = 3;
int pinLedLeft = 5;
int pinLedRight = 6;
int pinSquareWave = 2;  // defines D2 for square wave output signal

int stateButton;
int previous = LOW;
long time = 0;
long debounce = 500;  // default of 500 ms works well
int stayON = 1000;    // keeps relay on (HIGH) for 1000 ms or 1 second faster switching might be acceptable.
int voltageA0 = 0;
int voltageA1 = 0;
//int resetADC;  // A2 grounded to reset ADC
float freq = 15.0;  // default square wave frequency in kHz for testing the capacitors

void setup() {
  pinMode(pinButton, INPUT);  // defines D10 as an INPUT to activate the button switch
  pinMode(pinRelay, OUTPUT);  // defines D3 as an OUTPUT to control the relay

  pinMode(pinLedLeft, OUTPUT);   // defines D5 as an OUTPUT to drive the right facing LED
  pinMode(pinLedRight, OUTPUT);  // defines D6 as an OUTPUT to drive the left facing LED

  Serial.begin(9600);

  Serial.print("Test Frequency: ");
  Serial.print(freq);
  Serial.println(" kHz");

  tone(pinSquareWave, freq * 1000);
}

void loop() {
  /*
  // testing code
  digitalWrite(pinRelay, LOW);
  digitalWrite(pinLedLeft, HIGH);
  digitalWrite(pinLedRight, LOW);
  Serial.println("A");
  delay(stayON);
  digitalWrite(pinLedLeft, LOW);
  digitalWrite(pinLedRight, HIGH);
  digitalWrite(pinRelay, HIGH);
  Serial.println("B");
  delay(stayON);
  */

  stateButton = digitalRead(pinButton);
  if (stateButton == HIGH && previous == LOW & millis() - time > debounce) {

    // Print a message
    Serial.println("Testing...");

    // Clear leds
    digitalWrite(pinLedLeft, LOW);   // D6 off - (left LED)
    digitalWrite(pinLedRight, LOW);  // D5 on - (right LED)
    delay(stayON);

    //turn on relais
    digitalWrite(pinRelay, HIGH);

    // Let things stabilise
    delay(stayON);

    // Clear ADC -> is this really needed?
    //resetADC = analogRead(A2);  // A2 is grounded to clear the ADC before reading A0 & A1

    // Measure signal strength
    float voltageA0 = analogRead(A0) * (5.0 / 1023.0);
    Serial.print("Value0: ");
    Serial.println(voltageA0, 4);

    // turn off relais
    digitalWrite(pinRelay, LOW);  // D13 off

    // Let things stabilise
    delay(stayON);

    // Measure signal strength
    float voltageA1 = analogRead(A0) * (5.0 / 1023.0);
    Serial.print("Value1: ");
    Serial.println(voltageA1, 4);

    // Show the result.
    if (voltageA0 > voltageA1) {
      digitalWrite(pinLedLeft, LOW);    // D6 off - (left LED)
      digitalWrite(pinLedRight, HIGH);  // D5 on - (right LED) to indicate Foil Side of Capacitor
    } else if (voltageA0 < voltageA1) {
      digitalWrite(pinLedLeft, HIGH);  // D6 on - (left LED) to indicate Foil Side of Capacitor
      digitalWrite(pinLedRight, LOW);  // D5 off - (right LED)
    }

    time = millis();
  }
  previous == stateButton;
}