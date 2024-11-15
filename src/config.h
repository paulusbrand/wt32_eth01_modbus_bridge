#ifndef Config_h
#define Config_h

// Hostname
const char * hostname = "Modbus bridge 1\0";

// Port of modbus server
const uint16_t modbus_port = 502;

// Maximum simultanious allowed clients
const uint8_t modbus_max_clients = 4;

// Use 2000ms inactivity to disconnect client
const uint32_t modbus_client_timeout = 2000;

// Modbus RTU timeout in ms
const uint32_t modbus_rtu_timeout = 100;

#endif