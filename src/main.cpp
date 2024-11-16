// =================================================================================================
// eModbus: Copyright 2020 by Michael Harwerth, Bert Melis and the contributors to ModbusClient
//               MIT license - see license.md for details
// =================================================================================================
// Includes: <Arduino.h> for Serial etc., WiFi.h for WiFi support
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
      Serial2.println("ETH Started");
      ETH.setHostname(hostname);
      break;
    case ARDUINO_EVENT_ETH_CONNECTED:
      Serial2.println("ETH Connected");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP:
      Serial2.print("ETH MAC: ");
      Serial2.print(ETH.macAddress());
      Serial2.print(", IPv4: ");
      Serial2.print(ETH.localIP());
      if (ETH.fullDuplex()) {
        Serial2.print(", FULL_DUPLEX");
      }
      Serial2.print(", ");
      Serial2.print(ETH.linkSpeed());
      Serial2.println("Mbps");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP6:
      Serial2.println("We even have IPv6!");
    case ARDUINO_EVENT_ETH_DISCONNECTED:
      Serial2.println("ETH Disconnected");
      break;
    case ARDUINO_EVENT_ETH_STOP:
      Serial2.println("ETH Stopped");
      break;
    default:
      break;
  }
}

void setup() {
  // Init Serial2 to monitor
  Serial2.begin(115200, SERIAL_8N1, GPIO_NUM_5, GPIO_NUM_17);
  while (!Serial2) {}
  Serial2.println("__ OK __");

  // Init Serial connected to the RTU Modbus
  RTUutils::prepareHardwareSerial(Serial);
  Serial.begin(9600);

  // Set RTU Modbus message timeout to 100ms
  MB.setTimeout(modbus_rtu_timeout);
  // Start ModbusRTU background task on core 1
  MB.begin(Serial, 1);

  // Register callback
  WiFi.onEvent(WiFiEvent);
  
  // Start ethernet
  ETH.begin(ETH_PHY_TYPE, ETH_PHY_ADDR, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_POWER, ETH_CLK_MODE);

  // Define and start Ethernet modbus bridge for adresses 246 and 247
  MBbridge.attachServer(247, 247, ANY_FUNCTION_CODE, &MB);
  MBbridge.attachServer(246, 246, ANY_FUNCTION_CODE, &MB);

  // Start the bridge
  MBbridge.start(modbus_port, modbus_max_clients, modbus_client_timeout);

  Serial2.printf("Use the shown IP and port %d to send requests!\n", modbus_port);
}

// Do nothing
void loop() {;;}
