
import type { Request, Response } from "express";
import {
  registerDevice,
  getDevice,
  getDevices,
} from "../services/device.service";

export const register = (req: Request, res: Response) => {
  const { deviceId, ip } = req.body;

  console.log(deviceId, ip);

  registerDevice(deviceId, ip);

  res.json({
    success: true,
  });
};

export const findOne = (req: Request, res: Response) => {
  const device = getDevice((req.params as { deviceId: string }).deviceId);

  if (!device) {
    return res.status(404).json({
      message: "Device not found",
    });
  }

  res.json(device);
};

export const findAll = (req: Request, res: Response) => {
  res.json(getDevices());
};
