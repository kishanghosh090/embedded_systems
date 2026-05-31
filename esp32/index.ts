// index.ts

import express from "express";

const app = express();

app.use(express.json());
app.get("/", (req, res) => {
  return res.json({ msg: "server is alive" });
});

import deviceRoutes from "./routes/device.routes";
import attendanceRoutes from "./routes/attendance.routes";

app.use("/api/devices", deviceRoutes);
app.use("/api/attendance", attendanceRoutes);

app.listen(3000, "0.0.0.0", () => {
  console.log("Server running on port 3000");
});
