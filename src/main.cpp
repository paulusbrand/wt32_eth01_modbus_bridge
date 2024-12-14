// =================================================================================================
// eModbus: Copyright 2020 by Michael Harwerth, Bert Melis and the contributors to ModbusClient
//               MIT license - see license.md for details
// =================================================================================================
// Includes: <Arduino.h> for Serial etc., WiFi.h for WiFi support
//
// Dependency Graph
// |-- SPI @ 3.0.5
// |-- eModbus @ 1.7.2+sha.18a5a88
// |-- AsyncTCP @ 1.1.1+sha.17039c3
// |-- WiFi @ 3.0.5

#include <Arduino.h>
#include <WiFi.h>
#include "HardwareSerial.h"

// Modbus bridge include
#include "ModbusBridgeETH.h"
// Modbus RTU client include
#include "ModbusClientRTU.h"

// Include configuration
#include "config.h"

// Create a ModbusRTU client instance
ModbusClientRTU MB;

// Create bridge
ModbusBridgeEthernet MBbridge;

// Event callback
void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case ARDUINO_EVENT_ETH_START:
      Serial.println("ETH Started");
      ETH.setHostname(hostname);
      break;
    case ARDUINO_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP:
      Serial.print("ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP6:
      Serial.println("We even have IPv6!");
    case ARDUINO_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      break;
    case ARDUINO_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      break;
    default:
      break;
  }
}

void setup() {
  // Init Serial to monitor
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("__ OK __");

  // Init Serial2 connected to the RTU Modbus
  RTUutils::prepareHardwareSerial(Serial2);
  Serial2.begin(9600, SERIAL_8N1, GPIO_NUM_5, GPIO_NUM_17);

  // Set RTU Modbus message timeout to 100ms
  MB.setTimeout(modbus_rtu_timeout);
  // Start ModbusRTU background task on core 1
  MB.begin(Serial2, 1);

  // Register callback
  WiFi.onEvent(WiFiEvent);
  
  // Start ethernet
  ETH.begin(ETH_PHY_TYPE, ETH_PHY_ADDR, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_POWER, ETH_CLK_MODE);

  // Define and start Ethernet modbus bridge for adresses 246 and 247
  MBbridge.attachServer(247, 247, ANY_FUNCTION_CODE, &MB);
  MBbridge.attachServer(246, 246, ANY_FUNCTION_CODE, &MB);

  // Start the bridge
  MBbridge.start(modbus_port, modbus_max_clients, modbus_client_timeout);

  Serial.printf("Use the shown IP and port %d to send requests!\n", modbus_port);
}

// Do nothing
void loop() {;;}
