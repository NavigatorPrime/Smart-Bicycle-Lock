# 🚲 Smart Anti-Theft Bicycle Lock

> **An Arduino-based smart bicycle security system that detects unauthorized movement and provides real-time location alerts using GPS and GSM technology.**

---

## 📌 Project Overview

The **Smart Anti-Theft Bicycle Lock** is an individual embedded-systems project developed to create a portable and intelligent security system for bicycles.

Traditional bicycle locks primarily provide physical protection. However, once a bicycle is stolen, recovering it can be difficult because there is usually no way to determine its current location.

This project addresses that problem by combining:

* 📍 **GPS tracking**
* 📱 **GSM communication**
* 📐 **Motion and tilt detection**
* 🗺️ **Location-based theft detection**
* 🚨 **Real-time SMS alerts**
* 🌐 **GPRS/HTTP communication**

The system monitors the bicycle continuously after it is armed. If the bicycle experiences suspicious movement or moves significantly from its parked location, the system identifies a potential theft event and sends an alert to the owner.

The alert contains the bicycle's current GPS coordinates along with a Google Maps link, allowing the owner to locate the bicycle.

---

# 🎯 Project Objectives

The main objectives of this project are:

1. Detect unauthorized bicycle movement.
2. Detect sudden motion and tilting using an accelerometer.
3. Determine the bicycle's location using GPS.
4. Compare the current location with the original parked location.
5. Send an immediate SMS alert when theft is detected.
6. Generate a Google Maps location link from GPS coordinates.
7. Investigate the use of GSM/GPRS for Internet-based communication.
8. Develop a portable, low-cost embedded security system.

---

# ✨ Features

## 📍 Real-Time GPS Tracking

The GPS module continuously provides the bicycle's geographical coordinates.

The system obtains:

* Latitude
* Longitude
* GPS fix status

These coordinates are used for movement detection and location reporting.

---

## 🗺️ Geofencing / Location Change Detection

When the system is armed, the current bicycle location is stored as the reference position.

The system then calculates the distance between:

```text
Reference Location
        ↓
Current Location
```

If the distance exceeds a predefined threshold, the system considers the bicycle to have moved.

This approach is more reliable than simply comparing latitude and longitude values because GPS naturally experiences small position fluctuations.

---

## 📐 Motion & Tilt Detection

An **ADXL345 3-axis accelerometer** is used to detect:

* Sudden movement
* Shaking
* Tilting
* Lifting
* Physical tampering

This provides an additional layer of protection before a significant GPS displacement occurs.

---

## 📱 Instant SMS Alerts

When unauthorized movement is detected, the Arduino communicates with the SIM800L GSM module.

The owner receives an SMS containing the current location.

Example:

```text
🚨 ALERT! Bike is moving!

Location:
https://maps.google.com/?q=6.9271,79.8612
```

---

## 🔗 Google Maps Integration

The GPS coordinates can be converted into a Google Maps query:

```text
https://maps.google.com/?q=LATITUDE,LONGITUDE
```

For example:

```text
https://maps.google.com/?q=6.9271,79.8612
```

Opening this link allows the owner to view the reported position on Google Maps.

---

## 🌐 GPRS / HTTP Communication

During development, the project also explored using the SIM800L's **GPRS capability to communicate with Internet servers through HTTP requests**.

This created the possibility of extending the project from SMS-based alerts to:

* Cloud logging
* Web dashboards
* Mobile applications
* Remote monitoring
* Location history

---

# 🧠 System Architecture

