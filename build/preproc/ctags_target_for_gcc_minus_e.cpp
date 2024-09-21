# 1 "C:\\Users\\aliff\\Downloads\\damkars\\damkars.ino"
// Definisikan pin kontrol relay
const int pinBuzzer = 21;

void setup() {
  pinMode(pinBuzzer, 0x03);

  digitalWrite(pinBuzzer, 0x0);
}

void loop() {

  digitalWrite(pinBuzzer, 0x1);
  delay(1000);
  digitalWrite(pinBuzzer, 0x0);
  delay(1000);
}
