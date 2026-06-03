#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "Config.h"
#include <Servo.h>
#include "WebPage.h"

#define STEER_LEFT   120
#define STEER_CENTER 71
#define STEER_RIGHT  22

Servo steeringServo;
ESP8266WebServer server(80);

const char *ssid = "RaceCar_AP";
const char *password = "12345678"; // Password must be at least 8 characters

int motorSpeed = 255; // Max speed for ESP8266 PWM (0-1023)

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

void handleRoot() {
  server.send(200, "text/html", WEBPAGE_HTML);
}

void handleControl() {
  if (server.hasArg("motor")) {
    int pwm = server.arg("motor").toInt();
    Serial.print("Motor speed: ");
    Serial.println(pwm);
    if (pwm > 0) {
      motorForward(pwm);
    } else if (pwm < 0) {
      motorBackward(abs(pwm));
    } else {
      motorStop();
    }
  }
  
  if (server.hasArg("steer")) {
    int angle = server.arg("steer").toInt();
    angle = constrain(angle, min(STEER_LEFT, STEER_RIGHT), max(STEER_LEFT, STEER_RIGHT));
    Serial.print("Steering angle: ");
    Serial.println(angle);
    steeringServo.write(angle);
  }
  
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);

  // Initialize Servo
  steeringServo.attach(SERVO_STEERING_PIN, 500, 2500);
  steeringServo.write(STEER_CENTER);

  // Initialize Motor Pins
  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  analogWriteRange(255); // Set PWM range to 0-255 (ESP8266 defaults to 1023)
  motorStop();

  // Setup WiFi Access Point
  Serial.println("\nConfiguring Access Point...");
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Setup mDNS - access via http://racecar.local
  if (MDNS.begin("racecar")) {
    Serial.println("mDNS started: http://racecar.local");
  } else {
    Serial.println("mDNS failed to start");
  }

  // Setup Web Server Endpoints
  server.on("/", handleRoot);
  server.on("/control", handleControl);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  MDNS.update();
  server.handleClient();
}
