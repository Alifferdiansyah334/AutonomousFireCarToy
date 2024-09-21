#include <Arduino.h>
#line 1 "C:\\Users\\aliff\\Downloads\\damkars\\damkars.ino"
// Definisikan pin kontrol relay
const int pinBuzzer = 21;

#line 4 "C:\\Users\\aliff\\Downloads\\damkars\\damkars.ino"
void setup();
#line 10 "C:\\Users\\aliff\\Downloads\\damkars\\damkars.ino"
void loop();
#line 4 "C:\\Users\\aliff\\Downloads\\damkars\\damkars.ino"
void setup() {
  pinMode(pinBuzzer, OUTPUT);

  digitalWrite(pinBuzzer, LOW);
}

void loop() {

  digitalWrite(pinBuzzer, HIGH);
  delay(1000);
  digitalWrite(pinBuzzer, LOW);
  delay(1000);
}

