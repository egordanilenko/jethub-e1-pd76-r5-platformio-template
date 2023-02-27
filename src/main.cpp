#include <Arduino.h>

//ethernet jethome e1-pd76-r5
#define ETH_PHY_ADDR  1
#define ETH_CLK_MODE  ETH_CLOCK_GPIO17_OUT
#define ETH_MDC_PIN   23
#define ETH_MDIO_PIN  18

#include <ETH.h>
#include <PCF8575.h>




TwoWire I2Cone = TwoWire(0);

//gpio extender jethome e1-pd76-r5
PCF8575 pcf8575(&I2Cone,0x22);


static bool eth_connected = false;

void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case ARDUINO_EVENT_ETH_START:
      Serial.println("ETH Started");
      ETH.setHostname("esp32-executor");
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
      eth_connected = true;
      break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case ARDUINO_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);

  Serial.print("Init i2c...");
  if(I2Cone.begin(5,4,400000U)){
		Serial.println("OK");
  }else{
		Serial.println("KO");
  }
  // Relay 1 (NO/NC)
  pcf8575.pinMode(8, OUTPUT);
  // Relay 2
  pcf8575.pinMode(9, OUTPUT);
  // Relay 3
  pcf8575.pinMode(10, OUTPUT);
  // Relay 4
  pcf8575.pinMode(11, OUTPUT);
  // Relay 5
  pcf8575.pinMode(12, OUTPUT);
  
  pcf8575.begin();


  WiFi.onEvent(WiFiEvent);

  Serial.print("Init ethernet...");

  if(ETH.begin()){
		Serial.println("OK");
  }else{
		Serial.println("KO");
  }
}

void loop() {

}