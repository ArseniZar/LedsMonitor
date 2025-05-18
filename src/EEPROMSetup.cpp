#include "EEPROMSetup.h"

EEPROMSetup::EEPROMSetup(int size)
    : _size(size)
{}

void EEPROMSetup::begin() {
    EEPROM.begin(_size);
}

void EEPROMSetup::writeString(const String& str, int startAddr) {
    int len = str.length();
    if (len > _size - startAddr - 1) {
        len = _size - startAddr - 1; 
    }

    for (int i = 0; i < len; i++) {
        EEPROM.write(startAddr + i, str[i]);
    }
    EEPROM.write(startAddr + len, 0); 
    EEPROM.commit();
}

String EEPROMSetup::readString(int startAddr) {
    int maxLen = _size - startAddr;
    char* buf = new char[maxLen + 1];

    for (int i = 0; i < maxLen; i++) {
        buf[i] = EEPROM.read(startAddr + i);
        if (buf[i] == 0) {
            buf[i] = '\0';
            break;
        }
    }
    String result(buf);
    delete[] buf;
    return result;
}
