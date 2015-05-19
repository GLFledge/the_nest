/*
 This example shows how to serve data from an analog input  
 via the Arduino Yún's built-in webserver using the Bridge library.

 The circuit:
 * TMP36 temperature sensor on analog pin A1
 * SD card attached to SD card slot of the Arduino Yún

 Prepare your SD card with an empty folder in the SD root
 named "arduino" and a subfolder of that named "www".
 This will ensure that the Yún will create a link
 to the SD to the "/mnt/sd" path.

 In this sketch folder is a basic webpage and uses AngularJS.
 When you upload your sketch, the associated files
 will be placed in the /arduino/www/TempSensorServoWeb folder on your SD card.

 You can then go to http://arduino.local/sd/TempSensorServoWeb
 to see the output of this sketch.
*/

#include <Bridge.h>
#include <HttpClient.h>
#include <Servo.h>
#include <YunServer.h>
#include <YunClient.h>


const short SERVO_OUT = 9;
String result;
Servo servoCtrl;
int pos = 0;

// Listen on default port 5555, the webserver on the Yun
// will forward there all the HTTP requests for us.
YunServer server;
String startString;
long hits = 0;


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
  
    // using A0 and A2 as vcc (power) and gnd (ground) for the TMP36 sensor:
  pinMode(A0, OUTPUT);
  pinMode(A2, OUTPUT);
  digitalWrite(A0, HIGH);
  digitalWrite(A2, LOW);

  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();

  // get the time that this sketch started:
  Process startTime;
  startTime.runShellCommand("date");
  while(startTime.available()) {
    char s = startTime.read();
    startString += s;
  }

  while (!Serial); // wait for a serial connection
}

void loop() {
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
    String posString = result.substring(result.indexOf("<val>") + 5, result.indexOf("</val>"));
    int newPos = posString.toInt();
    if(newPos != pos){
      pos = newPos;
      Serial.println();
      Serial.println(pos);
      servoCtrl.write(pos);
    }
  }
  Serial.flush();
  
    // Get clients coming from server
  YunClient clientLocal = server.accept();

  // There is a new  local client?
  if (clientLocal) {
    // read the command
    String command = clientLocal.readString();
    command.trim();        //kill whitespace
    Serial.println(command);
    
    // is "temperature" command?
    if (command == "temperature") {
      // get the time from the server:
      Process time;
      time.runShellCommand("date");
      String timeString = "";
      while(time.available()) {
        char t = time.read();
        timeString += t;
      }
      
      Serial.println(timeString);
      int sensorValue = analogRead(A1);
      // the reading from analog pin A1 is converted to millivolts
      // then converted to celsius (temperatureC)
      // and finally fahrenheit (temperature)
      
      float voltage = sensorValue *  (5000/ 1024);
      float temperatureC = (voltage - 500)/10;
      float temperature = temperatureC * (9/5) + 32;
      
      // print the temperature:
      clientLocal.print("Current time on the Yun: ");
      clientLocal.println(timeString);
      clientLocal.print("<br>Current temperature: ");
      clientLocal.print(temperature);
      clientLocal.print(" degrees F");
      
    }

    // Close connection and free resources.
    clientLocal.stop();
    
  }
  delay(5000);
}


