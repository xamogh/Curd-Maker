# Yogurt Temperature Control System

This project implements a temperature control system for making L. reuteri yogurt. It uses a NodeMCU with a DS18B20 temperature sensor to monitor the temperature and a Node.js server to control a Tuya smart plug for maintaining the desired temperature range.

## Features

- Real-time temperature monitoring
- Automatic control of heating element via Tuya smart plug
- Web interface for setting temperature range and viewing temperature history
- Full temperature history logging and graphing

## Hardware Requirements

- NodeMCU (ESP8266)
- DS18B20 temperature sensor
- Tuya-compatible smart plug
- Heating element (e.g., heating pad or incandescent bulb)

## Software Requirements

- Node.js (v12 or later)
- npm (usually comes with Node.js)

## Setup

### NodeMCU Setup

1. Connect the DS18B20 temperature sensor to your NodeMCU:
   - VCC to 3.3V
   - GND to GND
   - DATA to GPIO 4 (D2 on NodeMCU)

2. Install the Arduino IDE and set it up for ESP8266 development.

3. Install the following libraries in the Arduino IDE:
   - ESP8266WiFi
   - OneWire
   - DallasTemperature

4. Create a file named `env.h` in the same directory as your NodeMCU sketch with the following content:

   ```cpp
   #define WIFI_SSID "Your_WiFi_SSID"
   #define WIFI_PASSWORD "Your_WiFi_Password"
   #define SERVER_HOST "Your_Server_IP_Address"
   #define SERVER_PORT Your_Server_Port
   ```

5. Upload the NodeMCU sketch to your device.

### Server Setup

1. Clone this repository to your local machine.

2. Navigate to the project directory and install the required npm packages:

   ```
   npm install
   ```

3. Create a `.env` file in the project root with the following content:

   ```
   TUYA_DEVICE_ID=your_tuya_device_id
   TUYA_DEVICE_KEY=your_tuya_device_key
   TCP_PORT=3000
   HTTP_PORT=3001
   ```

   Replace `your_tuya_device_id` and `your_tuya_device_key` with your Tuya smart plug credentials.

4. Start the server:

   ```
   node server.js
   ```

## Usage

1. Power on your NodeMCU and ensure it's connected to your WiFi network.

2. Start the Node.js server if it's not already running.

3. Open a web browser and navigate to `http://your_server_ip:3001` (replace `your_server_ip` with the IP address of the machine running the Node.js server).

4. Use the web interface to:
   - View the current temperature
   - Set the desired temperature range
   - Monitor the temperature history graph
   - Check the current status of the heating element (ON/OFF)

## Troubleshooting

- If the NodeMCU is not connecting to WiFi, double-check the SSID and password in the `env.h` file.
- If the server is not receiving temperature data, ensure that the NodeMCU is powered on and the SERVER_HOST and SERVER_PORT in `env.h` are correct.
- If the Tuya smart plug is not responding, verify the TUYA_DEVICE_ID and TUYA_DEVICE_KEY in the `.env` file.

## Contributing

Contributions to improve the system are welcome. Please feel free to submit a Pull Request.

## License

This project is open source and available under the [MIT License](LICENSE).

## Disclaimer

This system is designed for personal use. Please ensure you follow proper food safety guidelines when making yogurt.