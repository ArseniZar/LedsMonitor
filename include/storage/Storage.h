#pragma once
#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include <FileData.h>
#include <LittleFS.h>

template <typename T>
class Storage
{

public:
    void begin();
    T readData();
    void writeData(const T &data);
    const T& getData() const;
    bool isLoaded() const;

    Storage() = delete;
    Storage(const char *path, const uint8_t key);

private:
    T savedData;
    FileData file;
    bool loaded;
};

#include "Storage.tpp"

#endif // STORAGE_H