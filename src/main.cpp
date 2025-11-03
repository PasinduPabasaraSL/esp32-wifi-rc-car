// espnow_rx.ino
#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

// Motor pins (adjust to your wiring)
#define LF_IN1 27
#define LF_IN2 26
#define RF_IN3 25
#define RF_IN4 33
#define ENA 14
#define ENB 32

// PWM channels and config
#define CH_A 0
#define CH_B 1
#define PWM_FREQ 1000
#define PWM_RES 8

int speed = 200; // 0-255 for 8-bit resolution

void stopCar();

void setupMotors() {
  pinMode(LF_IN1, OUTPUT);
  pinMode(LF_IN2, OUTPUT);
  pinMode(RF_IN3, OUTPUT);
  pinMode(RF_IN4, OUTPUT);

  ledcSetup(CH_A, PWM_FREQ, PWM_RES);
  ledcSetup(CH_B, PWM_FREQ, PWM_RES);
  ledcAttachPin(ENA, CH_A);
  ledcAttachPin(ENB, CH_B);

  ledcWrite(CH_A, 0);
  ledcWrite(CH_B, 0);

  stopCar();
}

void stopCar() {
  digitalWrite(LF_IN1, LOW);
  digitalWrite(LF_IN2, LOW);
  digitalWrite(RF_IN3, LOW);
  digitalWrite(RF_IN4, LOW);
  ledcWrite(CH_A, 0);
  ledcWrite(CH_B, 0);
}

void forward() {
  ledcWrite(CH_A, speed);
  ledcWrite(CH_B, speed);
  digitalWrite(LF_IN1, HIGH);
  digitalWrite(LF_IN2, LOW);
  digitalWrite(RF_IN3, HIGH);
  digitalWrite(RF_IN4, LOW);
}

void backward() {
  ledcWrite(CH_A, speed);
  ledcWrite(CH_B, speed);
  digitalWrite(LF_IN1, LOW);
  digitalWrite(LF_IN2, HIGH);
  digitalWrite(RF_IN3, LOW);
  digitalWrite(RF_IN4, HIGH);
}

void left() {
  ledcWrite(CH_A, speed);
  ledcWrite(CH_B, speed);
  digitalWrite(LF_IN1, LOW);
  digitalWrite(LF_IN2, HIGH);
  digitalWrite(RF_IN3, HIGH);
  digitalWrite(RF_IN4, LOW);
}

void right() {
  ledcWrite(CH_A, speed);
  ledcWrite(CH_B, speed);
  digitalWrite(LF_IN1, HIGH);
  digitalWrite(LF_IN2, LOW);
  digitalWrite(RF_IN3, LOW);
  digitalWrite(RF_IN4, HIGH);
}

// ESP-NOW receive callback
void onDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  if (len < 1) return;
  char cmd = (char)incomingData[0];

  Serial.print("From ");
  for (int i = 0; i < 6; ++i) {
    if (i) Serial.print(":");
    Serial.print(mac[i], HEX);
  }
  Serial.print(" Cmd: ");
  Serial.println(cmd);

  switch (cmd) {
    case 'F': forward(); Serial.println("=> FORWARD"); break;
    case 'B': backward(); Serial.println("=> BACKWARD"); break;
    case 'L': left(); Serial.println("=> LEFT"); break;
    case 'R': right(); Serial.println("=> RIGHT"); break;
    case 'S': stopCar(); Serial.println("=> STOP"); break;
    default: stopCar(); Serial.print("=> UNKNOWN "); Serial.println(cmd); break;
  }
}

void setupEspNow() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_register_recv_cb(onDataRecv);
  Serial.println("ESP-NOW ready");
}

void setup() {
  Serial.begin(115200);
  delay(50);

  setupMotors();

  setupEspNow();

  // print this board MAC so you can confirm
  uint8_t mac[6];
  esp_read_mac(mac, ESP_MAC_WIFI_STA);
  Serial.print("This device STA MAC: ");
  for (int i = 0; i < 6; ++i) {
    if (i) Serial.print(":");
    Serial.print(mac[i], HEX);
  }
  Serial.println();
}

void loop() {
  // nothing to poll. actions come via callback
  delay(10);
}
