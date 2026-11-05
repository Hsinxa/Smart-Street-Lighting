# Arduino Simulation Project with Wokwi

This project is set up to simulate an **Arduino Uno** using the **Wokwi Simulator** extension in Antigravity / VS Code.

---

## 📁 Project Structure

- [sketch.ino](file:///c:/Users/Pulkit/Desktop/Computer%20Exhibition%202026/sketch.ino): Arduino starter code (Blinks Pin 13 LED and outputs serial telemetry at 115200 baud).
- [diagram.json](file:///c:/Users/Pulkit/Desktop/Computer%20Exhibition%202026/diagram.json): Virtual hardware circuit diagram (Arduino Uno connected to a red LED and a 220Ω current-limiting resistor).
- [wokwi.toml](file:///c:/Users/Pulkit/Desktop/Computer%20Exhibition%202026/wokwi.toml): Wokwi configuration file mapping the simulator to the compiled binary (`build/sketch.ino.hex`).
- [.vscode/extensions.json](file:///c:/Users/Pulkit/Desktop/Computer%20Exhibition%202026/.vscode/extensions.json): Workspace recommended extensions (`wokwi.wokwi-vscode`).
- [.vscode/arduino.json](file:///c:/Users/Pulkit/Desktop/Computer%20Exhibition%202026/.vscode/arduino.json): Arduino board configuration (`arduino:avr:uno`, build output directory).
- [.vscode/tasks.json](file:///c:/Users/Pulkit/Desktop/Computer%20Exhibition%202026/.vscode/tasks.json): Build task for `arduino-cli`.

---

## 🚀 Setup & Running Instructions

### 1. Install Wokwi Extension
1. Open the Extensions sidebar (`Ctrl + Shift + X`).
2. Search for **Wokwi Simulator** (`wokwi.wokwi-vscode`).
3. Click **Install**.

### 2. Request / Activate Wokwi License
1. Press `Ctrl + Shift + P` (or `F1`) to open the Command Palette.
2. Type `Wokwi: Request Free Trial` or `Wokwi: Sign in to Wokwi Club`.
3. Follow the browser prompt to obtain and paste your license key (if prompted).

### 3. Compile the Code
Wokwi simulates the compiled binary (`.hex` file):
- **Option A (Arduino IDE / CLI)**:
  Run in terminal:
  ```powershell
  arduino-cli compile --fqbn arduino:avr:uno --output-dir build sketch.ino
  ```
  *(Or press `Ctrl + Shift + B` to trigger the default build task if `arduino-cli` is in your PATH)*.
- **Option B (PlatformIO)**:
  If you use PlatformIO, update `wokwi.toml` firmware path to `.pio/build/uno/firmware.hex` and click Build.

### 4. Start the Simulator
- Open [diagram.json](file:///c:/Users/Pulkit/Desktop/Computer%20Exhibition%202026/diagram.json) and click the green **Play** button at the top-right of the diagram editor.
- OR press `Ctrl + Shift + P` and select **`Wokwi: Start Simulator`**.
- The virtual Arduino board and LED will appear, along with the interactive Serial Monitor!

