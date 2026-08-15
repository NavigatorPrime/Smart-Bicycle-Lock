#include "DHT.h"
#include <AFMotor.h>

#define DHTPIN 2     // The digital pin connected to the DHT11 data pin
#define DHTTYPE DHT11   // Defining the specific sensor type
DHT dht(DHTPIN, DHTTYPE);

const int methanePin = A1; // MQ-4 Analog pin connected to A1
const int peltierPin = 9;   // MOSFET Gate

// Define your pump on terminal M1 and fan on terminal M2
AF_DCMotor pump(1); 
AF_DCMotor fan(2);

// Dynamic Control Variables
float setTemperature = 0.0;   // Starts at 0, will be overwritten by LabVIEW
bool hasSetpoint = false;     // Safety flag: locks Peltier until LabVIEW talks to us
int peltierStatus = 0;        // 0 = OFF, 1 = ON

void setup() {
  Serial.begin(9600); 
  Serial.setTimeout(50); // Short timeout so reading serial doesn't stall the loop 
  dht.begin();

  pinMode(peltierPin, OUTPUT);
  digitalWrite(peltierPin, LOW); // Keep Peltier safely OFF at startup

  // 1. Set the operating speeds (0 = Off, 255 = Maximum Speed)
  pump.setSpeed(200); 
  fan.setSpeed(255);  // Set fan to 100% full throttle
  
  // 2. Turn both components ON permanently right at startup
  pump.run(FORWARD); 
  fan.run(FORWARD);
}

void loop() {
  // Constantly look for the live Set Temperature string from LabVIEW
  if (Serial.available() > 0) {
    float incomingTarget = Serial.parseFloat();
    
    // Quick validation check to ensure it's a real temperature reading
    if (incomingTarget > 0.0 && incomingTarget < 50.0) {
      setTemperature = incomingTarget;
      hasSetpoint = true; // LabVIEW has successfully sent data! Unlock the Peltier.
    }
  }

  // Read values from your DHT sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Read raw methane data (0 to 1023)
  int rawMethane = analogRead(methanePin);


  // Check if any readings failed and exit early (to try again)
  if (isnan(humidity) || isnan(temperature)) {
    return;
  }

  // Hysteresis Control Logic (Only runs if LabVIEW has sent a setpoint)
  if (hasSetpoint) {
    if (temperature > (setTemperature + 0.5)) {
      digitalWrite(peltierPin, HIGH); // Too hot! Turn Peltier ON
      peltierStatus = 1;
    } 
    else if (temperature < (setTemperature - 0.5)) {
      digitalWrite(peltierPin, LOW);  // Cool enough! Turn Peltier OFF
      peltierStatus = 0;
    }
  } else {
    // Safety fallback if LabVIEW isn't running yet
    digitalWrite(peltierPin, LOW);
    peltierStatus = 0;
  }

  // 1. Print the first value (Humidity)
  Serial.print(humidity);
  Serial.print(","); // Separating comma

  // 2. Print the second value (Temperature)
  Serial.print(temperature);
  Serial.print(","); // New separating comma

  // 3. Print the third value (Raw Methane) 
  Serial.print(rawMethane);
  Serial.print(","); // New separating comma

  Serial.print(1);
  Serial.print(",");

  Serial.println(1);

  // Delay to match your system timing (DHT sensors need ~2 seconds)
  delay(200);
}
