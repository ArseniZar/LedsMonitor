#ifndef EEPROMSETUP_H
#define EEPROMSETUP_H

#include <EEPROM.h>
#include <Arduino.h>

class EEPROMSetup {
public:
    EEPROMSetup(int size = 512);

    void begin();

    void writeString(const String& str, int startAddr = 0);

    String readString(int startAddr = 0);

private:
    int _size;
};

#endif // EEPROMSETUP_H
