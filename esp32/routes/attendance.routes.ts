// routes/attendance.routes.ts

import { Router } from "express";

import {
  createAttendance,
  findAllAttendance,
} from "../controllers/attendance.controller";

const router = Router();

router.post("/", createAttendance);

router.get("/", findAllAttendance);

export default router;
