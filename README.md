# HSINXA Smart Street Lighting

**Version:** 1.1  
**Project:** Computer Exhibition 2026  
**Controller:** Arduino Uno

An educational smart-street-lighting prototype. Two PIR motion sensors monitor the entry and exit areas. When either sensor detects movement, the four existing street-light LEDs switch on. They switch off after both sensors remain clear for five seconds.

> **Detection note:** The Wokwi components are PIR motion sensors. They detect movement in their field of view; they do not identify cars or distinguish vehicles from pedestrians.

## Components

- Arduino Uno
- 2 × PIR motion sensors (Wokwi PIR modules)
- 4 × yellow LEDs
- Existing 220 Ω resistor
- Breadboard and jumper wires

## Pin connections

| Component | Arduino connection |
| --- | --- |
| Entry PIR OUT | Digital pin 2 |
| Exit PIR OUT | Digital pin 3 |
| Existing street-light LED line | Digital pin 6 |
| Both PIR VCC pins | 5V |
| Both PIR GND pins | GND |
| Existing LED return | GND |

The existing LED wiring arrangement is intentionally unchanged in version 1.1, as requested. Its shared-resistor arrangement remains a known electrical limitation and has not been corrected or validated for a physical installation. Do not treat the current LED circuit as a finished real-world street-lighting design.

## How the program works

1. The Arduino reads both PIR sensor outputs.
2. Motion from either sensor switches the street lights on.
3. While either sensor remains active, the five-second hold timer is refreshed.
4. Once both sensors are clear, the timer runs from the last detected motion.
5. If both sensors remain clear for five seconds, the lights switch off.
6. The Serial Monitor reports sensor state changes and lighting state changes at 9600 baud.

The program uses millis-based elapsed-time timing rather than a blocking delay, so sensor inputs continue to be checked while the lights are on. Serial messages are printed on state changes rather than on every loop.

## Wokwi simulation

1. Open the project in Wokwi.
2. Start the simulation.
3. Trigger motion on the entry PIR sensor, then repeat with the exit PIR sensor.
4. Confirm that either sensor turns the lights on.
5. Keep triggering motion before the five-second hold period expires and confirm that the lights stay on.
6. Stop triggering both sensors and confirm that the lights turn off after five seconds with both outputs LOW.
7. Open the Serial Monitor at 9600 baud and check the sensor and light-state messages.

PIR modules may need a short stabilization period after startup. Follow the simulator's motion controls and observe the actual sensor output states when testing.

## Version 1.1 improvements

- Corrected project wording to identify the sensors as PIR motion sensors.
- Removed the inaccurate claim that the system specifically detects cars.
- Reworked the control logic to keep the lights on while either sensor is active.
- Refreshes the five-second timer during continued motion.
- Turns the lights off only when both sensors are inactive and the hold time has elapsed.
- Removed the blocking loop delay.
- Added clear sensor transition and light-state messages.
- Uses named constants and rollover-safe elapsed-time arithmetic.
- Added a documented simulation test procedure and known limitation.

## Known limitations

- The existing LED wiring arrangement has deliberately not been changed in this release.
- The prototype uses one shared lighting output, so all four LEDs are controlled together.
- PIR sensors detect movement, not vehicle identity, direction, or speed.
- A Wokwi simulation and a physical hardware test are separate forms of validation. Do not claim physical validation until the assembled prototype has been tested.

## Project files

- sketch.ino: Arduino program
- diagram.json: Wokwi circuit layout
- TEST_PLAN.md: validation checklist
