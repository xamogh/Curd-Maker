require("dotenv").config();
const net = require("net");
const TuyAPI = require("tuyapi");

if (!process.env.TUYA_DEVICE_ID || !process.env.TUYA_DEVICE_KEY) {
  console.error(
    "Error: TUYA_DEVICE_ID and TUYA_DEVICE_KEY must be set in the .env file"
  );
  process.exit(1);
}

const device = new TuyAPI({
  id: process.env.TUYA_DEVICE_ID,
  key: process.env.TUYA_DEVICE_KEY,
});

let deviceState = false;

function connectDevice() {
  return new Promise((resolve, reject) => {
    device.find().then(() => {
      device.connect();
    });

    device.on("connected", () => {
      console.log("Connected to Tuya device!");
      resolve();
    });

    device.on("error", (error) => {
      console.log("Tuya device error!", error);
      reject(error);
    });

    device.on("disconnected", () => {
      console.log("Disconnected from Tuya device.");
    });
  });
}

async function setDeviceState(turnOn) {
  if (turnOn !== deviceState) {
    try {
      await device.set({ dps: "1", set: turnOn });
      console.log(`Device turned ${turnOn ? "on" : "off"}`);
      deviceState = turnOn;
    } catch (error) {
      console.log(`Error setting device state:`, error);
    }
  } else {
    console.log(`Device already ${turnOn ? "on" : "off"}`);
  }
}

// TCP server to receive control flags from ESP8266
const server = net.createServer((socket) => {
  console.log("ESP8266 connected from:", socket.remoteAddress);

  let buffer = "";

  socket.on("data", (data) => {
    buffer += data.toString();
    let index;
    while ((index = buffer.indexOf("\n")) !== -1) {
      const controlFlag = buffer.substring(0, index).trim();
      buffer = buffer.substring(index + 1);

      console.log(`Received control flag: ${controlFlag}`);

      if (controlFlag === "ON") {
        setDeviceState(true);
      } else if (controlFlag === "OFF") {
        setDeviceState(false);
      } else if (controlFlag === "MAINTAIN") {
        console.log("Maintaining current state");
      } else {
        console.log(`Invalid control flag: ${controlFlag}`);
      }
    }
  });

  socket.on("end", () => {
    console.log("ESP8266 disconnected");
  });

  socket.on("error", (err) => {
    console.log("Socket error:", err);
  });
});

const PORT = process.env.SERVER_PORT || 3000;

async function main() {
  try {
    await connectDevice();
    server.listen(PORT, () => {
      console.log(`Server listening on port ${PORT}`);
    });

    server.on("error", (err) => {
      console.log("Server error:", err);
    });

    server.on("connection", (socket) => {
      console.log("New connection from:", socket.remoteAddress);
    });
  } catch (error) {
    console.log("An error occurred:", error);
  }
}

main();
