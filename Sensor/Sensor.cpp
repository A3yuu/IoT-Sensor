#include "Sensor.h"

#include <Wire.h>

unsigned char i2c_address = 0x62;

//SCD4xLib
#define CRC8_POLYNOMIAL 0x31
#define CRC8_INIT 0xFF
uint8_t sensirion_common_generate_crc(const uint8_t* data, uint16_t count) {
  uint16_t current_byte;
  uint8_t crc = CRC8_INIT;
  uint8_t crc_bit;
  /* calculates 8-Bit checksum with given polynomial */
  for (current_byte = 0; current_byte < count; ++current_byte) {
    crc ^= (data[current_byte]);
    for (crc_bit = 8; crc_bit > 0; --crc_bit) {
      if (crc & 0x80)
        crc = (crc << 1) ^ CRC8_POLYNOMIAL;
      else
        crc = (crc << 1);
    }
  }
  return crc;
}

int scd40_start_periodic_measurement () {
  Wire.beginTransmission(i2c_address);
  Wire.write(0x21);
  Wire.write(0xb1);
  return -Wire.endTransmission();
}
int scd4x_start_low_power_periodic_measurement() {
  Wire.beginTransmission(i2c_address);
  Wire.write(0x21);
  Wire.write(0xac);
  return -Wire.endTransmission();
}

int scd4x_stop_periodic_measurement() {
  Wire.beginTransmission(i2c_address);
  Wire.write(0x3f);
  Wire.write(0x86);
  return -Wire.endTransmission();
}

int scd4x_read_measurement(Measurements& measurements) {
  Wire.beginTransmission(i2c_address);
  Wire.write(0xec);
  Wire.write(0x05);
  int ret = Wire.endTransmission(false);
  if (ret)return -ret;
  ret = Wire.requestFrom(i2c_address, 9u);
  if (ret != 9)return ret;
  uint8_t data[9];
  for (int i = 0; i < ret; i++) {
    data[i] = Wire.read();
  }
  uint16_t data16[3];
  for (int i = 0; i < 3; i++) {
    uint8_t crc = sensirion_common_generate_crc(data + i * 3, 2);
    if (crc != data[i * 3 + 2])return -5;
    data16[i] = ((uint16_t)data[i * 3])<<8 | data[i * 3 + 1];
  }
  measurements.co2 = data16[0];
  measurements.temperature = -45 + 175 * ((float)data16[1]) / 65536;
  measurements.humidity = 100 * ((float)data16[2]) / 65536;
  return ret;
}

int scd4x_set_temperature_offset(float offset) {
  uint16_t _offset = offset * 65536 / 175;
  uint8_t data[2];
  data[0] = _offset >> 8;
  data[1] = _offset & 0xff;
  uint8_t crc = sensirion_common_generate_crc(data, 2);
  Wire.beginTransmission(i2c_address);
  Wire.write(0x24);
  Wire.write(0x1d);
  Wire.write(data[0]);
  Wire.write(data[1]);
  Wire.write(crc);
  return -Wire.endTransmission();
}

int scd4x_Setup()
{
  int ret;
  ret = scd4x_stop_periodic_measurement();
  delay(500);
  ret += 10 * scd4x_set_temperature_offset(0);
  ret += 100 * scd40_start_periodic_measurement();
  return ret;
}

int sensorSetup()
{
  Wire.begin(25, 21);
  return scd4x_Setup();
}

int sensorGet(Measurements& measurements)
{
  int ret = scd4x_read_measurement(measurements);
  if (ret <= 0) {
    ret += 10 * scd4x_Setup();
  }
  return ret;
}