```text
                         ┌───────────────────┐
                         │     NEO-6M GPS    │
                         │  Latitude/Long.   │
                         └─────────┬─────────┘
                                   │
                                   │ GPS Data
                                   ▼
┌──────────────────┐       ┌─────────────────────┐
│     ADXL345      │──────▶│      Arduino        │
│ Accelerometer    │ I²C   │  Main Controller    │
└──────────────────┘       └──────────┬──────────┘
                                      │
                                      │ Serial
                                      ▼
                              ┌──────────────────┐
                              │     SIM800L      │
                              │   GSM / GPRS     │
                              └────────┬─────────┘
                                       │
                         ┌─────────────┴─────────────┐
                         │                           │
                         ▼                           ▼
                  ┌─────────────┐             ┌─────────────┐
                  │ SMS Alert   │             │ HTTP/GPRS   │
                  └──────┬──────┘             └──────┬──────┘
                         │                           │
                         ▼                           ▼
                  ┌─────────────┐             ┌─────────────┐
                  │ Owner Phone │             │ Web Server  │
                  └─────────────┘             └─────────────┘
```

---

# 🔄 System Operation

## 1. 🔌 System Initialization

When the device is powered on:

```text
Power ON
   ↓
Initialize Arduino
   ↓
Initialize ADXL345
   ↓
Initialize GPS
   ↓
Initialize SIM800L
```

The system checks whether the required modules are communicating correctly.

---

## 2. 📍 Obtain GPS Fix

The system waits until the GPS module provides a valid position.

```text
GPS Data Available?
       │
   ┌───┴───┐
   │       │
  NO      YES
   │       │
Wait       ↓
       Store Position
```

---

## 3. 🔐 Arm the System

Once a valid GPS position is available, the current location is stored as the parked/reference location.

```text
Reference Latitude
Reference Longitude
```

The system then enters the monitoring state.

---

## 4. 👀 Continuous Monitoring

The Arduino continuously monitors:

* GPS coordinates
* Accelerometer readings
* Movement
* Tilt
* GSM status

---

## 5. 🚨 Theft Detection

The system uses two primary detection mechanisms.

### Method 1 — Accelerometer Detection

```text
ADXL345
   ↓
Acceleration / Tilt Change
   ↓
Movement Threshold
   ↓
Suspicious Activity
```

### Method 2 — GPS Detection

```text
Current GPS Position
        ↓
Calculate Distance
        ↓
Compare With Threshold
        ↓
Distance > Threshold?
      /        \
    YES         NO
     ↓           ↓
  ALERT       Continue
```

---

## 6. 📲 Send Alert

Once unauthorized movement is detected:

```text
Theft Detected
      ↓
Obtain Current GPS Coordinates
      ↓
Generate Location Link
      ↓
SIM800L
      ↓
SMS
      ↓
Owner
```

---

# 🛠️ Hardware Requirements

| Component                     | Purpose                         |
| ----------------------------- | ------------------------------- |
| Arduino Uno / Nano / Mega     | Main controller                 |
| SIM800L GSM Module            | GSM, SMS and GPRS communication |
| NEO-6M GPS Module             | GPS positioning                 |
| ADXL345 Accelerometer         | Motion and tilt detection       |
| 3.7V Li-ion Battery           | Portable power                  |
| Voltage Regulator / Converter | Power management                |
| Breadboard                    | Prototyping                     |
| Jumper Wires                  | Connections                     |

---

# 🔌 Hardware Connections

## Arduino ↔ SIM800L

Example configuration:

| SIM800L | Arduino                   |
| ------- | ------------------------- |
| TX      | Digital Pin 7             |
| RX      | Digital Pin 8             |
| GND     | GND                       |
| VCC     | External regulated supply |

> ⚠️ **Important:** SIM800L modules can require high peak current during GSM transmission. A stable power source capable of handling the module's peak current is essential.

---

## Arduino ↔ GPS

Example configuration:

| GPS | Arduino                           |
| --- | --------------------------------- |
| TX  | Digital Pin 9                     |
| RX  | Digital Pin 10                    |
| GND | GND                               |
| VCC | According to module specification |

---

## Arduino ↔ ADXL345

For an Arduino Uno/Nano:

| ADXL345 | Arduino                                   |
| ------- | ----------------------------------------- |
| SDA     | A4                                        |
| SCL     | A5                                        |
| GND     | GND                                       |
| VCC/VIN | According to breakout-board specification |

The ADXL345 communicates with the Arduino using the **I²C protocol**.

