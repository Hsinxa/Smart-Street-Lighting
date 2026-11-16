// Smart Street Light Project
// Computer Exhibition 2026
// Arduino Uno + 2 IR Sensors + 4 Street Light LEDs

// Pin configuration
const int streetLightsPin = 6;  // Controls the 4 LEDs
const int sensor1Pin = 2;       // Entry IR motion sensor
const int sensor2Pin = 3;       // Exit IR motion sensor

// Settings
const unsigned long onTime = 5000;  // Keep lights on for 5 seconds after motion

unsigned long lastTriggerTime = 0;
bool lightsOn = false;

void setup() {
  Serial.begin(9600);

  pinMode(streetLightsPin, OUTPUT);
  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);

  // Start with lights turned off
  digitalWrite(streetLightsPin, LOW);

  Serial.println("Smart Street Light System Started");
  Serial.println("Waiting for cars to pass by...");
}

void loop() {
  // Read both IR motion sensors
  int sensor1 = digitalRead(sensor1Pin);
  int sensor2 = digitalRead(sensor2Pin);

  unsigned long currentMillis = millis();

  // If either sensor spots a car
  if (sensor1 == HIGH || sensor2 == HIGH) {
    lastTriggerTime = currentMillis;

    if (!lightsOn) {
      digitalWrite(streetLightsPin, HIGH);
      lightsOn = true;

      if (sensor1 == HIGH && sensor2 == HIGH) {
        Serial.println("Motion detected on both sensors!");
      } else if (sensor1 == HIGH) {
        Serial.println("Car detected at Entry (Sensor 1) -> Turning lights ON");
      } else {
        Serial.println("Car detected at Exit (Sensor 2) -> Turning lights ON");
      }
    }
  }

  // Turn lights off after 5 seconds of no motion
  if (lightsOn && (currentMillis - lastTriggerTime >= onTime)) {
    if (sensor1 == LOW && sensor2 == LOW) {
      digitalWrite(streetLightsPin, LOW);
      lightsOn = false;
      Serial.println("No more cars detected. Turning lights OFF to save power.");
    }
  }

  delay(50);
}
