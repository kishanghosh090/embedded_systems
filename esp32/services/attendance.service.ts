// services/attendance.service.ts

import { attendances } from "../attendance.store";

export const markAttendance = (deviceId: string, studentId: number) => {
  attendances.push({
    deviceId,
    studentId,
    timestamp: new Date(),
  });
};

export const getAttendances = () => {
  return attendances;
};
