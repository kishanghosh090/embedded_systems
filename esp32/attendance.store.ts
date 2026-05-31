// store/attendance.store.ts

export interface AttendanceRecord {
  deviceId: string;
  studentId: number;
  timestamp: Date;
}

export const attendances: AttendanceRecord[] = [];
