require("dotenv").config();
const express = require("express");
const http = require("http");
const socketIo = require("socket.io");
const path = require("path");
const net = require("net");
const TuyAPI = require("tuyapi");

const app = express();
const server = http.createServer(app);
const io = socketIo(server);

const device = new TuyAPI({
  id: process.env.TUYA_DEVICE_ID,
  key: process.env.TUYA_DEVICE_KEY,
});

let deviceState = false;
let currentTemp = 0;
let minTemp = 36.0;
let maxTemp = 38.0;
const tempLog = [];

async function connectDevice() {
  try {
    await device.find();
    await device.connect();
    console.log("Connected to Tuya device");
  } catch (error) {
    console.error("Failed to connect to Tuya device:", error);
  }
}

async function setDeviceState(turnOn) {
  if (turnOn !== deviceState) {
    try {
      await device.set({ set: turnOn });
      console.log(`Device turned ${turnOn ? "on" : "off"}`);
      deviceState = turnOn;
      io.emit("deviceUpdate", { deviceState });
    } catch (error) {
      console.error("Error setting device state:", error);
    }
  }
}

async function controlTemperature(temperature) {
  if (temperature < minTemp && !deviceState) {
    await setDeviceState(true);
  } else if (temperature > maxTemp && deviceState) {
    await setDeviceState(false);
  }
}

const tcpServer = net.createServer((socket) => {
  console.log("NodeMCU connected from:", socket.remoteAddress);

  socket.on("data", async (data) => {
    const temperature = parseFloat(data.toString().trim());
    if (!isNaN(temperature)) {
      console.log(`Received temperature: ${temperature}°C`);
      currentTemp = temperature;
      tempLog.push({ timestamp: new Date().toISOString(), temperature });

      await controlTemperature(temperature);

      io.emit("tempUpdate", { temperature, deviceState });
      io.emit("newTempData", {
        timestamp: tempLog[tempLog.length - 1].timestamp,
        temperature,
      });
    }
  });

  socket.on("end", () => {
    console.log("NodeMCU disconnected");
  });

  socket.on("error", (err) => {
    console.log("Socket error:", err);
  });
});

app.use(express.static("public"));
app.use(express.json());

app.get("/api/status", (req, res) => {
  res.json({ currentTemp, deviceState, minTemp, maxTemp });
});

app.post("/api/setTemp", (req, res) => {
  const { min, max } = req.body;
  if (min && max && min < max) {
    minTemp = parseFloat(min);
    maxTemp = parseFloat(max);
    res.json({ success: true, minTemp, maxTemp });
  } else {
    res
      .status(400)
      .json({ success: false, message: "Invalid temperature range" });
  }
});

app.get("/api/tempLog", (req, res) => {
  res.json(tempLog);
});

io.on("connection", (socket) => {
  console.log("New client connected");
  socket.emit("tempUpdate", { temperature: currentTemp, deviceState });
  socket.emit("fullTempLog", tempLog);
});

const TCP_PORT = process.env.TCP_PORT || 3000;
const HTTP_PORT = process.env.HTTP_PORT || 3001;

async function main() {
  try {
    await connectDevice();

    tcpServer.listen(TCP_PORT, () => {
      console.log(`TCP Server listening on port ${TCP_PORT}`);
    });

    server.listen(HTTP_PORT, () => {
      console.log(`HTTP Server listening on port ${HTTP_PORT}`);
    });

    device.on("data", (data) => {
      deviceState = data.dps["1"];
      console.log(`Device state updated: ${deviceState ? "on" : "off"}`);
      io.emit("deviceUpdate", { deviceState });
    });
  } catch (error) {
    console.log("An error occurred:", error);
  }
}

main();
