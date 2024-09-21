// Definisikan pin kontrol relay
const int pinBuzzer = 21;

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
