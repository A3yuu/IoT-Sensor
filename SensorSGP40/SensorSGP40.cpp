#include "Sensor.h"
#include "sensirion_voc_algorithm.h"

unsigned char sgp40_i2c_address = 0x59;

RTC_DATA_ATTR VocAlgorithmParams vocAlgorithmParameters;

//SGP4xLib
uint8_t CalcCrc(uint8_t data[2]) {
  uint8_t crc = 0xFF;
  for (int i = 0; i < 2; i++) {
    crc ^= data[i];
    for (uint8_t bit = 8; bit > 0; --bit) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ 0x31u;
      } else {
        crc = (crc << 1);
      }
    }
  }
  return crc;
}

int sgp40_heater_off () {
  Wire.beginTransmission(sgp40_i2c_address);
  Wire.write(0x36);
  Wire.write(0x15);
  return -Wire.endTransmission();
}

int sgp40_measure_raw (uint16_t& measurement) {
  Wire.beginTransmission(sgp40_i2c_address);
  Wire.write(0x26);
  Wire.write(0x0f);
  int ret = Wire.endTransmission();
  if (ret)return -ret;
  delay(30);
  ret = Wire.requestFrom(sgp40_i2c_address, 3u);
  if (ret != 3)return ret;
  uint8_t data[3];
  for (int i = 0; i < ret; i++) {
    data[i] = Wire.read();
  }
  uint8_t crc = CalcCrc(data);
  if (crc != data[2])return -5;
  measurement = ((uint16_t)data[0]) << 8 | data[1];
  return ret;
}

int sgp40_measure_raw (double rh, double temp, uint16_t& measurement) {
  Wire.beginTransmission(sgp40_i2c_address);
  Wire.write(0x26);
  Wire.write(0x0f);
  uint16_t rh_ = rh / 100 * 65535;
  uint16_t temp_ = (temp + 45) / 175 * 65535;
  uint8_t send[3];
  send[0] = rh_ >> 8;
  send[1] = rh_ & 0x00ff;
  send[2] = CalcCrc(send);
  Wire.write(send, 3);
  send[0] = temp_ >> 8;
  send[1] = temp_ & 0x00ff;
  send[2] = CalcCrc(send);
  Wire.write(send, 3);
  int ret = Wire.endTransmission();
  if (ret)return -ret;
  delay(30);
  ret = Wire.requestFrom(sgp40_i2c_address, 3u);
  if (ret != 3)return ret;
  uint8_t data[3];
  for (int i = 0; i < ret; i++) {
    data[i] = Wire.read();
  }
  uint8_t crc = CalcCrc(data);
  if (crc != data[2])return -5;
  measurement = ((uint16_t)data[0]) << 8 | data[1];
  return ret;
}

int sgp40_measure_test (uint16_t& result) {
  Wire.beginTransmission(sgp40_i2c_address);
  Wire.write(0x28);
  Wire.write(0x0e);
  int ret = Wire.endTransmission();
  if (ret)return -ret;
  delay(250);
  ret = Wire.requestFrom(sgp40_i2c_address, 3u);
  if (ret != 3)return ret;
  uint8_t data[3];
  for (int i = 0; i < ret; i++) {
    data[i] = Wire.read();
  }
  uint8_t crc = CalcCrc(data);
  if (crc != data[2])return -5;
  result = ((uint16_t)data[0]) << 8 | data[1];
  return ret;
}

int sensorSetupSGP40()
{
  VocAlgorithm_init(&vocAlgorithmParameters);
  return 0;
}

int sensorGetSGP40(double rh, double temp, int32_t& vocIndex)
{
  uint16_t measurement;
  int ret = sgp40_measure_raw(rh, temp, measurement);
  if (ret == 3)VocAlgorithm_process(&vocAlgorithmParameters, measurement, &vocIndex);
  return ret;
}
