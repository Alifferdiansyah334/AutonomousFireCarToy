#include <ESP32Servo.h>  // Sertakan Library Servo

// Definisikan pin yang terhubung ke kabel sinyal servo
const int pinServo = 17;

// Definisikan pin yang terhubung ke kabel buzzer

// Definisikan pin yang terhubung ke sensor api
const int pinSensorApi = 39;  //13

// Definisikan pin yang terhubung ke sensor MQ7
const int pinSensorMQ7 = 21;

// Definisikan pin Onboard LED ESP32
const int onBoardLed = 2;

// Definisikan pin untuk Motor Driver 1 (Roda Sisi Kiri)
const int in1_driver1 = 23;  // Positif untuk roda kiri belakang
const int in2_driver1 = 19;  // Negatif untuk roda kiri belakang
const int in3_driver1 = 18;  // Negatif untuk roda kiri depan
const int in4_driver1 = 5;   // Positif untuk roda kiri depan

// Definisikan pin untuk Motor Driver 2 (Roda Sisi Kanan)
const int in1_driver2 = 22;  // Positif untuk roda kanan belakang
const int in2_driver2 = 15;  // Negatif untuk roda kanan belakang
const int in3_driver2 = 4;   // Negatif untuk roda kanan depan
const int in4_driver2 = 16;  // Positif untuk roda kanan depan

// Definisikan pin untuk Sensor Ultrasonik
const int pinTrig1 = 32;  // Sensor depan kanan 32
const int pinEcho1 = 33;
const int pinTrig2 = 26;  // Sensor depan tengah
const int pinEcho2 = 25;
const int pinTrig3 = 14;  // Sensor depan kiri
const int pinEcho3 = 27;

// Definisikan pin untuk Relay
const int pinRelay = 13;  // Pin kontrol relay //12

// Definisikan batas jarak untuk deteksi rintangan
const long batasDepan = 30; // cm
const long batasSamping = 30;  // cm

// Buat objek Servo
Servo servoSaya;

// Definisikan rentang sudut servo
int sudutServo = 0;
int langkahServo = 5;  // Langkah inkremental untuk gerakan servo
unsigned long waktuSebelumnyaServo = 0;
const unsigned long intervalServo = 15;  // Waktu antara pembaruan servo dalam milidetik

void setup() {

  // Pasang servo ke pin yang telah ditentukan
  servoSaya.attach(pinServo);

  // Inisialisasi pin sensor api sebagai input
  pinMode(pinSensorApi, INPUT);

  // Inisialisasi pin MQ7 sebagai input
  pinMode(pinSensorMQ7, INPUT);

  // Inisialisasi pin MQ7 sebagai input
  pinMode(onBoardLed, OUTPUT);

  // Setel pin sebagai output atau input untuk motor driver
  pinMode(in1_driver1, OUTPUT);
  pinMode(in2_driver1, OUTPUT);
  pinMode(in3_driver1, OUTPUT);
  pinMode(in4_driver1, OUTPUT);

  pinMode(in1_driver2, OUTPUT);
  pinMode(in2_driver2, OUTPUT);
  pinMode(in3_driver2, OUTPUT);
  pinMode(in4_driver2, OUTPUT);

  pinMode(pinTrig1, OUTPUT);
  pinMode(pinEcho1, INPUT);
  pinMode(pinTrig2, OUTPUT);
  pinMode(pinEcho2, INPUT);
  pinMode(pinTrig3, OUTPUT);
  pinMode(pinEcho3, INPUT);

  pinMode(pinRelay, OUTPUT); // Setel pin relay sebagai output

  digitalWrite(pinRelay, LOW);

  // Inisialisasi semua pin ke LOW
  berhentiRobot();  // Pastikan mobil berhenti awalnya

  // Mulai komunikasi serial untuk debugging
  Serial.begin(115200);
}

