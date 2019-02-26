/*
 * Project: LED LAMP
 * Author: Keisuke Tomizawa
 */

#define LED_PIN 3
#define ANALOG_WRITE_MAX 255

int freq = 25000;
float duty_cycle = 0.50f;
String inString = "";    // string to hold input

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(LED_PIN, OUTPUT);
  analogWriteFrequency(freq);
  analogWrite(LED_PIN, (int)(ANALOG_WRITE_MAX * duty_cycle));
}

void loop() {
  // Read serial input:
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
    }
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n') {
      Serial.print("Value:");
      duty_cycle = inString.toFloat() / 100;
      Serial.println(duty_cycle);
      analogWrite(LED_PIN, (int)(ANALOG_WRITE_MAX * duty_cycle));
      inString = "";
    }
  }
}
