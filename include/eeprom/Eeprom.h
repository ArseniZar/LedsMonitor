#pragma once
#ifndef EEPROMHELPER_H
#define EEPROMHELPER_H

#include <EEPROM.h>
#include <Arduino.h>

class Eeprom
{
public:
    String readString(int startAddr = 0);
    void writeString(const String &str, int startAddr = 0);
    static Eeprom &init(int size);

private:
    int size;
    Eeprom(int size);
    Eeprom(bool debugMode = true);
    Eeprom(const Eeprom &) = default;
    Eeprom(Eeprom &&) = default;
    Eeprom &operator=(const Eeprom &) = default;
    Eeprom &operator=(Eeprom &&) = default;
};

#endif // EEPROMHELPER_H