---

# 📚 Software & Libraries

The project is developed using the **Arduino IDE**.

## Main Libraries

The following libraries are used or investigated during development:

```text
TinyGPS++
TinyGSM
Adafruit ADXL345
Adafruit Unified Sensor
SoftwareSerial
```

### TinyGPS++

Used to parse GPS NMEA data received from the GPS module.

### TinyGSM

Used as a high-level interface for GSM modem communication and to simplify operations such as:

* Network registration
* GPRS connectivity
* TCP/IP communication
* HTTP communication
* SMS functionality

### Adafruit ADXL345

Used to communicate with the ADXL345 accelerometer.

### Adafruit Unified Sensor

Provides the sensor abstraction required by the Adafruit sensor library ecosystem.

### SoftwareSerial

Used to create additional serial communication interfaces on Arduino boards that have limited hardware serial ports.

---

# 🧩 Challenges & Problem Solving

One of the most valuable aspects of this project was not simply getting the hardware to work, but figuring out **how the different technologies should communicate with each other**.

The project involved considerable research, experimentation, debugging, and library selection.

---

## 📡 1. Finding a Method to Send HTTP Requests Through the SIM800L

One of the major challenges was understanding how to send **HTTP requests through the SIM800L**.

Initially, it was straightforward to find examples for sending SMS messages using the module. However, sending data to an Internet server was considerably more complicated.

Unlike a Wi-Fi module, the SIM800L requires a cellular data connection before Internet communication can take place.

The communication path is:

```text
Arduino
   ↓
Serial Communication
   ↓
SIM800L
   ↓
GSM Network
   ↓
GPRS
   ↓
Internet
   ↓
HTTP Server
```

Understanding this complete communication chain was an important part of the project.

---

# 🔎 2. Understanding SIM800L AT Commands

The SIM800L is controlled using **AT commands**.

These commands are used for operations such as:

```text
Network registration
SIM status
SMS
GPRS
TCP/IP
HTTP
DNS
Internet connectivity
```

Initially, understanding the large number of available commands was challenging.

An HTTP communication sequence can involve multiple steps:

```text
Initialize Modem
       ↓
Check Network
       ↓
Configure APN
       ↓
Activate GPRS
       ↓
Configure HTTP
       ↓
Set URL
       ↓
Send HTTP Request
       ↓
Read Response
       ↓
Close Connection
```

Implementing all of this manually using AT commands can make the Arduino program complex.

---

# 📚 3. Choosing Which Library to Use

Another major challenge was deciding **which library was actually appropriate**.

There are many Arduino libraries, GitHub repositories, tutorials, and examples for SIM800/SIM900 modules.

Some approaches were based on:

* Raw AT commands
* `SoftwareSerial`
* GSM libraries
* `TinyGSM`
* TCP/IP communication
* HTTP libraries
* GPRS-specific implementations

The large number of available options made it difficult to immediately determine which solution would be the most reliable for the project.

The library selection was therefore based on:

* Hardware compatibility
* Arduino compatibility
* SIM800L support
* GPRS support
* HTTP support
* SMS support
* Documentation
* Community examples
* Ease of debugging
* Flexibility

After investigating the available approaches, **TinyGSM** was selected as the high-level GSM communication library.

---

# 🧪 4. Testing the Communication Step by Step

Instead of attempting to integrate everything at once, the system was divided into smaller tests.

```text
Serial Communication
        ↓
Detect SIM800L
        ↓
Check SIM Card
        ↓
Check Network
        ↓
Test SMS
        ↓
Establish GPRS
        ↓
Test Internet
        ↓
Test HTTP Request
        ↓
Integrate GPS
        ↓
Integrate Theft Detection
```

This approach made it easier to identify the source of communication failures.

---

# 🌐 5. Understanding APN Configuration

Another challenge was understanding that simply inserting a SIM card into the GSM module is not enough for Internet communication.

For GPRS communication, the correct **Access Point Name (APN)** must be configured.

The communication process is:

