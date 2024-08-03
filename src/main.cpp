#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "env.h"

const int oneWireBus = 4; // GPIO pin D2 on NodeMCU
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

const int MEASUREMENT_INTERVAL = 5000;
const int WIFI_RETRY_DELAY = 5000;
const int SERVER_RETRY_DELAY = 5000;

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
const char *serverHost = SERVER_HOST;
const int serverPort = SERVER_PORT;

WiFiClient client;

void setupWiFi();
float readTemperature();
bool connectToServer();
void sendTemperature(float temp);

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.println("\nNodeMCU Temperature Sender for L. reuteri Yogurt");
  sensors.begin();
  setupWiFi();
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi disconnected. Attempting to reconnect...");
    setupWiFi();
  }

  if (!client.connected() && !connectToServer())
  {
    delay(SERVER_RETRY_DELAY);
    return;
  }

  float temperature = readTemperature();
  if (temperature != DEVICE_DISCONNECTED_C)
  {
    sendTemperature(temperature);
  }

  delay(MEASUREMENT_INTERVAL);
}

void setupWiFi()
{
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20)
  {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("\nFailed to connect to WiFi. Will retry later.");
  }
}

float readTemperature()
{
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);

  if (temperatureC == DEVICE_DISCONNECTED_C)
  {
    Serial.println("Error: Temperature sensor disconnected or malfunctioning!");
  }
  else
  {
    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.println("°C");
  }

  return temperatureC;
}

bool connectToServer()
{
  if (!client.connected())
  {
    Serial.print("Connecting to server: ");
    Serial.println(serverHost);

    if (client.connect(serverHost, serverPort))
    {
      Serial.println("Connected to server");
      return true;
    }
    else
    {
      Serial.println("Connection to server failed");
      return false;
    }
  }
  return true;
}

void sendTemperature(float temp)
{
  if (client.connected())
  {
    client.println(String(temp, 2));
    Serial.print("Sent temperature: ");
    Serial.println(temp);
  }
  else
  {
    Serial.println("Error: Client disconnected. Couldn't send temperature.");
  }
}

// void sendTemperature(float temperature)
// {
//   WiFiClient client;
//   int attempts = 0;

//   while (attempts < MAX_SERVER_ATTEMPTS)
//   {
//     Serial.print("Connecting to server: ");
//     Serial.println(SERVER_HOST);

//     if (client.connect(SERVER_HOST, SERVER_PORT))
//     {
//       Serial.println("Connected successfully!");
//       String data = String(temperature);
//       client.println(data);
//       Serial.print("Sent: ");
//       Serial.println(data);
//       client.stop();
//       Serial.println("Connection closed");
//       return;
//     }
//     else
//     {
//       Serial.println("Connection failed. Retrying...");
//       attempts++;
//       delay(RETRY_INTERVAL);
//     }
//   }

//   Serial.println("Failed to connect to the server after multiple attempts.");
// }