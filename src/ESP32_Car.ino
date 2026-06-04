Contents:

WiFi.softAP()
Web server
Joystick handling
Motor control
Ultrasonic functions
Obstacle detection
Buzzer logic

This is the actual project code.




#include <WiFi.h>
#include <WebServer.h>

// ---------------- WIFI ----------------
const char* ssid = "ESP32-Car";
const char* password = "12345678";

WebServer server(80);

// ---------------- MOTOR PINS ----------------
#define IN1 27
#define IN2 26
#define IN3 18
#define IN4 19

// ---------------- ULTRASONIC SENSOR ----------------
#define TRIG_PIN 23
#define ECHO_PIN 32

// ---------------- BUZZER ----------------
#define BUZZER 25
#define BUZZER_CHANNEL 0
#define BUZZER_FREQ 2000
#define BUZZER_RESOLUTION 8

// ---------------- OBSTACLE DISTANCE ----------------
int getDistanceCM() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout

  int distance = duration * 0.034 / 2;

  return distance;
}

// ---------------- MOTOR FUNCTION ----------------
void setMotor(int in1, int in2, int speedVal) {

  if (speedVal > 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
  else if (speedVal < 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
  else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
}

// ---------------- DRIVE ----------------
void drive(int x, int y) {

  int throttle = map(y, -100, 100, -255, 255);
  int steer = map(x, -100, 100, -255, 255);

  int speedL = throttle + steer;
  int speedR = throttle - steer;

  if (throttle > 0) {
    speedL = constrain(speedL, 0, 255);
    speedR = constrain(speedR, 0, 255);
  }

  if (throttle < 0) {
    speedL = constrain(speedL, -255, 0);
    speedR = constrain(speedR, -255, 0);
  }

  if (throttle == 0) {
    speedL = steer;
    speedR = -steer;
  }

  setMotor(IN1, IN2, speedR);
  setMotor(IN3, IN4, speedL);
}

// ---------------- STOP ----------------
void stopCar() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ---------------- WEB PAGE ----------------
const char PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body { text-align:center; font-family:Arial; background:#f2f2f2; }

#joystick {
  width:200px;
  height:200px;
  background:#ccc;
  border-radius:50%;
  margin:60px auto;
  position:relative;
  touch-action:none;
}

#knob {
  width:80px;
  height:80px;
  background:#333;
  border-radius:50%;
  position:absolute;
  top:60px;
  left:60px;
}
</style>
</head>

<body>

<h2>ESP32 RC Car</h2>

<div id="joystick">
  <div id="knob"></div>
</div>

<script>
let joy = document.getElementById("joystick");
let knob = document.getElementById("knob");

let active = false;

function send(x,y){
  fetch(`/drive?x=${x}&y=${y}`);
}

joy.addEventListener("touchstart", ()=>active=true);

joy.addEventListener("touchend", ()=>{
  active=false;
  knob.style.left="60px";
  knob.style.top="60px";
  send(0,0);
});

joy.addEventListener("touchmove", e=>{
  if(!active) return;

  let rect = joy.getBoundingClientRect();

  let x = e.touches[0].clientX - rect.left - 100;
  let y = e.touches[0].clientY - rect.top - 100;

  x = Math.max(-100, Math.min(100, x));
  y = Math.max(-100, Math.min(100, -y));

  knob.style.left = (x + 60) + "px";
  knob.style.top  = (-y + 60) + "px";

  send(x,y);
});
</script>

</body>
</html>
)rawliteral";

// ---------------- SETUP ----------------
void setup() {

  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BUZZER, OUTPUT);

  // PWM setup for passive buzzer
  ledcSetup(BUZZER_CHANNEL, BUZZER_FREQ, BUZZER_RESOLUTION);
  ledcAttachPin(BUZZER, BUZZER_CHANNEL);

  stopCar();

  WiFi.softAP(ssid, password);

  server.on("/", []() {
    server.send_P(200, "text/html", PAGE);
  });

  server.on("/drive", []() {
    int x = server.arg("x").toInt();
    int y = server.arg("y").toInt();
    drive(x, y);
    server.send(200, "text/plain", "OK");
  });

  server.begin();

  Serial.println("Car Ready!");
  Serial.println(WiFi.softAPIP());
}

// ---------------- LOOP ----------------
void loop() {

  server.handleClient();

  int distance = getDistanceCM();

  Serial.println(distance);

  if (distance > 0 && distance <= 15) {

    stopCar();

    // passive buzzer beep
    ledcWriteTone(BUZZER_CHANNEL, 2000);
    delay(100);
    ledcWriteTone(BUZZER_CHANNEL, 0);
    delay(100);

  }
  else {
    ledcWriteTone(BUZZER_CHANNEL, 0);
  }
}
