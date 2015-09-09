#include <Temp.h>

float readTemp(int analogPin) {

  int a;          // analog read value
  int B=3975;     // B value of the thermistor
  float temperature;
  float resistance;
  
  a = analogRead(analogPin);
  resistance = (float)(1023-a)*10000/a; //get the resistance of the sensor;
  temperature = 1/(log(resistance/10000)/B+1/298.15)-273.15; //convert to temperature via datasheet
  temperature = temperature*1.8 + 32; // Fahrenheit
  return temperature;
}

void postTemp(const char* addr, int port, int temp) {

	YunClient postClient;
	String postStr;

  if (postClient.connect(addr, port)) {
    Console.println("connected to POST server");
    postStr = "{\"temp\": " + String(temp) + ", \"arbitrary-key\": " + "123454321098767890}";
    postClient.println("POST /temperature HTTP/1.1");
    postClient.println("Host: " + String(addr));
    postClient.println("User-Agent: Arduino/1.0");
    postClient.println("Connection: close");
    postClient.println("Content-Type: application/json");
    postClient.print("Content-Length: ");
    postClient.println(postStr.length());
    postClient.println();
    postClient.println(postStr);
    postClient.flush();
    postClient.stop();
  } else {
    Console.println("connection to POST server failed");
  }
}