import express from "express";

const app = express();

app.use(express.json());

app.post("/heartbeat", (req, res) => {
  console.log("Heartbeat Received");
  console.log(req.body);

  res.json({
    success: true,
  });
});

app.post("/attendance", (req, res) => {
  console.log("Attendance Received");
  console.log(req.body);

  res.json({
    success: true,
  });
});

app.listen(3000, "0.0.0.0", () => {
  console.log("Server running");
});
