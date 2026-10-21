# Smart Parking System 🚗

An embedded systems project built using **ATmega32** and **Embedded C** to control a smart parking gate, monitor parking occupancy, and display the number of cars inside the parking area.

## 📌 Project Overview

The system uses IR sensors to detect cars entering and exiting the parking area. A DC motor controls the gate, while a 7-segment display shows the current number of parked cars. An LED indicates when the parking area reaches its maximum capacity.

## ⚙️ Hardware Components

* ATmega32 Microcontroller
* 2 × IR Sensors
* DC Motor
* L298 Motor Driver
* 7-Segment Display
* LED
* Resistors
* Power Supply

## 🛠️ Software & Tools

* Embedded C
* AVR Microcontroller
* Atmel Studio
* Proteus *(if used for simulation)*

## 🚀 Features

* Automatic gate control using a DC motor.
* Car detection using IR sensors.
* Parking occupancy counting.
* Real-time occupancy display using a 7-segment display.
* LED indication when the parking area is full.
* Maximum parking capacity of 3 cars.

## 🧠 Project Architecture

The project follows a layered structure to organize the code and separate hardware control from application logic.

* **APP:** Main application logic.
* **HAL:** Hardware abstraction drivers, such as LED, IR sensor, DC motor, and 7-segment display.
* **MCAL:** Microcontroller abstraction drivers, such as DIO.
* **LIB:** Common definitions and utilities.

## 🔄 How It Works

1. The IR sensors detect cars near the parking gate.
2. The DC motor opens and closes the gate.
3. The system updates the parking counter when a car enters or exits.
4. The 7-segment display shows the current number of parked cars.
5. The LED indicates when the parking area reaches its maximum capacity.

---

*This project was developed as a practical application of embedded systems concepts and peripheral interfacing using the ATmega32 microcontroller.*
