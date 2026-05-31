// controllers/attendance.controller.ts

import type { Request, Response } from "express";

import { markAttendance, getAttendances } from "../services/attendance.service";

export const createAttendance = (req: Request, res: Response) => {
  const { deviceId, studentId } = req.body;
  console.log(deviceId, studentId);
  
  markAttendance(deviceId, studentId);

  res.status(201).json({
    success: true,
  });
};

export const findAllAttendance = (req: Request, res: Response) => {
  res.json(getAttendances());
};
