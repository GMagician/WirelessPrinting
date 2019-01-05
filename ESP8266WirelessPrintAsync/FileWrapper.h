#pragma once

#define FS_NO_GLOBALS //allow spiffs to coexist with SD card, define BEFORE including FS.h

#include <Arduino.h>
#include <SD.h>
#include <FS.h>

class FileWrapper {
  private:
    static bool hasSD;
    File sdFile;
    fs::File fsFile;
    
  public:
    ~FileWrapper() {
      close();
    }

    static inline void begin(bool hasSD) {
      FileWrapper::hasSD = hasSD;
    }

    inline operator bool() {
      return sdFile || fsFile;
    }

    static FileWrapper open(String path, char *openMode);
    bool available();
    String readStringUntil(char eol);
    void write(const uint8_t *buf, size_t len);
    void close();
    void remove(String filename);
};

extern FileWrapper fileWrapper;
