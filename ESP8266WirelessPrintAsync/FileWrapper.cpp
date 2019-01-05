#include "FileWrapper.h"

FileWrapper fileWrapper;

bool FileWrapper::hasSD = false;

FileWrapper FileWrapper::open(String path, char *openMode) {
  FileWrapper fileWrapper;

  if (openMode != NULL && openMode[0] != '\0') {
    if (FileWrapper::hasSD) {
      bool isWriteMode = (openMode[0] == 'w' ? true : false);

      const char *sdPath = path.c_str();
      fileWrapper.sdFile = SD.open(sdPath, isWriteMode ? FILE_WRITE : FILE_READ);
    }
    else
      fileWrapper.fsFile = SPIFFS.open(path, openMode);
  }

  return fileWrapper ? fileWrapper : FileWrapper();
}

bool FileWrapper::available() {
  return sdFile ? sdFile.available() : (fsFile ? fsFile.available() : false);
  }

String FileWrapper::readStringUntil(char eol) {
  return sdFile ? sdFile.readStringUntil(eol) : (fsFile ? fsFile.readStringUntil(eol) : "");
  }

void FileWrapper::write(const uint8_t *buf, size_t len) {
  if (sdFile) {
    for (size_t i = 0; i < len; i++)
      sdFile.write(buf[i]);
  }
  else if (fsFile) {
    ESP.wdtDisable();
    fsFile.write(buf, len);
    ESP.wdtEnable(10);
  }
}

void FileWrapper::close() {
  if (sdFile) {
    sdFile.close();
    sdFile = File();
  }
  else if (fsFile) {
    fsFile.close();
    fsFile = fs::File();
  }
}

void FileWrapper::remove(String filename) {
  if (FileWrapper::hasSD)
    SD.remove(filename);
  else
    SPIFFS.remove(filename);
}
