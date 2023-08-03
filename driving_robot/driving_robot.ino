#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <NewPing.h>
#include <Servo.h>

#define TRIGGER_PIN 5
#define ECHO_PIN 6
#define MAX_DISTANCE 20  // 2 metry 200

#define STOP_DISTANCE 50  // 2 metry 200

#define SERVO_PIN 10
#define SERVO_TOP_DEG 85
#define SERVO_TOP_DEG2 100
#define SERVO_LEFT_DEG 180
#define SERVO_RIGHT_DEG 0

#define DISABLE_MOTORS 0

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motorLeft = AFMS.getMotor(2);
Adafruit_DCMotor *motorRight = AFMS.getMotor(3);

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

Servo servo;
bool canToUp = true;

void setup() {
  AFMS.begin();

  motorLeft->setSpeed(0);
  motorRight->setSpeed(0);
  motorLeft->run(FORWARD);
  motorRight->run(FORWARD);

  servo.attach(SERVO_PIN);
  Serial.begin(9600);

  servo.write(SERVO_TOP_DEG);
}

void loop() {
  if (getDistance() >= STOP_DISTANCE && canToUp) {
    toTopMotors();
    delay(100);
  } else {
    if (canToUp == false) {
      canToUp = true;
    }
    stopMotors();
    servo.write(SERVO_LEFT_DEG);
    delay(800);
    if (getDistance() >= STOP_DISTANCE) {
      toLeftMotors();
      servo.write(SERVO_TOP_DEG);
      delay(1000);
    } else {
      servo.write(SERVO_RIGHT_DEG);
      delay(800);
      if (getDistance() > STOP_DISTANCE) {
        servo.write(SERVO_TOP_DEG2);
        toRightMotors();
        delay(1000);
      } else {
        toDownMotors();
        servo.write(SERVO_TOP_DEG2);
        delay(1000);
        canToUp = false;
      }
    }
  }
}
int getDistance() {
  unsigned int timeS = sonar.ping();
  unsigned int distance = (timeS / US_ROUNDTRIP_CM);
  Serial.println(distance);
  distance = distance ? distance : 999;
  return distance;
}

void toTopMotors() {
  if (! DISABLE_MOTORS) {
    motorLeft->setSpeed(250);
    motorRight->setSpeed(240);
    motorLeft->run(FORWARD);
    motorRight->run(FORWARD);
  }
}

void toLeftMotors() {
  if (! DISABLE_MOTORS) {
    motorLeft->setSpeed(150);
    motorRight->setSpeed(150);
    motorLeft->run(BACKWARD);
    motorRight->run(FORWARD);
  }
}

void toRightMotors() {
  if (! DISABLE_MOTORS) {
    motorLeft->setSpeed(150);
    motorRight->setSpeed(150);
    motorLeft->run(FORWARD);
    motorRight->run(BACKWARD);
  }
}

void toDownMotors() {
  if (! DISABLE_MOTORS) {
    motorLeft->setSpeed(150);
    motorRight->setSpeed(150);
    motorLeft->run(BACKWARD);
    motorRight->run(BACKWARD);
  }
}

void stopMotors() {
  motorLeft->setSpeed(0);
  motorRight->setSpeed(0);
}
