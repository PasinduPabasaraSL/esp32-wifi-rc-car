#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi Access Point credentials
const char* ssid = "RC_Car_AP";
const char* password = "12345678";

WebServer server(80);

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
void setupMotors() {
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
void stopCar() {
  digitalWrite(LF_IN1, LOW); digitalWrite(LF_IN2, LOW);
  digitalWrite(RF_IN3, LOW); digitalWrite(RF_IN4, LOW);
}

void forward() {
  digitalWrite(LF_IN1, HIGH); digitalWrite(LF_IN2, LOW);
  digitalWrite(RF_IN3, HIGH); digitalWrite(RF_IN4, LOW);
}

void backward() {
  digitalWrite(LF_IN1, LOW); digitalWrite(LF_IN2, HIGH);
  digitalWrite(RF_IN3, LOW); digitalWrite(RF_IN4, HIGH);
}

void left() {
  digitalWrite(LF_IN1, LOW); digitalWrite(LF_IN2, HIGH);
  digitalWrite(RF_IN3, HIGH); digitalWrite(RF_IN4, LOW);
}

void right() {
  digitalWrite(LF_IN1, HIGH); digitalWrite(LF_IN2, LOW);
  digitalWrite(RF_IN3, LOW); digitalWrite(RF_IN4, HIGH);
}

// Web page handler
void handleRoot() {
  String html = R"rawliteral(
    <html>
    <head>
      <title>ESP32 RC Car</title>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <style>
        body { background:#111; color:white; text-align:center; font-family:Arial; }
        button { width:100px; height:60px; margin:10px; font-size:20px; border:none; border-radius:10px; background:#673AB7; color:white; }
        button:active { background:#9575CD; }
      </style>
    </head>
    <body>
      <h2>ESP32 Wi-Fi RC Car</h2>
      <div>
        <button onclick="fetch('/forward')">Forward</button><br>
        <button onclick="fetch('/left')">Left</button>
        <button onclick="fetch('/stop')">Stop</button>
        <button onclick="fetch('/right')">Right</button><br>
        <button onclick="fetch('/backward')">Backward</button>
      </div>
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  setupMotors();

  WiFi.softAP(ssid, password);
  Serial.println("Connect to Wi-Fi AP:");
  Serial.println(ssid);
  Serial.println("Password: 12345678");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/forward", [](){ forward(); server.send(200,"text/plain","F"); });
  server.on("/backward", [](){ backward(); server.send(200,"text/plain","B"); });
  server.on("/left", [](){ left(); server.send(200,"text/plain","L"); });
  server.on("/right", [](){ right(); server.send(200,"text/plain","R"); });
  server.on("/stop", [](){ stopCar(); server.send(200,"text/plain","S"); });

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}
