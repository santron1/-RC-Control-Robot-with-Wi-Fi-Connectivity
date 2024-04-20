#include <WiFi.h>
#include <AsyncUDP.h>
#include <Arduino.h>
#include "DataParser.h"

const char* ssid = "sanchay123";
const char* password = "396";

DataParser dataParser;

const int udpPort = 12345;

int in1=27;
int in2=26;
int ena=14;
int in3=25;
int in4=33;
int enb=32;

int DataIndex;
int Speed = 50;
int Right_speed = 0;
int Left_speed = 0;

AsyncUDP udp;

void setup() {
  Serial.begin(115200);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enb, OUTPUT);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  
  if(udp.listen(udpPort)) {
    Serial.print("UDP Listening on IP: ");
    Serial.println(WiFi.localIP());
    udp.onPacket( [](AsyncUDPPacket packet) {
      String IncomingData = (char*)packet.data();
      dataParser.parseData(IncomingData, ',');
      Speed = (dataParser.getField(1)).toInt();
      Left_speed = Speed;
      Right_speed = Speed;
    });
  }
}

void loop() {     
  if(dataParser.getField(0) == "f")
 {
  forward(Left_speed, Right_speed);
  Serial.println("fwd");
 }
 if(dataParser.getField(0) == "b")
 {
  backward(Left_speed, Right_speed);
  Serial.println("bck");
 }
 if(dataParser.getField(0) == "l")
 {
  left(Left_speed, Right_speed);
  Serial.println("left");
 }
 if(dataParser.getField(0) == "r")
 {
  right(Left_speed, Right_speed);
  Serial.println("right");
 }
 if(dataParser.getField(0) == "s")
 {
  Stop();
}
}

void forward(int left_speed, int right_speed) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(ena, left_speed);
  analogWrite(enb, right_speed);
}

void backward(int left_speed, int right_speed) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(ena, left_speed);
  analogWrite(enb, right_speed);
}

void left(int left_speed, int right_speed) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(ena, left_speed);
  analogWrite(enb, right_speed);
}

void right(int left_speed, int right_speed) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(ena, left_speed);
  analogWrite(enb, right_speed);
}

void motor_speed(int Right_Speed,int Left_Speed)
{
Left_speed = Left_Speed;
Right_speed = Right_Speed;
}

void Stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(ena, 0);
  analogWrite(enb, 0);
}
