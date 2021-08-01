
struct Measurements {
  int co2;
  float temperature;
  float humidity;
};

int sensorSetup();
int sensorGet(Measurements& measurements);
