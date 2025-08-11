#include "Eeprom.h"

Eeprom &Eeprom::init(int size)
{
    static Eeprom instance(size);
    return instance;
}

Eeprom::Eeprom(int size) : size(size) { EEPROM.begin(size); }



void Eeprom::writeString(const String &str, int startAddr)
{
    int len = str.length();
    if (len > size - startAddr - 1)
    {
        len = size - startAddr - 1;
    }

    for (int i = 0; i < len; i++)
    {
        EEPROM.write(startAddr + i, str[i]);
    }
    EEPROM.write(startAddr + len, 0);
    EEPROM.commit();
}

String Eeprom::readString(int startAddr)
{
    int maxLen = size - startAddr;
    char *buf = new char[maxLen + 1];

    for (int i = 0; i < maxLen; i++)
    {
        buf[i] = EEPROM.read(startAddr + i);
        if (buf[i] == 0)
        {
            break;
        }
    }
    String result(buf);
    delete[] buf;
    return result;
}
