# L. reuteri Yogurt Temperature Control System

This project implements an automated temperature control system for making L. reuteri yogurt. It uses a NodeMCU (ESP8266) with a DS18B20 temperature sensor to monitor the temperature and control a Tuya smart plug via a Node.js server.

## Components

1. NodeMCU (ESP8266) with temperature sensor
2. Node.js server
3. Tuya smart plug

## Features

- Monitors temperature using a DS18B20 sensor
- Maintains temperature between 36°C and 38°C for optimal L. reuteri yogurt cultivation
- Controls a Tuya smart plug to manage heating
- Provides real-time temperature monitoring and control

## Prerequisites

- NodeMCU (ESP8266) board
- DS18B20 temperature sensor
- Tuya smart plug
- Node.js installed on your server machine

## Setup

### NodeMCU Setup

1. Install the following libraries in your Arduino IDE:
   - ESP8266WiFi
   - OneWire
   - DallasTemperature

2. Create an `env.h` file in the same directory as your NodeMCU code with the following content:

   ```cpp
   #define WIFI_SSID "Your_WiFi_SSID"
   #define WIFI_PASSWORD "Your_WiFi_Password"
   #define SERVER_HOST "Your_Server_IP"
   #define SERVER_PORT Your_Server_Port
   ```

3. Update the `env.h` file with your WiFi credentials and server details.

4. Upload the NodeMCU code to your ESP8266 board.

### Node.js Server Setup

1. Clone this repository to your server.

2. Navigate to the server directory and run:

   ```
   npm install
   ```

3. Create a `.env` file in the server directory with the following content:

   ```
   TUYA_DEVICE_ID=your_tuya_device_id
   TUYA_DEVICE_KEY=your_tuya_device_key
   SERVER_PORT=your_server_port
   ```

4. Update the `.env` file with your Tuya device credentials and desired server port.

## Usage

1. Start the Node.js server:

   ```
   node server.js
   ```

2. Power on the NodeMCU device.

3. The system will automatically start monitoring the temperature and controlling the Tuya smart plug to maintain the optimal temperature range for L. reuteri yogurt (36°C - 38°C).

## Troubleshooting

- If the NodeMCU is not connecting to the server, check the following:
  - Ensure the WiFi credentials in `env.h` are correct.
  - Verify that the server IP and port in `env.h` match your Node.js server.
  - Check if there are any firewalls blocking the connection.

- If the Tuya device is not responding, verify the `TUYA_DEVICE_ID` and `TUYA_DEVICE_KEY` in the `.env` file.

## Contributing

Contributions to improve the system are welcome. Please submit a pull request or open an issue to discuss proposed changes.

## License

This project is open source and available under the [MIT License](LICENSE).