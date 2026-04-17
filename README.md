# Smart Parking Lot Indicator System

An Arduino-based embedded system designed to automate vehicle entry and exit while displaying real-time parking availability.

## 🚀 Features
- Real-time parking slot tracking
- Automated gate control using servo motor
- Entry/exit detection using IR sensors
- LCD display for live status updates
- Buzzer alerts for system feedback

## ⚙️ How It Works
- Two IR sensors detect vehicles at entry and exit points
- Entry reduces available slots and opens the gate if space is available
- Exit increases available slots (up to maximum capacity)
- If parking is full, entry is blocked and a warning buzzer is triggered

## 🧠 Logic Implemented
- Prevents multiple detections of the same vehicle
- Uses cooldown timing to avoid false triggers
- Ensures slot count stays within limits

## 🖥️ Display Output
- Shows:
  - `SLOTS = X`
  - `Available`
  - `FULL`
- Temporary messages:
  - `WELCOME`
  - `THANK YOU`
  - `PARKING FULL`

## 🔧 Components Used
- Arduino Board
- IR Sensors (x2)
- Servo Motor
- 16x2 LCD Display
- Buzzer

## 📚 Project Context
Built as part of an MPCA (Microprocessor and Computer Architecture) lab project.

## Circuit Diagram
![Circuit Diagram](circuit-diagram.png)
