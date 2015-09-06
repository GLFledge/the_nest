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