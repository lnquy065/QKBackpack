# QKBackpack
<p align="center"><img src="/readme/app.png" width="700px"/></p>

Smart backpack for your health!.  Attach QKbackpack KIT with your backpack, download QKApp let it protect you.
#### Youtube: https://www.youtube.com/watch?v=nNQT7VV-78U
#### QKApp: https://github.com/lnquy065/QKBackpackApp

## Features
  + Detect and show UV Intensity, CO2 Intensity, Temperature, Humidity.
  + Real-time UV and CO2 map.
  + Vibrating alert when in danger zone.
  + Show danger levels and preventive guidance.
  + Users can view information from the map even when no QKBackpack Kit is needed.
  + Anti-Theft with piezo alarm.
## QKBackpack
### QKBackpack KIT
  + A hardware device, using bluetooth connectivity to send data that has been read from the sensor.
  + Board: Arduino Nano.
  + Sensors: UV (ML8511), Co2 (MQ135), Temperature (DHT11), Humidity (DHT11), Vibration.
  + Modules: RFID (MC522), Bluetooth (HC-05), Piezo alarm.
  <p align="center"><img src="/readme/qkkit.png" width="700px"/></p>
    
### QKBackpack App
  + An Android mobile application, two modes of using app:
    - Paired:
      - Receive data from sensors, attach location taken from gps and time. Then send to the server through the internet if you are in danger.
    - Unpaired:
      - Get data from the server and mark up the earliest dangerous locations (within 60 minutes).
  + Real-time UV and Co2 map.
  + Vibrating alert when in danger zone.
  + Find QKBackpack by alarm.
  + Two styles of map: Normal and Satellite.
  + Map supports filters: Only UV, Only Co2, Both.
  + Show UV intensity, Co2 intensity on map 60 minutes ago.
  + On/Off security mode.
  + On/Off alert mode.
  <p align="center"><img src="/readme/qkapp0.png" width="700px"/></p>
   <p align="center"><img src="/readme/qkapp1.png" width="700px"/></p>
    <p align="center"><img src="/readme/qkapp2.png" width="700px"/></p>
<p align="center"><img src="/readme/app.png" width="700px"/></p>

## How it works
  <p align="center"><img src="/readme/howitworks.png" width="700px"/></p>
  
## Schematic
  <p align="center"><img src="/readme/mach.png" width="700px"/></p>


## About
+ Release date: 04/2018
+ QK team:
  - Lương Ngọc Quý
  - Nguyễn Đăng Khoa
