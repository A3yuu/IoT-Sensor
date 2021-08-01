
#include "save.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("start");
  if (!saveSetup()) {
    Serial.println("error");
  }
  SaveData saveData;
  dataGet(saveData);
  Serial.println(saveData.uri);
  Serial.println("------------------------------------\n");
  strcpy(saveData.uri, "aaa");
  Serial.println(saveData.uri);
  dataSave(saveData);
}

void loop()
{
}
