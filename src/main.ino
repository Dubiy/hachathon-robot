//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseRoom_ESP8266 is a sample that demo using multiple sensors
// and actuactor with the FirebaseArduino library.

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <FirebaseArduino.h>
#include <Servo.h>

#include "melodies.h"
extern "C" {
  #include "user_interface.h"
}

#define FIREBASE_HOST "iottestcheb.firebaseio.com"

#define FIREBASE_AUTH "D9b1YvaQh3YFo4AAbT6BNAcRir7PUmWFIakZBr3v"
#define WIFI_SSID "ekreative"
#define WIFI_PASSWORD "yabloka346"

// http://www.14core.com/wp-content/uploads/2015/06/Node-MCU-Pin-Out-Diagram1.png
const int MOTOR_L1 = 5; // D1
const int MOTOR_L2 = 4; // D2
const int MOTOR_R1 = 0; // D3
const int MOTOR_R2 = 2; // D4
const int BUZZER = 14; // D5
const int SERVO =  15;  // D8



// const String ROBOT_ID = "cheb_";
// const String ROBOT_ID = "kirill_";
const String ROBOT_ID = "gary_";
const bool DEBUG = true;

Servo servo;
int pos1 = 170;
int currentMelody = 0;



os_timer_t myTimer;

int buzz_note = 1;
int buzz_melody_num = 0;
bool buzz_playing = false;
bool buzz_delay = false;
unsigned long buzz_next_fire = 0;

void timerCallback(void *pArg) {
  if (!buzz_playing) {return;}

  unsigned long currentMillis = millis();
  if (buzz_next_fire < currentMillis) {
    if (buzz_note > buzz_melodies[buzz_melody_num * 2][0]) {
      buzz_playing = false;
      Serial.println("finish");
      return;
    }

    if (!buzz_delay) { //play note
      int frequency = buzz_melodies[buzz_melody_num * 2][buzz_note];
      if (frequency) {
        tone(BUZZER, frequency);
      }
      // note duration
      buzz_next_fire = currentMillis + 1000/buzz_melodies[buzz_melody_num * 2 + 1][buzz_note];
      buzz_delay = true;
    } else {
      noTone(BUZZER);
      buzz_next_fire = currentMillis + 1000/buzz_melodies[buzz_melody_num * 2 + 1][buzz_note] * 1.2;
      buzz_delay = false;
      buzz_note++;
    }
  }
}

void timerInit(void) {
  // http://www.switchdoc.com/2015/10/iot-esp8266-timer-tutorial-arduino-ide/
  os_timer_setfn(&myTimer, timerCallback, NULL);
  os_timer_arm(&myTimer, 10, true);
}

void setup() {
  Serial.begin(115200);
  Serial.print("start");

  pinMode(MOTOR_L1, OUTPUT);
  pinMode(MOTOR_L2, OUTPUT);
  pinMode(MOTOR_R1, OUTPUT);
  pinMode(MOTOR_R2, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  servo.attach(SERVO);


  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  play(0);
  timerInit();
}



int motor_l = 0;
int motor_r = 0;
int servo_pos = 0;

void loop() {
  Serial.print(".");

  motor_l = Firebase.getInt(ROBOT_ID + "motor_l");
  motor_r = Firebase.getInt(ROBOT_ID + "motor_r");
  servo_pos = Firebase.getInt(ROBOT_ID + "servo");
  setMotorState(motor_l, motor_r);
  servo.write(servo_pos);

  // delay(50);
}

bool play(int melody_num) {
  if (buzz_playing) {
    return false;
  }

  // buzz_melody_num = melody_num;
  buzz_melody_num = currentMelody++;
  if (currentMelody > 30) {
    currentMelody = 0;
  }

  buzz_note = 1;
  buzz_playing = true;
  buzz_next_fire = 0;
  return true;
}

void setMotorState(int motor_l, int motor_r) {
  int motor_l1 = 0,
      motor_l2 = 0,
      motor_r1 = 0,
      motor_r2 = 0;

  if (motor_l > 0) {
    motor_l1 = motor_l;
  } else {
    motor_l2 = abs(motor_l);
  }

  if (motor_r > 0) {
    motor_r1 = motor_r;
  } else {
    motor_r2 = abs(motor_r);
  }

  if (DEBUG) {
    Serial.print("motor_l1 = ");
    Serial.print(motor_l1);
    Serial.print(". motor_l2 = ");
    Serial.print(motor_l2);
    Serial.print(". motor_r1 = ");
    Serial.print(motor_r1);
    Serial.print(". motor_r2 = ");
    Serial.println(motor_r2);
  }

  analogWrite(MOTOR_L1, motor_l1);
  analogWrite(MOTOR_L2, motor_l2);
  analogWrite(MOTOR_R1, motor_r1);
  analogWrite(MOTOR_R2, motor_r2);
}
