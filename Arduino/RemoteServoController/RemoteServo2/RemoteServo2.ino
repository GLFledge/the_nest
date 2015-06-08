#include <SPI.h>
#include <EthernetV2_0.h>
#include <Servo.h>

const short W5200_CS = 10;
const short SDCARD_CS = 4;
const short SERVO_OUT = 8;
const short LED_OUT = 9;
const unsigned long postingInterval = .5 * 1000;  // delay between updates, in milliseconds
const unsigned long blinkInterval = 1 * 1000;
const char server[] = "localhost/LocalSiteRoot";
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

EthernetClient client;
String result;
Servo servoCtrl;
IPAddress localIp;

unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
unsigned long lastBlink = 0;
boolean lastConnected = false;                 // state of the connection last time through the main loop
int pos = 0;

void setup() {
  Serial.begin(9600);
  Serial.print("Setup");
  pinMode(LED_OUT, OUTPUT);
  pinMode(SDCARD_CS, OUTPUT);
  servoCtrl.attach(SERVO_OUT);
  
  Serial.print(".");
  digitalWrite(SDCARD_CS,HIGH);//Deselect the SD card
  Serial.print(".");
  // give the ethernet module time to boot up:
  delay(1000);
  Serial.print(".");
  Ethernet.begin(mac);
  Serial.print(".");
  localIp = Ethernet.localIP();

  Serial.println();
  Serial.print("My IP address: ");
  Serial.println(localIp);
  Serial.print("Update Interval: ");
  Serial.print(postingInterval);
  Serial.print(" ms");
  Serial.println();

  

}
int brightness = 0;
int fadeAmount = 5;

void doblink(){
  // set the brightness of pin 9:
  analogWrite(LED_OUT, brightness);    

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade: 
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount ; 
  }     
  // wait for 30 milliseconds to see the dimming effect    
  delay(30);
  
  if(millis() - lastBlink > blinkInterval){
    
    lastBlink = client.connected();
  }
  
}

void loop() {
  retrieveData();
  doblink();
}

void retrieveData(){
  if (client.available()) {
    result = "";
    while(client.available()){
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

  if (!client.connected() && lastConnected) {
    client.stop();
  }

  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
    httpRequest();
  }
  lastConnected = client.connected();
}

void httpRequest() {
  if (client.connect(server, 80)) {
    Serial.print(".");
    client.println("GET /servo.axd HTTP/1.0");
    client.println("Host: localhost/LocalSiteRoot");
    //client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
    lastConnectionTime = millis();
  } 
  else {
    Serial.println("disconnecting.");
    client.stop();
  }
}








