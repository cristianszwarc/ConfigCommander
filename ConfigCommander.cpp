/*
  ConfigCommander.cpp - Functions to save data to EEPROM.
  Created by Cristian D. Szwarc, January 2017.
  Released into the public domain.
*/
#include "ConfigCommander.h"

// sets the size of a config field
void ConfigCommander::setSize(short int slot, int8_t size)
{
  this->configSizes[slot] = size;
}

// get the size of a config field
int8_t ConfigCommander::getSize(short int slot)
{
  return this->configSizes[slot];
}

// fill a piece of memory with null values and returns the next address
int ConfigCommander::cleanEEPROM(unsigned int address, int8_t size) {
  while (size>0) {
    size--;
    address++;
    EEPROM.write(address, '\0');
  }

  // return next address
  return address++;
}

// fill each config slot with null elements
bool ConfigCommander::initialize(bool forced) {
  unsigned int address = 0;
  unsigned int i;

  if (EEPROM.read(address) != 123 || forced) {
    EEPROM.write(address, 123);  // indicate this board was initialized

    // format each slot with null elements
    for (i = 0; i < sizeof(this->configSizes); i++){
      address = this->cleanEEPROM(address, this->configSizes[i]);
    }

    return true;

  }

  return false;
}

// return the address of a slot in the EEPROM
int ConfigCommander::addressOfConfigSlot(unsigned int slot) {
  unsigned int address = 1; // skip first byte, indicates is initialized
  unsigned int i = 0;

  if (slot >= sizeof(this->configSizes)) return 0;

  while (i < slot) {
    address += this->configSizes[i];
    i++;
  }

  return address;
}

// read a field from the memory
bool ConfigCommander::readConfig(int8_t slot, char *output) {
  unsigned int address = this->addressOfConfigSlot(slot);
  int8_t i = 0;
  char value;

  if (!address) return false; // incorrect slot

  while (i < this->configSizes[slot]) { // size of the slot
    value = EEPROM.read(address);
    if (value == '\0') {  // terminator found before end of size, break, terminator is added anyway
      break;
    }
    output[i] = value;
    address++;
    i++;
  }

  output[i] = '\0'; // app

  return true;
}

// save data into a field
bool ConfigCommander::saveConfig(int8_t slot, char *input) {
  unsigned int address = this->addressOfConfigSlot(slot);
  int8_t i = 0;

  if (!address) return false; // incorrect slot

  while (i < this->configSizes[slot]) { // size of the slot
    EEPROM.write(address, input[i]);
    i++;
    address++;
  }

  return true;
}

// read an ip (human string) and return a 4 bytes array
bool ConfigCommander::readIP(int8_t slot, byte *output) {
  char ipStr[16]; // max config field plus terminator
  if (this->readConfig(slot, ipStr)) {
    this->parseBytes(ipStr, '.', output, 4, 10);
    return true;
  }

  return false;
}

// read a mac address (human string) and return a 6 bytes array
bool ConfigCommander::readMAC(int8_t slot, byte *output) {
  char ipStr[18]; // max config field plus terminator
  if (this->readConfig(slot, ipStr)) {
    this->parseBytes(ipStr, '.', output, 6, 16);
    return true;
  }

  return false;
}

void ConfigCommander::parseBytes(const char* str, char sep, byte* bytes, int maxBytes, int base) {
    for (int i = 0; i < maxBytes; i++) {
        bytes[i] = strtoul(str, NULL, base);  // Convert byte
        str = strchr(str, sep);               // Find next separator
        if (str == NULL || *str == '\0') {
            break;                            // No more separators, exit
        }
        str++;                                // Point to next character after separator
    }
}
