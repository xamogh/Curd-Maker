#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const int oneWireBus = 4; // GPIO 4 (D2 on NodeMCU)

OneWire oneWire(oneWireBus);

DallasTemperature sensors(&oneWire);

DeviceAddress devAddress;

void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16)
      Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println("DS18B20 Temperature Sensor Test");

  sensors.begin();

  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  Serial.print("Parasite power is: ");
  if (sensors.isParasitePowerMode())
    Serial.println("ON");
  else
    Serial.println("OFF");

  if (!sensors.getAddress(devAddress, 0))
  {
    Serial.println("Unable to find address for Device 0");
  }
  else
  {
    Serial.print("Device 0 Address: ");
    printAddress(devAddress);
    Serial.println();
  }
}

void loop()
{
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");

  float tempC = sensors.getTempCByIndex(0);

  if (tempC != DEVICE_DISCONNECTED_C)
  {
    Serial.print("Temperature for Device 0: ");
    Serial.print(tempC);
    Serial.println("°C");
  }
  else
  {
    Serial.println("Error: Could not read temperature data");
  }

  delay(5000);
}