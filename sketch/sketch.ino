/*
  JalRakshak - Flood Warning System
  Computer Exhibition 2026

  Hardware:
  - Arduino UNO
  - HC-SR04 ultrasonic sensor
  - Green, yellow and red LEDs (each with a 220 ohm resistor)
  - Active buzzer

  Wiring:
  HC-SR04: TRIG -> D9, ECHO -> D10, VCC -> 5V, GND -> GND
  Green LED anode -> D3 through 220 ohm resistor; cathode -> GND
  Yellow LED anode -> D4 through 220 ohm resistor; cathode -> GND
  Red LED anode -> D5 through 220 ohm resistor; cathode -> GND
  Active buzzer (+) -> D6, (-) -> GND

  The sensor measures the distance from itself to the water surface.
  As the water rises, this distance gets smaller.

  IMPORTANT:
  The warning and danger thresholds are preserved exactly:
  WARNING = 30 cm
  DANGER  = 12 cm
  These are demonstration settings, not universal flood-safety limits.
*/

const byte TRIG_PIN = 9;
const byte ECHO_PIN = 10;

const byte GREEN_LED_PIN = 3;
const byte YELLOW_LED_PIN = 4;
const byte RED_LED_PIN = 5;
const byte BUZZER_PIN = 6;

// Preserved exactly as requested.
const float WARNING_DISTANCE_CM = 30.0f;
const float DANGER_DISTANCE_CM = 12.0f;

// 3000 us gives a theoretical range of about 51 cm.
// It keeps each measurement quick while covering both thresholds.
const unsigned long ECHO_TIMEOUT_US = 3000UL;
const unsigned long SAMPLE_INTERVAL_MS = 100UL;

// Non-blocking danger alarm: 100 ms ON in each 300 ms cycle.
const unsigned long ALARM_CYCLE_MS = 300UL;
const unsigned long ALARM_ON_MS = 100UL;

// Non-blocking sensor-fault indicator.
const unsigned long ERROR_CYCLE_MS = 1000UL;
const unsigned long ERROR_BEEP_MS = 70UL;

unsigned long lastSampleTime = 0;
unsigned long alarmCycleStart = 0;
unsigned long errorCycleStart = 0;

enum SystemState : byte {
  STATE_NORMAL,
  STATE_WARNING,
  STATE_DANGER,
  STATE_SENSOR_ERROR
};

SystemState currentState = STATE_SENSOR_ERROR;

float readDistanceCm() {
  // Start with a clean trigger pulse.
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Trigger the HC-SR04.
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // A short timeout prevents a long wait when no echo is received.
  const unsigned long duration =
      pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  if (duration == 0UL) {
    return -1.0f;
  }

  // Sound travels to the water and back, so divide by two.
  return (duration * 0.0343f) / 2.0f;
}

void applyStateIndicators(unsigned long now) {
  switch (currentState) {
    case STATE_NORMAL:
      digitalWrite(GREEN_LED_PIN, HIGH);
      digitalWrite(YELLOW_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      break;

    case STATE_WARNING:
      digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(YELLOW_LED_PIN, HIGH);
      digitalWrite(RED_LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      break;

    case STATE_DANGER: {
      digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(YELLOW_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, HIGH);

      if (now - alarmCycleStart >= ALARM_CYCLE_MS) {
        alarmCycleStart = now;
      }

      const bool buzzerOn =
          (now - alarmCycleStart) < ALARM_ON_MS;
      digitalWrite(BUZZER_PIN, buzzerOn ? HIGH : LOW);
      break;
    }

    case STATE_SENSOR_ERROR: {
      digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, LOW);

      if (now - errorCycleStart >= ERROR_CYCLE_MS) {
        errorCycleStart = now;
      }

      const unsigned long phase = now - errorCycleStart;
      digitalWrite(YELLOW_LED_PIN,
                   phase < 500UL ? HIGH : LOW);
      digitalWrite(BUZZER_PIN,
                   phase < ERROR_BEEP_MS ? HIGH : LOW);
      break;
    }
  }
}

void printState(float distanceCm) {
  Serial.print(F("Distance: "));
  Serial.print(distanceCm, 1);
  Serial.print(F(" cm | "));

  switch (currentState) {
    case STATE_NORMAL:
      Serial.println(F("NORMAL"));
      break;
    case STATE_WARNING:
      Serial.println(F("WARNING - Water level rising"));
      break;
    case STATE_DANGER:
      Serial.println(F("DANGER - High water level"));
      break;
    case STATE_SENSOR_ERROR:
      Serial.println(F("SENSOR ERROR - Check sensor and wiring"));
      break;
  }
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

  const unsigned long now = millis();
  alarmCycleStart = now;
  errorCycleStart = now;
  applyStateIndicators(now);

  Serial.println(F("======================================"));
  Serial.println(F("       JALRAKSHAK FLOOD WARNING       "));
  Serial.println(F("       Computer Exhibition 2026      "));
  Serial.println(F("======================================"));
  Serial.println(F("Monitoring water surface distance..."));
  Serial.println(F("Warning: 30 cm | Danger: 12 cm"));
  Serial.println(F("Smaller distance means a higher level."));
}

void loop() {
  const unsigned long now = millis();

  // Update alarm patterns continuously without using delay().
  applyStateIndicators(now);

  if (now - lastSampleTime < SAMPLE_INTERVAL_MS) {
    return;
  }
  lastSampleTime = now;

  const float distanceCm = readDistanceCm();

  if (distanceCm < 0.0f) {
    currentState = STATE_SENSOR_ERROR;
    printState(0.0f);
    applyStateIndicators(millis());
    return;
  }

  // Thresholds and comparison directions are unchanged.
  if (distanceCm <= DANGER_DISTANCE_CM) {
    if (currentState != STATE_DANGER) {
      alarmCycleStart = millis();
    }
    currentState = STATE_DANGER;
  } else if (distanceCm <= WARNING_DISTANCE_CM) {
    currentState = STATE_WARNING;
  } else {
    currentState = STATE_NORMAL;
  }

  printState(distanceCm);
  applyStateIndicators(millis());
}
