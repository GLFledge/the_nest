#include <Bridge.h>
#include <YunClient.h>
#include <math.h>

float readTemp(int analogPin);
void postTemp(const char* addr, int port, int temp);