```text
SIM Card
   ↓
Cellular Network
   ↓
APN Configuration
   ↓
GPRS Connection
   ↓
Internet
   ↓
HTTP Request
```

This became an important configuration step when investigating HTTP communication.

---

# 📍 6. Integrating GPS and GSM

Another challenge was combining the GPS and GSM systems into one Arduino application.

The GPS continuously produces NMEA data while the GSM module requires serial communication.

At the same time, the Arduino must process the accelerometer readings and run the theft-detection logic.

Therefore, the controller has to coordinate:

```text
GPS Data
   +
ADXL345 Data
   +
Theft Detection
   +
GSM Communication
```

The resulting process is:

```text
GPS Coordinates
      ↓
Movement Detection
      ↓
Theft Detected
      ↓
Retrieve Current Location
      ↓
SIM800L
      ↓
SMS / HTTP
```

---

# ⚡ 7. SIM800L Power Supply

The SIM800L also introduced a hardware-related challenge.

GSM transmission can cause significant instantaneous current demand.

Therefore, a circuit that appears to work during basic testing may become unstable when the GSM module attempts to connect to the network or transmit data.

This demonstrated the importance of considering:

* Voltage stability
* Peak current
* Battery capability
* Regulator selection
* Wiring
* Ground connections

The project therefore required both **software debugging and hardware-level debugging**.

---

# 🧠 Engineering Lessons Learned

This project provided several important practical lessons.

### 1. Understand the hardware before choosing the library

A library is much easier to use when the underlying communication process is understood.

### 2. Don't select a library simply because it appears in the first tutorial

The library needs to match the actual hardware and requirements.

### 3. Test individual modules before system integration

Testing GPS, GSM, and ADXL345 independently significantly simplifies troubleshooting.

### 4. Break complicated problems into layers

Instead of thinking:

```text
Bicycle → Internet
```

the problem can be divided into:

```text
Arduino
   ↓
Serial
   ↓
SIM800L
   ↓
GSM Network
   ↓
GPRS
   ↓
Internet
   ↓
HTTP Server
```

### 5. Hardware and software problems can look very similar

A failed GSM connection could be caused by:

* Incorrect code
* Wrong baud rate
* Incorrect AT commands
* Poor power supply
* SIM configuration
* Network availability
* APN configuration
* Library configuration

This project helped develop a more systematic debugging approach.

---

# ⚙️ Installation & Setup

## 1. Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/Smart-Anti-Theft-Bicycle-Lock.git
```

Navigate to the project directory:

```bash
cd Smart-Anti-Theft-Bicycle-Lock
```

---

## 2. Open the Arduino Project

Open:

```text
src/Smart_Bicycle_Lock.ino
```

using the Arduino IDE.

---

## 3. Install Required Libraries

Open:

```text
Arduino IDE
    ↓
Sketch
    ↓
Include Library
    ↓
Manage Libraries
```

Install the required libraries.

---

## 4. Configure the Owner's Phone Number

The phone number should be configured inside the Arduino code.

Example:

```cpp
const char PHONE_NUMBER[] = "+94XXXXXXXXX";
```

Replace the placeholder with the required phone number.

> ⚠️ Do not commit personal phone numbers or other private information to a public GitHub repository.

---

## 5. Configure the APN

If GPRS/HTTP communication is used, configure the APN according to the SIM card's mobile network.

Example:

```cpp
const char APN[] = "YOUR_APN";
```

The exact APN depends on the cellular network provider.

---

## 6. Configure the Movement Threshold

A GPS movement threshold can be defined in the firmware.

For example:

```cpp
const float GPS_MOVEMENT_THRESHOLD = 5.0;
```

The value should be selected according to the GPS accuracy and testing environment.

---

## 7. Upload the Firmware

1. Connect the Arduino to the computer.
2. Select the correct board.
3. Select the correct COM port.
4. Compile the program.
5. Upload the firmware.
6. Open Serial Monitor for debugging information.

---

# 🧪 Testing

## GPS Test

Check that:

* GPS module powers on.
* GPS data is received.
* Latitude is valid.
* Longitude is valid.
* A satellite fix is obtained.

---

## GSM Test

Check:

* SIM card detection.
* Network registration.
* Signal availability.
* SMS functionality.
* GPRS connectivity if required.

---

## ADXL345 Test

Check:

* X-axis acceleration
* Y-axis acceleration
* Z-axis acceleration
* Tilt
* Sudden movement

---

## Complete System Test

```text
Power ON
   ↓
