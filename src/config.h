#ifndef Config_h
#define Config_h

// Port of modbus server
uint16_t modbus_port = 502;
// Maximum simultanious allowed clients
uint8_t modbus_max_clients = 4;
// 2000ms inactivity to disconnect client
uint32_t modbus_timeout = 2000;

// Hostname
char hostname[] = "Modbus bridge 1";

#endif