/*
  Yún HTTP Client, for temperature sensor and servo
  
  analog pins for temp sensor are A0 through A5

 This example for the Arduino Yún shows how create a basic
 HTTP client that connects to the internet and downloads
 content. In this case, you'll connect to the Arduino
 website and download a version of the logo as ASCII text.

 created by Tom igoe
 May 2013

 This example code is in the public domain.

 http://arduino.cc/en/Tutorial/HttpClient

 */

#include <Bridge.h>
#include <HttpClient.h>
#include <Servo.h>

const short SERVO_OUT = 9;

String result;
Servo servoCtrl;

int pos = 0;

int sensorPin = 3; //attached to analog pin 3
int analogVal = 0; //varible to store the reading of A3
unsigned long previousMillis = 0; //to store the time since last update
const long intervalServo = 500; //interval for servo reading
const long intervalSensor = 20; //interval for sensor

void setup() {
  servoCtrl.attach(SERVO_OUT);
  
  // Bridge takes about two seconds to start up
  // it can be helpful to use the on-board LED
  // as an indicator for when it has initialized

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

  Serial.begin(9600);

  while (!Serial); // wait for a serial connection
}

void loop() {
   //check the current time since the program started running
  unsigned long currentMillis = millis(); 
 
  // Initialize the client library
  HttpClient client;

  // Make a HTTP request:
  client.get("http://17steps.com/servo.axd");

  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()){
    result = "";
    while (client.available()) {
      char c = client.read();
      result += c;
    }
    //if the interval in which the servo can be read has not 
    //timed out, the servo will be read, and updated.
    if (currentMillis - previousMillis >= intervalServo){
    String posString = result.substring(result.indexOf("<val>") + 5, result.indexOf("</val>"));
    int newPos = posString.toInt();
    if(newPos != pos){
      pos = newPos;
      Serial.println();
      Serial.println(pos);
      servoCtrl.write(pos);
      }
    }
    //when the interval for the servo expires, these instructions
    //will excute so long as the interval for the sensor is
    //not timed out.
    if (currentMillis - previousMillis >= intervalSensor){
      analogVal = analogRead(sensorPin);
      
      //presumes a 5V power source for ananlog;
      //use 3300 in place of 5000 for a 3.3V power source
      float voltage = analogVal * (5000/1024);
      
      float celsius = (voltage - 500)/10;
      float farhenheit = ((celsius * 9)/5)+32;
      
      Serial.println();
      Serial.println(farhenheit);

    }
  }
  Serial.flush();
}