void loop() {
  int nilaiSensorApi = digitalRead(pinSensorApi);
  Serial.print("Nilai Sensor Api: ");
  Serial.println(nilaiSensorApi);

  if (nilaiSensorApi == LOW) {
    // Api terdeteksi (LOW berarti api terdeteksi)
    Serial.println("Api terdeteksi!");
    digitalWrite(pinRelay, HIGH);
    berhentiRobot();
    delay(3000);
  } else {
    // Tidak ada api terdeteksi
    Serial.println("Tidak ada api terdeteksi.");
    digitalWrite(pinRelay, LOW);
    deteksiRintangan();  // Fungsi untuk mendeteksi rintangan dan mengontrol gerakan mobil
    perbaruiServo();  // Fungsi untuk mengontrol servo
    deteksiGas(); // Fungsi untuk mendeteksi Gas Dengan MQ7
  }
}

// Fungsi untuk pergerakkan Servo
void perbaruiServo() {
  unsigned long waktuSekarang = millis();
  if (waktuSekarang - waktuSebelumnyaServo >= intervalServo) {
    waktuSebelumnyaServo = waktuSekarang;
    sudutServo += langkahServo;
    if (sudutServo >= 180 || sudutServo <= 0) {
      langkahServo = -langkahServo;  // Balikkan arah
    }
    servoSaya.write(sudutServo);
    Serial.print("Sudut Servo: ");
    Serial.println(sudutServo);
  }
}

// Fungsi untuk deteksi Gas Dengan MQ7
void deteksiGas() {
  int nilaiSensorGas = digitalRead(pinSensorMQ7);
  Serial.print("Nilai Sensor MQ7: ");
  Serial.println(nilaiSensorGas);

  if (nilaiSensorGas == LOW) {
    // Gas terdeteksi (Berarti Gas terdeteksi)
    Serial.println("Gas terdeteksi!");
    digitalWrite(onBoardLed, HIGH);
    delay(1000);  // menyalakan onBoardLed pada ESP32
    digitalWrite(onBoardLed, LOW);
    delay(1000);  // menyalakan onBoardLed pada ESP32
  } else {
    // Tidak ada Gas terdeteksi
    digitalWrite(onBoardLed, LOW);
    Serial.println("Tidak ada gas terdeteksi.");
  }
}

// Fungsi untuk mendeteksi rintangan dan kontrol gerakan mobil
void deteksiRintangan() {
  long jarak1 = ukurJarak(pinTrig1, pinEcho1); // Sensor depan kanan
  long jarak2 = ukurJarak(pinTrig2, pinEcho2); // Sensor depan tengah
  long jarak3 = ukurJarak(pinTrig3, pinEcho3); // Sensor depan kiri

  String arah;

  if (jarak1 <= 10 && jarak2 <= 10 && jarak3 <= 10) {
    arah = "Mundur";
    mundur();
    delay(250);
    berhentiRobot();
    delay(500);
  } else if (jarak1 <= 5 && jarak2 <= 5 && jarak3 <= 5) {
    arah = "Mundur";
    mundur();
    delay(500);
    berhentiRobot();
    delay(500);
  } else if (jarak2 >= batasDepan && jarak1 >= batasSamping && jarak3 >= batasSamping) {
    arah = "Maju";
    maju();  
  } else {
    berhentiRobot();
    delay(500);
    mundur();
    delay(200);
    berhentiRobot();
    delay(500);
    
    if (jarak1 > jarak3) {
      arah = "Kanan";
      putarKanan();  // Logika dibalik
    } else {
      arah = "Kiri";
      putarKiri(); // Logika dibalik
    }
    
    delay(300);  // 100 ms delay per putaran
    berhentiRobot();
    delay(500);
  }

  // Cetak arah dan jarak dalam satu baris
  Serial.print("Arah: ");
  Serial.print(arah);
  Serial.print(" | Jarak 1 (Kanan): ");
  Serial.print(jarak1);
  Serial.print(" cm | Jarak 2 (Tengah): ");
  Serial.print(jarak2);
  Serial.print(" cm | Jarak 3 (Kiri): ");
  Serial.print(jarak3);
  Serial.println(" cm");

  // Opsional, delay singkat untuk menghindari loop berlebihan
  delay(100);  // Sesuaikan delay ini sesuai kebutuhan
}

