## Microcontroller and Sensing Submodule as well as UI and aBackend for Monitoring Southern Yellow-Billed Hornbill Nests

This repository contains the design and implementation of a microcontroller and sensing submodule for monitoring and controlling the temperature of Southern Yellow-Billed Hornbill nests in the Kalahari Desert.

### Features

- Reliable microcontroller (ESP32-CAM) that can survive brownouts/blackouts and continue operation when power is restored
- Monitors battery health by taking voltage readings every 5 minutes within a 5% tolerance
- Samples temperature and humidity sensors (DHT22) every 5 minutes, providing temperature readings within ±1°C and reasonably accurate humidity values
- Stores over 2 months worth of sensor data in non-volatile memory (SPIFFS) and allows retrieval
- Handles noisy and irregular data using a Kalman filter
- Scalable design with multiple free GPIO pins for future sensor additions
- Captures high-resolution images using the built-in OV2640 camera sensor for nest inspection

### Hardware

- ESP32-CAM microcontroller
- DHT22 digital temperature and humidity sensor
- OV2640 camera sensor

### Software

- Kalman filter implementation for noise suppression
- SPIFFS file system for non-volatile data storage
- Webserver for image retrieval and system monitoring

### Testing

Extensive testing was performed to validate the system's functionality, including:

- Camera testing and results
- Temperature and humidity sensor testing and results
- Voltage sensor testing and results
- Non-functional and functional specifications assessment

### Conclusion

The microcontroller and sensing submodule provides a reliable, efficient, and scalable solution for monitoring the critical environmental parameters of Southern Yellow-Billed Hornbill nests in the Kalahari Desert. Its robust design and implementation ensures the system can operate for extended periods in extreme conditions
