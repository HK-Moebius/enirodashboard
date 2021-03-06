# KIA ENIRO DASHBOARD

Supported devices
1. LILYGO TTGO T4 v1.3
2. M5STACK CORE1 IOT Development Kit

Working only with electric vehicles (Kia e-NIRO (EV), Hyundai Kona EV, Hyundai Ioniq EV). Vgate iCar Pro Bluetooth 4.0 (BLE4) OBD2 adapter is required. See Release notes, quick installation via flash tool bellow. 

Use it at your own risk!
Author: nick.n17@gmail.com (Lubos Petrovic / Slovakia)

## Supporting me

- Buy Me a Beer via paypal https://www.paypal.me/nickn17
- EU companies can support me via IBAN/Invoice (my company is non-VAT payer in Slovakia).

Many thanks to Blas, Jens, Калин, Aleš Dokupil and others for help. Thank you for supporting me. 

## Required hardware
Board
- M5STACK CORE1 IOT Development Kit(~EUR 35)
  https://rlx.sk/sk/m5stack/7285-esp32-basic-core-iot-development-kit-m5-k001-m5stack.html
  
or 
- LILYGO TTGO T4 v1.3 (~USD $30) https://www.banggood.com/LILYGO-TTGO-T-Watcher-BTC-Ticker-ESP32-For-Bitcoin-Price-Program-4M-SPI-Flash-Psram-LCD-Display-Module-p-1345292.html
I RECOMMEND TO REMOVE LION BATTERY DUE TO HIGH SUMMER TEMPERATURES

OBD2 adapter
- Supported is only this model... Vgate iCar Pro Bluetooth 4.0 (BLE4) OBD2 (~USD $30)

Others
- 3D printed case for TTGO-T4
  https://www.thingiverse.com/thing:3099913

## Quick installation with ESP32 flash tool

M5STACK (Many thanks to DimZen)

https://docs.google.com/document/d/17vJmeveNfN0exQy9wKC-5igU8zzNjsuOn1DPuPV_yJA/edit?usp=sharing

TTGO-T4 (older)

https://docs.google.com/document/d/1nEezrtXY-8X6mQ1hiZVWDjBVse1sXQg1SlnizaRmJwU/edit?usp=sharing

## Screens and shortcuts
- Middle button - menu 
- Left button - toggle screens

Screen list
- no0. blank screen, lcd off
- no1. auto mode (summary info / speed kmh / charging graph)
- no2. summary info (default)
- no3. speed kmh + kwh/100km (or kw for discharge)
- no4. battery cells + battery module temperatures
- no5. charging graph
- no6. consumption table. Can be used to measure available battery capacity! 
- no7. debug screen (default off in the menu)


![image](https://github.com/nickn17/enirodashboard/blob/master/screenshots/v1.jpg)

[![Watch the video](https://github.com/nickn17/enirodashboard/blob/master/screenshots/v0.9.jpg)](https://www.youtube.com/watch?v=Jg5VP2P58Yg&)

### v1.8.3 2020-11-28
- Automatic shutdown when car goes off
- Fixed M5stack speaker noise
- Fixed menu, added scroll support

### v1.8.2 2020-11-25
- Removed screen flickering. (via Sprites, esp32 with SRAM is now required!)
- Code cleaning. Removed force no/yes redraw mode. Not required with sprites
- Arrow for current (based on bat.temperature) pre-drawn charging graph 

### v1.8.1 2020-11-23
- Pre-drawn charging graphs (based on coldgates)
- Show version in menu

### v1.8.0 2020-11-20
- Support for new device m5stack core1 iot development kit
- TTGO T4 is still supported device!

### v1.7.5 2020-11-17
- Settings: Debug screen off/on 
- Settings: LCD brightness (auto, 20, 50, 100%)
- Speed screen: added motor rpm, brake lights indicator
- Soc% to kWh is now calibrated for NiroEV/KonaEV 2020
- eNiroDashboard speed improvements

### v1.7.4 2020-11-12
- Added default screen option to settings
- Initial config for Renault ZOE 22kWh
- ODB response analyzer. Please help community to decode unknown values like BMS valves, heater ON switch,...
  https://docs.google.com/spreadsheets/d/1eT2R8hmsD1hC__9LtnkZ3eDjLcdib9JR-3Myc97jy8M/edit?usp=sharing

### v1.7.3 2020-11-11
- Headlights reminder (if drive mode & headlights are off)

### v1.7.2 2020-11-10
- improved charging graph

### v1.7.1 2020-10-20
- added new screen 1 - auto mode
  - automatically shows screen 3 - speed when speed is >5kph
  - screen 5 chargin graph when power kw > 1kW
- added bat.fan status and fan feedback in Hz for Ioniq

### v1.7 2020-09-16
- added support for 39.2kWh Hyundai Kona and Kia e-Niro
- added initial support for Hyundai Ioniq 28kWh (not working yet)

### v1.6 2020-06-30
- fixed ble device pairing
- added command to set protocol ISO 15765-4 CAN (11 bit ID, 500 kbit/s) - some vgate adapters freezes during "init at command" phase

### v1.5 2020-06-03
- added support for different units (miles, fahrenheits, psi)

### v1.4 2020-05-29
- added menu 
- Pairing with VGATE iCar Pro BLE4 adapter via menu!
- Installation with flash tool. You don't have to install Arduino and compile sources :)
- New screen 5. Conpumption... Can be used to measure available battery capacity!
- Load/Save settings 
- Flip screen vertical option
- Several different improvements

### v1.1 2020-04-12
- added new screens (switch via left button)
- screen 0. (blank screen, lcd off)
- screen 1. (default) summary info
- screen 2. speed kmh + kwh/100km (or kw for discharge)
- screen 3. battery cells + battery module temperatures
- screen 4. charging graph
- added low batery temperature detection for slow charging on 50kW DC (15°C) and UFC >70kW (25°C).

### v1.0 2020-03-23
- first release
- basic dashboard

## About T4
ESP32-TTGO-T4
https://github.com/fdufnews/ESP32-TTGO-T4

## Installation from sources
- install arduino IDE + ESP32 support
- https://github.com/Bodmer/TFT_eSPI  - display library
- Configure TFT eSPI
  W:\Documents\Arduino\libraries\TFT_eSP\User_Setup_Select.h  
```  
// Comment
//#include <User_Setup.h>           // Default setup is root library folder
// And uncomment
#include <User_Setups/Setup22_TTGO_T4_v1.3.h>      // Setup file for ESP32 and TTGO T4 version 1.3
```  

My configuration
- Board ESP32 Dev module
- Upload speed 921600
- CPU freq: 240MHz (Wifi/BT)
- Flash freq: 80MHz
- Flash mode: QIO
- Flash size 4MB (32mb)
- Partion scheme: default 4MB with spiffs
- Core debug level: none
- PSRAM: disable
