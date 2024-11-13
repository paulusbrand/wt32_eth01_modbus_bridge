// =================================================================================================
// eModbus: Copyright 2020 by Michael Harwerth, Bert Melis and the contributors to ModbusClient
//               MIT license - see license.md for details
// =================================================================================================
// Includes: <Arduino.h> for Serial etc., WiFi.h for WiFi support
#include <Arduino.h>
#include "HardwareSerial.h"
#include <WiFi.h>

// Modbus bridge include
#include "ModbusBridgeWiFi.h"
// Modbus RTU client include
#include "ModbusClientRTU.h"

#ifndef MY_SSID
#define MY_SSID "myssid"
#endif
#ifndef MY_PASS
#define MY_PASS "testtesttest"
#endif

char ssid[] = MY_SSID;                     // SSID and ...
char pass[] = MY_PASS;                     // password for the WiFi network used
uint16_t port = 502;                       // port of modbus server

// Create a ModbusRTU client instance
ModbusClientRTU MB;

// Create bridge
ModbusBridgeWiFi MBbridge;

// Setup() - initialization happens here
void setup() {
// Init Serial monitor
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("__ OK __");

// Init Serial2 conneted to the RTU Modbus
// (Fill in your data here!)
  RTUutils::prepareHardwareSerial(Serial2);
  Serial2.begin(9600, SERIAL_8N1, GPIO_NUM_5, GPIO_NUM_17);

// Connect to WiFi
  WiFi.begin(ssid, pass);
  delay(200);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  IPAddress wIP = WiFi.localIP();
  Serial.printf("IP address: %u.%u.%u.%u\n", wIP[0], wIP[1], wIP[2], wIP[3]);

// Set RTU Modbus message timeout to 2000ms
  MB.setTimeout(2000);
// Start ModbusRTU background task on core 1
  MB.begin(Serial2, 1);

// Define and start WiFi bridge
// ServerID 4: Server with remote serverID 1, accessed through RTU client MB
//             All FCs accepted, with the exception of FC 06
  MBbridge.attachServer(247, 247, ANY_FUNCTION_CODE, &MB);

// Check: print out all combinations served to Serial
  MBbridge.listServer();

// Start the bridge. Port 502, 4 simultaneous clients allowed, 600ms inactivity to disconnect client
  MBbridge.start(port, 4, 600);

  Serial.printf("Use the shown IP and port %d to send requests!\n", port);

// Your output on the Serial monitor should start with:
//      __ OK __
//      .IP address: 192.168.178.74
//      [N] 1324| ModbusServer.cpp     [ 127] listServer: Server   4:  00 06
//      [N] 1324| ModbusServer.cpp     [ 127] listServer: Server   5:  03 04
//      Use the shown IP and port 502 to send requests!
}

// loop() - nothing done here today!
void loop() {
  delay(10000);
}
