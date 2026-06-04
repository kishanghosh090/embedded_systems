#pragma once

struct AttendanceJob {

  int studentId;

  unsigned long timestamp;
};


enum DeviceMode {

  IDLE = 0,

  ATTENDANCE = 1,

  ENROLL = 2
};

struct DeviceState {

  DeviceMode mode;

  int studentId;
};