Initialize Modules
   ↓
Obtain GPS Fix
   ↓
Store Parked Location
   ↓
ARM SYSTEM
   ↓
Monitor GPS + ADXL345
   ↓
Movement Detected?
   │
   ├── NO → Continue Monitoring
   │
   └── YES
          ↓
    Get Current GPS
          ↓
    Generate Location
          ↓
       Send SMS
          ↓
     Owner Alert
```

---

# 📁 Repository Structure

A recommended repository structure is:

```text
Smart-Anti-Theft-Bicycle-Lock/
│
├── README.md
│
├── src/
│   └── Smart_Bicycle_Lock.ino
│
├── hardware/
│   ├── circuit_diagram.png
│   ├── wiring_diagram.png
│   └── pin_configuration.md
│
├── documentation/
│   ├── project_report.pdf
│   └── system_architecture.png
│
├── images/
│   ├── prototype.jpg
│   ├── circuit.jpg
│   ├── gps_module.jpg
│   ├── gsm_module.jpg
│   └── testing.jpg
│
├── test/
│   ├── gps_test.ino
│   ├── gsm_test.ino
│   └── adxl345_test.ino
│
├── simulation/
│   └── simulation_files/
│
├── LICENSE
└── .gitignore
```

---

# 📂 File Description

| File / Folder    | Description                                |
| ---------------- | ------------------------------------------ |
| `README.md`      | Main project documentation                 |
| `src/`           | Main Arduino firmware                      |
| `hardware/`      | Circuit diagrams and wiring information    |
| `documentation/` | Project report and technical documentation |
| `images/`        | Prototype and testing photographs          |
| `test/`          | Individual module testing programs         |
| `simulation/`    | Simulation files if available              |
| `LICENSE`        | Open-source license                        |
| `.gitignore`     | Files excluded from Git                    |

---

# 📸 Project Documentation

It is recommended to include photographs of the actual prototype.

Suggested images:

```text
images/
├── prototype.jpg
├── circuit.jpg
├── gps_module.jpg
├── gsm_module.jpg
├── accelerometer.jpg
└── testing.jpg
```

The main prototype can be displayed in the README using:

```markdown
![Smart Anti-Theft Bicycle Lock](images/prototype.jpg)
```

---

# 📊 Project Status

| Feature                       | Status                        |
| ----------------------------- | ----------------------------- |
| Arduino Controller            | ✅ Completed                   |
| GPS Tracking                  | ✅ Implemented                 |
| GPS Location Change Detection | ✅ Implemented                 |
| ADXL345 Motion Detection      | ✅ Implemented                 |
| GSM Communication             | ✅ Implemented                 |
| SMS Alert                     | ✅ Implemented                 |
| Google Maps Location Link     | ✅ Implemented                 |
| GPRS Communication            | 🔄 Under Development / Tested |
| HTTP Communication            | 🔄 Investigated / Tested      |
| Battery-Powered Operation     | 🔄 Prototype                  |
| Mobile Application            | 🔮 Future                     |
| Cloud Dashboard               | 🔮 Future                     |
| Live Location Tracking        | 🔮 Future                     |

---

# ⚠️ Limitations

Although the prototype demonstrates the core concept, several limitations remain.

### GPS Accuracy

GPS accuracy can be affected by:

* Buildings
* Trees
* Weather
* Satellite visibility
* Indoor environments

Small coordinate variations may occur even when the bicycle is stationary.

---

### GSM Network

The system depends on cellular network availability.

If the bicycle is located in an area without sufficient GSM coverage, SMS or GPRS communication may fail.

---

### Battery Consumption

GPS and GSM modules can consume considerable power when operating continuously.

Battery optimization would therefore be important for a production version.

---

### SIM800L Availability

SIM800L uses older 2G cellular technology, so its practicality depends on the availability of compatible cellular networks.

A future version could use a newer cellular modem supporting technologies such as LTE-M, NB-IoT, or 4G depending on regional network availability.

---

### False Detection

Poorly selected motion or GPS thresholds can result in:

* False alarms
* Missed movement
* GPS drift triggering an alert

Therefore, threshold calibration is important.

---

# 🚀 Future Improvements

## 📱 Mobile Application

A dedicated mobile application could provide:

* Live bicycle location
* Theft notifications
* Battery status
* Device status
* Location history
* Remote arming/disarming

---

## ☁️ Cloud-Based Tracking

The GSM module could send GPS information to a cloud server.

```text
Bicycle
   ↓
