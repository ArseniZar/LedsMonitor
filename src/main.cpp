#include <Arduino.h>
#include "App.h"

void setup() {
    Serial.begin(115200);
    delay(1000); 
    
    App& app = App::init(); 
    app.begin();
    app.start();
}

void loop() {
    App::init().update();
}