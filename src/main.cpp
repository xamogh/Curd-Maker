#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "env.h"

const int oneWireBus = 4; // GPIO pin D2 on NodeMCU
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

const int MAX_WIFI_ATTEMPTS = 20;
const int MAX_SERVER_ATTEMPTS = 3;
const unsigned long RETRY_INTERVAL = 5000;
const unsigned long MEASUREMENT_INTERVAL = 5000;

// Temperature range for L. reuteri yogurt (in Celsius)
const float MIN_TEMP = 36.0;
const float MAX_TEMP = 38.0;

WiFiClient client;

void setupWiFi();
float readTemperature();
bool connectToServer();
void sendControlFlag(const String &flag);

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.println("\nNodeMCU Temperature Control for L. reuteri Yogurt");
  setupWiFi();
  sensors.begin();
}

void loop()
{
  static unsigned long lastMeasurementTime = 0;
  static String lastSentFlag = "";
  unsigned long currentTime = millis();

  if (currentTime - lastMeasurementTime >= MEASUREMENT_INTERVAL)
  {
    lastMeasurementTime = currentTime;

    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("WiFi disconnected. Attempting to reconnect...");
      setupWiFi();
    }

    float temperature = readTemperature();
    if (temperature != DEVICE_DISCONNECTED_C)
    {
      String flag;
      if (temperature < MIN_TEMP)
      {
        flag = "ON";
      }
      else if (temperature > MAX_TEMP)
      {
        flag = "OFF";
      }
      else
      {
        flag = "MAINTAIN";
      }

      if (flag != lastSentFlag)
      {
        if (connectToServer())
        {
          sendControlFlag(flag);
          lastSentFlag = flag;
        }
      }
    }
  }
}

void setupWiFi()
{
  int attempts = 0;
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED && attempts < MAX_WIFI_ATTEMPTS)
  {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nWiFi connected");
    Serial.print("NodeMCU IP address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("\nFailed to connect to WiFi. Please check your credentials.");
  }
}

float readTemperature()
{
  Serial.println("\n------- Reading temperature -------");
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  if (temperatureC == DEVICE_DISCONNECTED_C)
  {
    Serial.println("Error: Temperature sensor disconnected or malfunctioning!");
    return DEVICE_DISCONNECTED_C;
  }
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println("°C");
  return temperatureC;
}

bool connectToServer()
{
  if (!client.connected())
  {
    Serial.print("Connecting to server: ");
    Serial.println(SERVER_HOST);
    if (client.connect(SERVER_HOST, SERVER_PORT))
    {
      Serial.println("Connected successfully!");
      return true;
    }
    else
    {
      Serial.println("Connection failed. Will retry on next loop.");
      return false;
    }
  }
  return true;
}

void sendControlFlag(const String &flag)
{
  if (client.connected())
  {
    client.println(flag);
    Serial.print("Sent control flag: ");
    Serial.println(flag);
  }
  else
  {
    Serial.println("Error: Client disconnected. Couldn't send flag.");
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