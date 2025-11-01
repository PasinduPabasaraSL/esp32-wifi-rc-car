#include <WiFi.h>
#include <WiFiUdp.h>
#include <Arduino.h>

// Wi-Fi Access Point credentials
const char *ssid = "RC_Car_AP";
const char *password = "12345678";

WiFiUDP udp;
const uint16_t listenPort = 4210;

// Motor pins
#define LF_IN1 27
#define LF_IN2 26
#define RF_IN3 25
#define RF_IN4 33
#define ENA 14
#define ENB 32

// PWM channels
#define CH_A 0
#define CH_B 1
#define PWM_FREQ 1000
#define PWM_RES 8

int speed = 200;

// Motor setup
void setupMotors()
{
  pinMode(LF_IN1, OUTPUT);
  pinMode(LF_IN2, OUTPUT);
  pinMode(RF_IN3, OUTPUT);
  pinMode(RF_IN4, OUTPUT);

  // PWM setup
  ledcSetup(CH_A, PWM_FREQ, PWM_RES);
  ledcSetup(CH_B, PWM_FREQ, PWM_RES);
  ledcAttachPin(ENA, CH_A);
  ledcAttachPin(ENB, CH_B);
  ledcWrite(CH_A, speed);
  ledcWrite(CH_B, speed);

  stopCar();
}

// Motor control functions
void stopCar()
{
  digitalWrite(LF_IN1, LOW);
  digitalWrite(LF_IN2, LOW);
  digitalWrite(RF_IN3, LOW);
  digitalWrite(RF_IN4, LOW);
}

void forward()
{
  digitalWrite(LF_IN1, HIGH);
  digitalWrite(LF_IN2, LOW);
  digitalWrite(RF_IN3, HIGH);
  digitalWrite(RF_IN4, LOW);
}

void backward()
{
  digitalWrite(LF_IN1, LOW);
  digitalWrite(LF_IN2, HIGH);
  digitalWrite(RF_IN3, LOW);
  digitalWrite(RF_IN4, HIGH);
}

void left()
{
  digitalWrite(LF_IN1, LOW);
  digitalWrite(LF_IN2, HIGH);
  digitalWrite(RF_IN3, HIGH);
  digitalWrite(RF_IN4, LOW);
}

void right()
{
  digitalWrite(LF_IN1, HIGH);
  digitalWrite(LF_IN2, LOW);
  digitalWrite(RF_IN3, LOW);
  digitalWrite(RF_IN4, HIGH);
}

void setup()
{
  Serial.begin(115200);
  delay(50);
  setupMotors();

  WiFi.softAP(ssid, password);
  IPAddress AP_IP = WiFi.softAPIP();
  Serial.println("AP started");
  Serial.print("AP IP: ");
  Serial.println(AP_IP);
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Password: ");
  Serial.println(password);

  // Start UDP listener
  udp.begin(listenPort);
  Serial.print("UDP listening on port....");
  Serial.println(listenPort);
}

void loop()
{
  int packetSize = udp.parsePacket();
  if (packetSize)
  {
    char buf[32];
    int len = udp.read(buf, sizeof(buf) - 1);
    if (len > 0)
    {
      buf[len] = 0;
    }
    Serial.print("Received packet: ");
    Serial.println(buf);

    char cmd = buf[0];
    switch (cmd)
    {
    case 'F':
      //forward();
      Serial.println("=> FORWARD");
      break;
    case 'B':
      //backward();
      Serial.println("=> BACKWARD");
      break;
    case 'L':
      //left();
      Serial.println("=> LEFT");
      break;
    case 'R':
      //right();
      Serial.println("=> RIGHT");
      break;
    case 'S':
      //stopCar();
      Serial.println("=> STOP");
      break;
    default:
      //stopCar();
      Serial.println("=> UNKNOWN -> STOP");
      break;
    }
  }
}  
