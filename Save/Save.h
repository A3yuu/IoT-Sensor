
#define EEPROM_SIZE 256

struct SaveData {
  char uri[256];
};
bool saveSetup();
void dataSave(struct SaveData saveData);
void dataGet(struct SaveData &saveData);
