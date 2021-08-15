
#include <Wire.h>

struct Measurements {
  int co2;
  float temperature;
  float humidity;
};

int sensorSetup(float offset = 0);
int sensorGet(Measurements& measurements);

//SCD41
int sensorSingleStart(float offset = 0);
int sensorSingleGet(Measurements& measurements);
