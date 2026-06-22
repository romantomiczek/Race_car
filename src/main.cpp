#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include "Config.h"
#include <Servo.h>
#include <WebSocketsServer.h>
#include "WebPage.h"

/* ========== EEPROM Settings ========== */
#define EEPROM_SIZE 128
#define SETTINGS_MAGIC 0xAC

struct CarSettings
{
  uint8_t magic;
  uint8_t servoCenter;
  uint8_t servoLeftLimit;
  uint8_t servoRightLimit;
  uint8_t maxMotorSpeed;
  char ssid[33];
  char password[65];
};

// Runtime settings (loaded from EEPROM or defaults)
int steerCenter = 71;
int steerLeft = 120;
int steerRight = 22;
int maxMotorSpeed = 255;
String currentSsid = "RaceCar_AP";
String currentPassword = "12345678";

bool pendingWifiRestart = false;

Servo steeringServo;
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

/* ========== EEPROM Functions ========== */
void loadSettings()
{
  EEPROM.begin(EEPROM_SIZE);
  CarSettings s = {};
  EEPROM.get(0, s);
  if (s.magic == SETTINGS_MAGIC)
  {
    steerCenter = constrain(s.servoCenter, 0, 180);
    steerLeft = constrain(s.servoLeftLimit, 0, 180);
    steerRight = constrain(s.servoRightLimit, 0, 180);
    maxMotorSpeed = constrain(s.maxMotorSpeed, 0, 255);
    // Safety check for strings to prevent loading garbage
    s.ssid[sizeof(s.ssid) - 1] = '\0';
    s.password[sizeof(s.password) - 1] = '\0';
    if (strlen(s.ssid) > 0)
    {
      currentSsid = String(s.ssid);
    }
    if (strlen(s.password) >= 8)
    {
      currentPassword = String(s.password);
    }
    Serial.println("Settings loaded from EEPROM");
  }
  else
  {
    Serial.println("No saved settings, using defaults");
  }
}

void saveSettings()
{
  CarSettings s = {};
  s.magic = SETTINGS_MAGIC;
  s.servoCenter = (uint8_t)steerCenter;
  s.servoLeftLimit = (uint8_t)steerLeft;
  s.servoRightLimit = (uint8_t)steerRight;
  s.maxMotorSpeed = (uint8_t)maxMotorSpeed;
  strncpy(s.ssid, currentSsid.c_str(), sizeof(s.ssid) - 1);
  strncpy(s.password, currentPassword.c_str(), sizeof(s.password) - 1);
  EEPROM.put(0, s);
  EEPROM.commit();
  Serial.println("Settings saved to EEPROM");
}

/* ========== Motor Control ========== */
void motorForward(int pwmValue)
{
  analogWrite(MOTOR1_PIN1, abs(pwmValue));
  digitalWrite(MOTOR1_PIN2, LOW);
}

void motorBackward(int pwmValue)
{
  digitalWrite(MOTOR1_PIN1, LOW);
  analogWrite(MOTOR1_PIN2, abs(pwmValue));
}

void motorStop()
{
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, LOW);
}

/* ========== HTTP Handlers ========== */
void handleRoot()
{
  server.send(200, "text/html", WEBPAGE_HTML);
}

void handleControl()
{
  if (server.hasArg("motor"))
  {
    int pwm = server.arg("motor").toInt();
    pwm = constrain(pwm, -maxMotorSpeed, maxMotorSpeed);
    Serial.print("Motor: ");
    Serial.println(pwm);
    if (pwm > 0)
    {
      motorForward(pwm);
    }
    else if (pwm < 0)
    {
      motorBackward(abs(pwm));
    }
    else
    {
      motorStop();
    }
  }

  if (server.hasArg("steer"))
  {
    int angle = server.arg("steer").toInt();
    angle = constrain(angle, min(steerLeft, steerRight), max(steerLeft, steerRight));
    Serial.print("Steer: ");
    Serial.println(angle);
    steeringServo.write(angle);
  }

  server.send(200, "text/plain", "OK");
}

void sendSettingsJson()
{
  String json = "{\"servoCenter\":";
  json += String(steerCenter);
  json += ",\"servoLeftLimit\":";
  json += String(steerLeft);
  json += ",\"servoRightLimit\":";
  json += String(steerRight);
  json += ",\"maxMotorSpeed\":";
  json += String(maxMotorSpeed);
  json += ",\"ssid\":\"";
  json += currentSsid;
  json += "\",\"password\":\"";
  json += currentPassword;
  json += "\"}";
  server.send(200, "application/json", json);
}

