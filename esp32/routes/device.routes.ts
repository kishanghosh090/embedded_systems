// routes/device.routes.ts

import { Router } from "express";

import { register, findOne, findAll } from "../controllers/device.controller";

const router = Router();

router.post("/register", register);

router.get("/", findAll);

router.get("/:deviceId", findOne);

export default router;
