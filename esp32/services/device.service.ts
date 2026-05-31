// services/device.service.ts

import { devices } from "../devices";

export const registerDevice = (deviceId: string, ip: string) => {
  devices.set(deviceId, {
    deviceId,
    ip,
    lastSeen: new Date(),
  });
};

export const getDevice = (deviceId: string) => {
  return devices.get(deviceId);
};

export const getDevices = () => {
  return Array.from(devices.values());
};
