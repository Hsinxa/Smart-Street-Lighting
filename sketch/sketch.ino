/*
  JalRakshak - Flood Warning System
  Computer Exhibition 2026

  Hardware:
  - Arduino UNO
  - HC-SR04 ultrasonic sensor
  - Green, yellow and red LEDs (each with a 220 ohm resistor)
  - Active buzzer
  - Breadboard and jumper wires

  Wiring:
  HC-SR04: TRIG -> D9, ECHO -> D10, VCC -> 5V, GND -> GND
  Green LED anode -> D3 through 220 ohm resistor; cathode -> GND
  Yellow LED anode -> D4 through 220 ohm resistor; cathode -> GND
  Red LED anode -> D5 through 220 ohm resistor; cathode -> GND
  Active buzzer (+) -> D6, (-) -> GND

  The sensor measures the distance from itself to the water surface.
  As the water rises, this distance gets smaller.

  IMPORTANT:
  The thresholds below are example distances from the sensor to the
  water surface. Adjust them for your demonstration tank after testing.
*/

const byte TRIG_PIN = 9;
const byte ECHO_PIN = 10;

const byte GREEN_LED_PIN = 3;
const byte YELLOW_LED_PIN = 4;
const byte RED_LED_PIN = 5;
const byte BUZZER_PIN = 6;

// Example thresholds, measured from the sensor down to the water.
// Greater distance = lower water. Smaller distance = higher water.
const float WARNING_DISTANCE_CM = 30.0;
const float DANGER_DISTANCE_CM = 12.0;

const unsigned long ECHO_TIMEOUT_US = 3000UL;
const unsigned long SAMPLE_INTERVAL_MS = 250UL;

unsigned long lastSampleTime = 0;

float readDistanceCm() {
  // Ensure a clean trigger pulse.
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Send an ultrasonic pulse (10 microseconds).
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the echo pulse duration. A timeout prevents the code
  // from waiting indefinitely if no echo is received.
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  if (duration == 0) {
    return -1.0; // No valid echo.
  }

  // Speed of sound is approximately 0.0343 cm per microsecond.
  // Divide by 2 because the sound travels to the water and back.
  return (duration * 0.0343f) / 2.0f;
}

void setIndicators(bool green, bool yellow, bool red, bool buzzer) {
  digitalWrite(GREEN_LED_PIN, green ? HIGH : LOW);
  digitalWrite(YELLOW_LED_PIN, yellow ? HIGH : LOW);
  digitalWrite(RED_LED_PIN, red ? HIGH : LOW);
  digitalWrite(BUZZER_PIN, buzzer ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  setIndicators(false, false, false, false);

  Serial.println(F("======================================"));
  Serial.println(F("       JALRAKSHAK FLOOD WARNING        "));
  Serial.println(F("       Computer Exhibition 2026       "));
  Serial.println(F("======================================"));
  Serial.println(F("Monitoring water surface distance..."));
  Serial.println(F("Smaller distance means a higher level."));
}

void loop() {
  if (millis() - lastSampleTime < SAMPLE_INTERVAL_MS) {
    return;
  }
  lastSampleTime = millis();

  float distanceCm = readDistanceCm();

  if (distanceCm < 0.0f) {
    // Sensor did not receive an echo. Do not report a false normal reading.
    setIndicators(false, false, false, false);
    Serial.println(F("SENSOR ERROR: No echo received. Check sensor and wiring."));
    return;
  }

  Serial.print(F("Distance to water: "));
  Serial.print(distanceCm, 1);
  Serial.print(F(" cm | "));

  if (distanceCm <= DANGER_DISTANCE_CM) {
    setIndicators(false, false, true, true);
    Serial.println(F("DANGER - High water level!"));
  } else if (distanceCm <= WARNING_DISTANCE_CM) {
    setIndicators(false, true, false, false);
    Serial.println(F("WARNING - Water level is rising."));
  } else {
    setIndicators(true, false, false, false);
    Serial.println(F("NORMAL - Water level is below warning threshold."));
  }
}
