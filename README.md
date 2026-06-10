# ESP8266 Race Car Controller

A PlatformIO-based firmware for an ESP8266 remote-controlled car. It serves a sleek, responsive web interface to control steering (via Servo) and throttle (via DC motor with PWM) over Wi-Fi.

## Features
- **Standalone Wi-Fi Access Point**: Connect directly to the car's network (`RaceCar_AP`).
- **mDNS Support**: Access the control interface conveniently at `http://racecar.local`.
- **Real-Time WebSocket Control**: Low-latency communication for immediate driving response.
- **Modern Web Interface**: Responsive UI with both Joystick and Slider control modes, designed to work perfectly on mobile devices.
- **Persistent Settings**: Calibration for servo center, steering limits, and max motor speed are saved directly to the ESP8266's EEPROM.

## Hardware Requirements
- ESP8266 (configured for `d1_mini` by default)
- Steering Servo
- DC Motor Driver (e.g., L298N, MX1508, or similar)
- Suitable power supply for the ESP8266, servo, and motors

## Software Dependencies
- [PlatformIO](https://platformio.org/)
- `links2004/WebSockets` (installed automatically via `platformio.ini`)
- Arduino Core for ESP8266

## Setup & Usage
1. Clone this repository and open it in PlatformIO.
2. Build and upload the firmware to your ESP8266.
3. Once the ESP8266 boots, connect your device (phone, tablet, or laptop) to the new Wi-Fi network:
   - **SSID**: `RaceCar_AP`
   - **Password**: `12345678`
4. Open a web browser and navigate to `http://racecar.local` (or the AP IP address, usually `http://192.168.4.1`).
5. Use the settings menu (⚙️) to calibrate your steering center, limits, and max motor speed.

## Web Interface Control Modes
- **Joystick**: Provides a virtual thumbstick for simultaneous steering and throttle control.
- **Sliders**: Separate vertical (throttle) and horizontal (steering) sliders with snap-to-center functionality.

## Project Structure
- `platformio.ini`: PlatformIO configuration and library dependencies.
- `src/main.cpp`: Main firmware logic (Setup, Loop, WebServer, WebSockets, EEPROM handling).
- `src/WebPage.h`: The HTML, CSS, and JavaScript for the web interface, stored as a raw string literal to be served directly from memory.
