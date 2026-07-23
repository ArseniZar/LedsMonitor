#include <Arduino.h>
#include "App.h"

void setup() {
    Serial.begin(115200);
    delay(1000); 
    
    App& app = App::init(); 
    app.begin();
}

void loop() {
    App::init().update();
}