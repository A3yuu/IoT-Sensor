#include "save.h"

#include <EEPROM.h>

bool saveSetup() {
  return EEPROM.begin(EEPROM_SIZE);
}
void dataSave(struct SaveData saveData) {
  EEPROM.put(0, saveData);
  EEPROM.commit();
}
void dataGet(struct SaveData &saveData) {
  EEPROM.get( 0, saveData );
}
