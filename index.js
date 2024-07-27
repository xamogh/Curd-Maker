require("dotenv").config();
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

function connectDevice() {
  return new Promise((resolve, reject) => {
    device.find().then(() => {
      device.connect();
    });

    device.on("connected", () => {
      console.log("Connected to device!");
      resolve();
    });

    device.on("error", (error) => {
      console.log("Error!", error);
      reject(error);
    });

    device.on("disconnected", () => {
      console.log("Disconnected from device.");
    });
  });
}

async function turnOn() {
  try {
    await device.set({ dps: "1", set: true });
    console.log("Device turned on");
  } catch (error) {
    console.log("Error turning device on:", error);
  }
}

async function turnOff() {
  try {
    await device.set({ dps: "1", set: false });
    console.log("Device turned off");
  } catch (error) {
    console.log("Error turning device off:", error);
  }
}

// Main function to demonstrate usage
async function main() {
  try {
    await connectDevice();

    await turnOn(); // Turn the device on
    // await turnOff(); // Turn the device off
  } catch (error) {
    console.log("An error occurred:", error);
  } finally {
    // device.disconnect();
  }
}

main();
