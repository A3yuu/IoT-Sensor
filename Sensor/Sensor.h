
struct Measurements {
  int co2;
  float temperature;
  float humidity;
};

int sensorSetup(int offset = 0);
int sensorGet(Measurements& measurements);

//SCD41
int sensorSingleStart(int offset = 0);
int sensorSingleGet(Measurements& measurements);
