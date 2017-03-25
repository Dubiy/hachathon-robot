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

#define FIREBASE_HOST "iottestcheb.firebaseio.com"

#define FIREBASE_AUTH "D9b1YvaQh3YFo4AAbT6BNAcRir7PUmWFIakZBr3v"
#define WIFI_SSID "ekreative"
#define WIFI_PASSWORD "yabloka346"

const int MOTOR_L1 = 5; // D1
const int MOTOR_L2 = 4; // D2
const int MOTOR_R1 = 0; // D3
const int MOTOR_R2 = 2; // D4
const String ROBOT_ID = "";
const bool DEBUG = true;

void setup() {

  Serial.begin(115200);
  Serial.print("start");

  pinMode(MOTOR_L1, OUTPUT);
  pinMode(MOTOR_L2, OUTPUT);
  pinMode(MOTOR_R1, OUTPUT);
  pinMode(MOTOR_R2, OUTPUT);

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
  // Firebase.set("pushbutton", 0);
  // Firebase.set("sunlight", 0);
  // Firebase.set("redlight", 0);
  // Firebase.set("cooldown", 0);
  // Firebase.set("brrr", 0);

}



int motor_l = 0;
int motor_r = 0;

void loop() {
  Serial.println("loop ");

  motor_l = Firebase.getInt(ROBOT_ID + "motor_l");
  motor_r = Firebase.getInt(ROBOT_ID + "motor_r");

  setMotorState(motor_l, motor_r);

 // int newButton = digitalRead(buttonPin);
 //  if (newButton != button) {
 //    button = newButton;
 //    Firebase.setInt("pushbutton", button);
 //  }
 //  float newLight = analogRead(lightSensorPin);
 //  if (abs(newLight - light) > 100) {
 //    light = newLight;
 //    Firebase.setFloat("sunlight", light);
 //  }

  delay(200);

}

void setMotorState(int motor_l, int motor_r) {
  int motor_l1 = 0,
      motor_l2 = 0,
      motor_r1 = 0,
      motor_r2 = 0;

  if (motor_l > 0) {
    motor_l1 = motor_l;
  } else {
    motor_l2 = motor_l;
  }

  if (motor_r > 0) {
    motor_r1 = motor_r;
  } else {
    motor_r2 = motor_l;
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