SIM800L
   ↓
GPRS
   ↓
Internet
   ↓
Cloud Server
   ↓
Database
   ↓
Mobile / Web Dashboard
```

This would allow the owner to view historical movement data.

---

## 🔊 Audible Alarm

A buzzer or high-power alarm could be added.

```text
Motion Detected
      ↓
Verify Theft
      ↓
Activate Buzzer
      ↓
Send GSM Alert
```

This would provide both local and remote theft notification.

---

## 🔋 Battery Monitoring

A voltage/current sensor could be added to estimate the remaining battery capacity.

The system could then send:

```text
⚠️ Low Battery
```

to the owner.

---

## 🔐 Owner Authentication

Possible authentication mechanisms include:

* RFID
* Bluetooth
* PIN
* Smartphone authentication
* Fingerprint authentication

This would help distinguish legitimate movement from theft.

---

## 📡 Improved Positioning

Future versions could combine GPS with other positioning technologies:

* Assisted GPS
* Cellular positioning
* Wi-Fi positioning
* Multi-sensor localization

This could improve location availability in difficult environments.

---

# 🔒 Security Considerations

A production version should consider:

* Secure communication
* Authentication
* Encrypted data transmission
* Protected configuration
* Secure firmware
* Tamper detection
* Unauthorized device access prevention

Sensitive information such as phone numbers, APNs, passwords, API keys, or server credentials should not be committed to a public repository.

---

# 📜 Example `.gitignore`

A `.gitignore` file can be used to prevent sensitive or unnecessary files from being uploaded.

```gitignore
# Arduino build files
*.hex
*.bin
*.elf

# Temporary files
*.tmp
*.log

# IDE files
.vscode/
.idea/

# OS files
.DS_Store
Thumbs.db

# Local configuration
config.h
secrets.h
.env

# Personal test data
private/
```

---

# 📜 License

This project is intended primarily for educational and personal development purposes.

If you would like to make the project open-source, an appropriate license such as the **MIT License** can be added to the repository.

---

# 👨‍💻 Author

**Inuka**

Individual Academic / Hobby Project

---

# ⭐ Acknowledgements

This project makes use of open-source software, libraries, and embedded technologies including:

* Arduino ecosystem
* TinyGPS++
* TinyGSM
* Adafruit Sensor Libraries
* SIM800L GSM technology
* NEO-6M GPS technology
* ADXL345 accelerometer

---

# 💡 Final Project Concept

The core concept of the Smart Anti-Theft Bicycle Lock is:

```text
        DETECT
           ↓
    Unauthorized
       Movement
           ↓
        LOCATE
           ↓
       GPS Position
           ↓
        CONNECT
           ↓
       GSM / GPRS
           ↓
        NOTIFY
           ↓
      Owner / Server
```

The project demonstrates how relatively low-cost embedded hardware can be combined to create a practical security system.

More importantly, the development process provided hands-on experience in **embedded programming, sensor integration, GPS processing, GSM communication, GPRS networking, HTTP communication, library selection, serial communication, power management, and systematic debugging**.