void handleGetSettings()
{
  sendSettingsJson();
}

void handlePostSettings()
{
  bool wifiChanged = false;
  if (server.hasArg("servoCenter"))
    steerCenter = constrain(server.arg("servoCenter").toInt(), 0, 180);
  if (server.hasArg("servoLeftLimit"))
    steerLeft = constrain(server.arg("servoLeftLimit").toInt(), 0, 180);
  if (server.hasArg("servoRightLimit"))
    steerRight = constrain(server.arg("servoRightLimit").toInt(), 0, 180);
  if (server.hasArg("maxMotorSpeed"))
    maxMotorSpeed = constrain(server.arg("maxMotorSpeed").toInt(), 0, 255);

  if (server.hasArg("ssid") && server.hasArg("password"))
  {
    String newSsid = server.arg("ssid");
    String newPwd = server.arg("password");
    if (newSsid.length() > 0 && newPwd.length() >= 8)
    {
      if (newSsid != currentSsid || newPwd != currentPassword)
      {
        currentSsid = newSsid;
        currentPassword = newPwd;
        wifiChanged = true;
      }
    }
  }

  saveSettings();

  // Move servo to new center as confirmation
  steeringServo.write(steerCenter);

  Serial.print("Settings updated - Center:");
  Serial.print(steerCenter);
  Serial.print(" Left:");
  Serial.print(steerLeft);
  Serial.print(" Right:");
  Serial.print(steerRight);
  Serial.print(" MaxSpeed:");
  Serial.println(maxMotorSpeed);

  sendSettingsJson();

  if (wifiChanged)
  {
    pendingWifiRestart = true;
  }
}

/* ========== WebSocket Handler ========== */
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_DISCONNECTED:
    Serial.printf("[WS] Client %u disconnected\n", num);
    break;
  case WStype_CONNECTED:
    Serial.printf("[WS] Client %u connected\n", num);
    break;
  case WStype_TEXT:
  {
    if (length >= 4 && payload[0] == 'C' && payload[1] == ':')
    {
      int pwm = 0, angle = 0;
      if (sscanf((const char *)&payload[2], "%d,%d", &pwm, &angle) == 2)
      {
        pwm = constrain(pwm, -maxMotorSpeed, maxMotorSpeed);
        if (pwm > 0)
        {
          motorForward(pwm);
        }
        else if (pwm < 0)
        {
          motorBackward(-pwm);
        }
        else
        {
          motorStop();
        }
        angle = constrain(angle, min(steerLeft, steerRight), max(steerLeft, steerRight));
        steeringServo.write(angle);
        Serial.printf("[WS] Control - Motor: %d, Steer: %d\n", pwm, angle);
      }
    }
    break;
  }
  default:
    break;
  }
}

/* ========== Setup ========== */
void setup()
{
  Serial.begin(115200);

  // Load settings from EEPROM
  loadSettings();

  // Initialize Servo
  steeringServo.attach(SERVO_STEERING_PIN, 500, 2500);
  steeringServo.write(steerCenter);

  // Initialize Motor Pins
  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  analogWriteRange(255); // Set PWM range to 0-255 (ESP8266 defaults to 1023)
  motorStop();

  // Setup WiFi Access Point
  Serial.println("\nConfiguring Access Point...");
  WiFi.softAP(currentSsid.c_str(), currentPassword.c_str());
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Setup mDNS - access via http://racecar.local
  if (MDNS.begin("racecar"))
  {
    Serial.println("mDNS started: http://racecar.local");
  }
  else
  {
    Serial.println("mDNS failed to start");
  }

  // Setup Web Server Endpoints
  server.on("/", handleRoot);
  server.on("/control", handleControl);
  server.on("/settings", HTTP_GET, handleGetSettings);
  server.on("/settings", HTTP_POST, handlePostSettings);
  server.begin();
  Serial.println("HTTP server started");

  // Setup WebSocket Server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket server started on port 81");
}

/* ========== Loop ========== */
void loop()
{
  MDNS.update();
  server.handleClient();
  webSocket.loop();

  if (pendingWifiRestart)
  {
    delay(500); // Wait briefly to allow the HTTP response to be sent
    Serial.println("Restarting AP with new credentials...");
    WiFi.softAPdisconnect(true);
    delay(100);
    WiFi.softAP(currentSsid.c_str(), currentPassword.c_str());
    pendingWifiRestart = false;
  }
}
