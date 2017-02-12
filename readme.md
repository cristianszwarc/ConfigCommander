
Config Functions
-------------------------------

Initialize EEPROM in a given format to store data.

**Manual Installation**

[Download](https://github.com/cristianszwarc/ConfigCommander/archive/master.zip) and decompress it to your libraries folder.

**Platformio Installation**

By platformio.ini:
```
...
lib_deps =
     git@github.com:cristianszwarc/ConfigCommander.git
...
```

**Basic use**
```cpp
..
// define how many fields we want to store (default def is 10)
#define CONFIG_FUNCTIONS_MAX_LENGTH 8
#include <ConfigCommander.h>

// define a constant for each field slot (optional to improve the code readability)
#define CONFIG_NAME 0 
#define CONFIG_IP 1
#define CONFIG_GATEWAY 2
#define CONFIG_MASK 3
#define CONFIG_DNS 4
#define CONFIG_SERVER 5
#define CONFIG_SERVER_PORT 6
#define CONFIG_MAC 7

ConfigCommander MyConfigCommander;
...
void setup() {
  
  ...
  
  // define config fields size
  MyConfigCommander.setSize(CONFIG_NAME, 20);     // device name: 20 char
  MyConfigCommander.setSize(CONFIG_IP, 15);       // ip: 15 bytes
  MyConfigCommander.setSize(CONFIG_GATEWAY, 15);  // gateway: 15 bytes
  MyConfigCommander.setSize(CONFIG_MASK, 15);     // mask: 15 bytes
  MyConfigCommander.setSize(CONFIG_DNS, 15);      // dns: 15 bytes
  MyConfigCommander.setSize(CONFIG_MAC, 17);      // mac: 17 bytes
  MyConfigCommander.setSize(CONFIG_SERVER, 30);   // server domain: 30 char
  MyConfigCommander.setSize(CONFIG_SERVER_PORT, 4);   // server port: 4 char

  MyConfigCommander.initialize();                 // clean memory on new boards
  
  ...
  
  // print current board name
  char boardName[21]; // field size plus terminator
  MyConfigCommander.readConfig(CONFIG_NAME, boardName);
  Serial.print(F("Board: "));
  Serial.println(boardName);
  
  ...
  
  // store a new board name
  char newBoardName[21] = "My Device"; // field size plus terminator
  if (MyConfigCommander.saveConfig(CONFIG_NAME, newBoardName)) {
    Serial.println(F("Saved"));
  } else {
    Serial.println(F("Fail"));
  }
  
  ...
  
  // read ethernet settings
  byte mac[6];
  byte ip[4];
  byte gateway[4];
  byte dnsServer[4];
  byte subnet[4];

  MyConfigCommander.readIP(CONFIG_IP, ip);
  MyConfigCommander.readIP(CONFIG_GATEWAY, gateway);
  MyConfigCommander.readIP(CONFIG_DNS, dnsServer);
  MyConfigCommander.readIP(CONFIG_MASK, subnet);
  MyConfigCommander.readMAC(CONFIG_MAC, mac);

  Ethernet.begin(mac, ip, dnsServer, gateway, subnet);
  
}

```

**License**

MIT
