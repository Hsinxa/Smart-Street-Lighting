# Smart Street Light System

A working model of an automated, energy-saving street lighting system for the 2026 Computer Exhibition. 

The system uses an Arduino Uno, two IR motion sensors, and four street lights (LEDs). The lights automatically switch on whenever a car enters either end of the road, and turn off once the vehicle passes to conserve power.

---

## Components Used

- **Arduino Uno**
- **4x LEDs** (acting as street lights along the road)
- **1x 220Ω Resistor**
- **2x IR / PIR Motion Sensors** (one for entry, one for exit)
- Breadboard & jumper wires

---

## Pin Connections

| Component | Arduino Pin | Description |
| :--- | :--- | :--- |
| Street Lights (4 LEDs) | **Pin 6** | Positive line through the 220Ω resistor |
| Ground (LEDs) | **GND** | Shared negative return line |
| IR Sensor 1 (Entry) | **Pin 2** | Digital OUT pin |
| IR Sensor 2 (Exit) | **Pin 3** | Digital OUT pin |
| Sensor Power | **5V & GND** | Shared power rails for both sensors |

> **Note on wiring the 4 LEDs:**  
> Each LED has a forward voltage drop of roughly 1.8V to 2V. Putting four of them end-to-end in strict series would need around 7V to 8V, which is higher than the 5V an Arduino pin provides. To make sure all four lights turn on reliably at 5V, they are wired along a shared line from Pin 6 through the 220Ω resistor to ground.

---

## How It Works

1. **Idle State**: The street lights stay off while there is no traffic on the road.
2. **Vehicle Entry**: When a car approaches either sensor (Pin 2 or Pin 3), the sensor sends a `HIGH` signal to the Arduino.
3. **Lights On**: The Arduino immediately sends power through Pin 6, turning on all four street lights.
4. **Timer & Auto-off**: Every time motion is detected, the timer resets. Once a car has passed and no motion is sensed for 5 seconds, the lights turn back off.
5. **Serial Monitor**: You can open the Serial Monitor (set to 9600 baud) to see live messages whenever a car is detected.

---

## How to Test in Wokwi

1. Open `diagram.json`.
2. Click the green **Play** button in the top right to start the simulation.
3. Click on either motion sensor on the screen, then click **Simulate Motion**.
4. You'll see all four yellow street lights turn on and messages appear in the serial console.
5. After 5 seconds without clicking motion, the lights will turn off on their own.
