#pragma once
#include "Storage.h"

template <typename T>
Storage<T>::Storage(const char *path, const uint8_t key) : file(&LittleFS, path, key, &savedData, sizeof(savedData)), loaded(false) {}

template <typename T>
void Storage<T>::begin() {
    static bool fsStarted = false; 
    if (!fsStarted) {
        LittleFS.begin();
        fsStarted = true;
    }
}

template <typename T>
T Storage<T>::readData() {
    FDstat_t st = file.read();
    if (st == FD_READ || st == FD_ADD) {
        loaded = true;   
    } else {
        loaded = false;  
    }
    return savedData;
}

template <typename T>
void Storage<T>::writeData(const T &data)
{
    savedData = data;
    file.updateNow();
}

template <typename T>
const T& Storage<T>::getData() const {
    return savedData;
}

template <typename T>
bool Storage<T>::isLoaded() const {
    return loaded;
}
