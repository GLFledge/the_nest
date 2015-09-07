#include <Bridge.h>
#include <YunClient.h>
#include <HttpClient.h>
#include <Servo.h>
#include <Temp.h>

const short TEMP_IN = 5;
const short SERVO_OUT = 9;

String result;
Servo servoCtrl;
YunClient postClient;
double temp;
String postStr;

int pos = 0;

void setup() {
  servoCtrl.attach(SERVO_OUT);
  
  // Bridge takes about two seconds to start up
  // it can be helpful to use the on-board LED
  // as an indicator for when it has initialized
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Bridge.begin();
  digitalWrite(13, LOW);

  Console.begin();

  //while (!Console); // wait for a serial connection
}

void loop() {

  // BEGIN: Servo
  // Initialize the client library
  HttpClient getClient;

  // Make a HTTP request:
  getClient.get("http://17steps.com/servo.axd");

  // if there are incoming bytes available
  // from the server, read them and print them:
  if (getClient.available()){
    result = "";
    while (getClient.available()) {
      char c = getClient.read();
      result += c;
    }
    String posString = result.substring(result.indexOf("<val>") + 5, result.indexOf("</val>"));
    int newPos = posString.toInt();
    if(newPos != pos){
      pos = newPos;
      Console.println();
      Console.println(pos);
      servoCtrl.write(pos);
    }
  }
  // END: Servo

  // BEGIN: Temp
  temp = readTemp(TEMP_IN);
  Console.println(temp);

  if (postClient.connect("greensweaterknitting.com", 80)) {
    Console.println("connected to POST server");
    postStr = "{\"temp\": " + String(temp) + ", \"arbitrary-key\": " + "123454321098767890}";
    postClient.println("POST /temperature HTTP/1.1");
    postClient.println("Host: greensweaterknitting.com");
    postClient.println("User-Agent: Arduino/1.0");
    postClient.println("Connection: close");
    postClient.println("Content-Type: application/json");
    postClient.print("Content-Length: ");
    postClient.println(postStr.length());
    postClient.println();
    postClient.println(postStr);
  } else {
    Console.println("connection to POST server failed");
  }
  // END: Temp

  Console.flush();
  delay(5000);
}

