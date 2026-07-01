import express from "express";
import cors from "cors";
import crypto from "crypto";

const app = express();

app.use(cors());
app.use(express.json());

interface Device {
  deviceId: string;
  firmware: string;
  registeredBy: string;
  collegeId: string;
  departmentId: string;
  deviceSecret: string;
  mqttUser: string;
  mqttPassword: string;
}

const devices: Device[] = [];

app.post("/devices/provision", (req, res) => {
  const { deviceId, firmware, jwt } = req.body;
  console.log(deviceId);

  // fake user
  const registeredBy = "faculty001";

  const collegeId = "COL001";
  const departmentId = "CSE";

  const deviceSecret = crypto.randomUUID();

  const mqttUser = deviceId;

  const mqttPassword = crypto.randomUUID();

  const device: Device = {
    deviceId,
    firmware,
    registeredBy,
    collegeId,
    departmentId,
    deviceSecret,
    mqttUser,
    mqttPassword,
  };

  devices.push(device);

  res.json({
    success: true,
    registeredBy,
    collegeId,
    departmentId,
    deviceSecret,
    mqttUser,
    mqttPassword,
  });
});

app.get("/devices", (_, res) => {
  res.json(devices);
});

app.listen(3000, () => {
  console.log("Server running on port 3000");
});
