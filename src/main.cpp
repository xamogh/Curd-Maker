#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "env.h"

const int oneWireBus = 4; // GPIO pin D2 on NodeMCU
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.println("\nNodeMCU Temperature Sensor");
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("NodeMCU IP address: ");
  Serial.println(WiFi.localIP());
  sensors.begin();
}

void loop()
{
  Serial.println("\n------- Reading temperature and sending data -------");
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  if (temperatureC == DEVICE_DISCONNECTED_C)
  {
    Serial.println("Error reading temperature!");
    delay(5000);
    return;
  }
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println("°C");

  WiFiClient client;
  Serial.print("Connecting to ");
  Serial.println(SERVER_HOST);
  if (!client.connect(SERVER_HOST, SERVER_PORT))
  {
    Serial.println("Connection failed");
    delay(5000);
    return;
  }
  Serial.println("Connected successfully!");
  String data = "Temperature: " + String(temperatureC) + "°C";
  client.println(data);
  Serial.print("Sent: ");
  Serial.println(data);
  Serial.println("Closing connection");
  client.stop();
  delay(30000);
}