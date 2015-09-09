#include <avr/wdt.h>
#include <Bridge.h>
#include <HttpClient.h>
#include <Servo.h>
#include <Temp.h>

const short TEMP_IN = 5;
const short SERVO_OUT = 9;

String result;
Servo servoCtrl;
double temp;
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
  // Initiate watchdog timer for when arduino crashes
  wdt_enable(WDTO_8S);

  Console.begin();
}

void loop() {

  wdt_reset();
  
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
  postTemp("greensweaterknitting.com", 80, temp);
  wdt_reset();
  postTemp("10.1.10.30", 3000, temp);
  // END: Temp

  wdt_reset();
  Console.flush();
  delay(5000);
}

