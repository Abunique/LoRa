
#include "SPIFFS.h"
 
void setup() 
{
  Serial.begin(115200);
  Serial.println("SPIFFS File Content Tester");
  
  if (!SPIFFS.begin(true)) { Serial.println("An Error has occurred while mounting SPIFFS");return; }

  File root = SPIFFS.open("/");
  File file = root.openNextFile();
 
  while(file)
  {
 
      Serial.print("FILE: ");
      Serial.println(file.name());
      file = root.openNextFile();
  }
}
 
void loop() {}
