/*
  ConfigCommander.h - Functions to save data to EEPROM.
  Created by Cristian D. Szwarc, January 2017.
  Released into the public domain.
*/
#ifndef CONFIG_FUNCTIONS
#define CONFIG_FUNCTIONS

#define CONFIG_FUNCTIONS_MAX_LENGTH 10

#include <Arduino.h>
#include <EEPROM.h>

class ConfigCommander
{
  private:
    int cleanEEPROM(unsigned int address, int8_t size);
    int addressOfConfigSlot(unsigned int slot = 0);
    void parseBytes(const char* str, char sep, byte* bytes, int maxBytes, int base);
    int8_t configSizes[CONFIG_FUNCTIONS_MAX_LENGTH]; // define the max allowed qty of field to accept

  public:
    bool initialize(bool forced = false);
    bool readConfig(int8_t slot, char *output);
    bool readIP(int8_t slot, byte *output);
    bool readMAC(int8_t slot, byte *output);
    bool saveConfig(int8_t slot, char *input);
    void setSize(short int slot, int8_t size);
    int8_t getSize(short int slot);
};

#endif
