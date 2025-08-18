#include "Arduino.h"
#include "App.h"

App &app = App::init();
void setup()
{
  Serial.begin(115200);
  app.begin();
}

void loop()
{
  app.update();
}