// Fungsi untuk memutar mobil ke arah kanan
void putarKiri() {
  digitalWrite(in1_driver1, HIGH);  // Roda kiri depan
  digitalWrite(in2_driver1, LOW);   // Roda kiri depan
  digitalWrite(in3_driver1, LOW);   // Roda kiri belakang
  digitalWrite(in4_driver1, HIGH);  // Roda kiri belakang

  digitalWrite(in1_driver2, LOW);   // Roda kanan depan
  digitalWrite(in2_driver2, HIGH);  // Roda kanan depan
  digitalWrite(in3_driver2, HIGH);  // Roda kanan belakang
  digitalWrite(in4_driver2, LOW);   // Roda kanan belakang
}

// Fungsi untuk memutar mobil ke arah kiri
void putarKanan() {
  digitalWrite(in1_driver1, LOW);   // Roda kiri depan
  digitalWrite(in2_driver1, HIGH);  // Roda kiri depan
  digitalWrite(in3_driver1, HIGH);  // Roda kiri belakang
  digitalWrite(in4_driver1, LOW);   // Roda kiri belakang

  digitalWrite(in1_driver2, HIGH);  // Roda kanan depan
  digitalWrite(in2_driver2, LOW);   // Roda kanan depan
  digitalWrite(in3_driver2, LOW);   // Roda kanan belakang
  digitalWrite(in4_driver2, HIGH);  // Roda kanan belakang
}

// Fungsi untuk menggerakkan mobil maju
void maju() { 
  digitalWrite(in1_driver1, LOW);  // Roda kiri depan
  digitalWrite(in2_driver1, HIGH); // Roda kiri depan
  digitalWrite(in3_driver1, HIGH); // Roda kiri belakang
  digitalWrite(in4_driver1, LOW);  // Roda kiri belakang

  digitalWrite(in1_driver2, LOW);  // Roda kanan depan
  digitalWrite(in2_driver2, HIGH); // Roda kanan depan
  digitalWrite(in3_driver2, HIGH); // Roda kanan belakang
  digitalWrite(in4_driver2, LOW);  // Roda kanan belakang
}

// Fungsi untuk memundurkan mobil
void mundur() {
  digitalWrite(in1_driver1, HIGH); // Roda kiri depan
  digitalWrite(in2_driver1, LOW);  // Roda kiri depan
  digitalWrite(in3_driver1, LOW);  // Roda kiri belakang
  digitalWrite(in4_driver1, HIGH); // Roda kiri belakang

  digitalWrite(in1_driver2, HIGH); // Roda kanan depan
  digitalWrite(in2_driver2, LOW);  // Roda kanan depan
  digitalWrite(in3_driver2, LOW);  // Roda kanan belakang
  digitalWrite(in4_driver2, HIGH); // Roda kanan belakang
}

// Fungsi untuk menghentikan mobil
void berhentiRobot() {
  digitalWrite(in1_driver1, LOW);
  digitalWrite(in2_driver1, LOW);
  digitalWrite(in3_driver1, LOW);
  digitalWrite(in4_driver1, LOW);

  digitalWrite(in1_driver2, LOW);
  digitalWrite(in2_driver2, LOW);
  digitalWrite(in3_driver2, LOW);
  digitalWrite(in4_driver2, LOW);
}

// Fungsi untuk mengukur jarak dengan sensor ultrasonik
long ukurJarak(int pinTrig, int pinEcho) {
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);
  
  long durasi = pulseIn(pinEcho, HIGH);
  long jarak = durasi * 0.0344 / 2;
  return jarak;
}