This repository is built for my and Tengiz Pataraia's Bachelors Project. It can be found on SDSU website.

The project described in the report is a Smart Radiation Monitoring System, aimed at detecting and warning people about potentially harmful radiation exposure. 
The system is built around a Geiger-Muller counter and utilizes Internet of Things (IoT) technology to allow for remote monitoring and data accessibility. 
The project includes the development of three different device versions: handheld, half-automated, and fully automated, each tested in various conditions.

Key Objectives:

Radiation Monitoring Hardware: The hardware efficiently detects radiation using a Geiger-Muller counter, which offers high efficiency, spatial and time resolution, and resistance to noise.

Software Integration: The software accurately analyzes radiation data using custom algorithms and visualization tools.

Automated Hazard Response: The system generates automated responses, such as warning messages or calls, if hazardous radiation levels are detected.


System Components:

Geiger-Muller Counter: The primary sensor for radiation detection.

ESP32 Microcontroller: For processing data and connecting to the IoT cloud.

GPS and GSM Modules: Provide device location data and enable remote alerts.

Solar Panel and Charging Circuit: Ensure autonomous device operation in remote locations.

IoT Server and Web Application: Data visualization using Grafana and Leaflet for location mapping.


Software:

MQTT Protocol: Facilitates communication between the device and the cloud.

InfluxDB and Grafana: For real-time data storage and visualization.

Django Web Server: Hosts the website where radiation data and device status can be accessed globally.


Achievements:

Developed a smart digital dosimeter that allows monitoring radiation levels from any location.

Successfully automated safety monitoring through IoT integration.

Enabled energy independence of the devices via solar power.


The project transforms a simple radiation dosimeter into a smart, automated, and globally accessible monitoring system, contributing to enhanced safety and awareness of radiation exposure risks. The team aims to continue developing and refining the device for broader applications and improved functionality.
