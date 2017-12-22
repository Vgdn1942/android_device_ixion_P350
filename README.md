# LineageOS 14.1 device tree for DEXP Ixion P350

DEXP Ixion P350
==============

Basic   | Spec Sheet
-------:|:-------------------------
CPU     | 1.3GHz Quad-Core MT6580
GPU     | Mali-400MP
Memory  | 1GB RAM
Shipped Android Version | 5.1
Storage | 8GB
Battery | 5000 mAh
Display | 5" 1280 x 720 px
Camera  | 8MPx, LED Flash

![DEXP](https://content2.onliner.by/catalog/device/main/28756eabf91d930d247cb06211958125.jpeg "DEXP Ixion P350 Tundra Black")

Working:
- Wi-Fi
- Modem/RIL
- Voice
- Mobile data
- SMS
- USSD
- Bluetooth
- Audio
- Wi-Fi tethering
- Second SIM
- Sensors
- Lights
- USB connection (ADB/MTP/PTP)
- SD-Card

Not working/Bugs:
- Reboots after deepsleep (when screen off) (temporary fix 99wakelock_for_no_reboots script, but the processor does not go to sleep)
- Camera
- GPS
- Video record/play
- Poweroff charge
- YouTube 720p

# Acknowledgements

(in alphabetical order)

* DerTeufel
* ferhung
* fire855
* mishkannn (4pda, fix YouTube 720p)
* varunchitre15
* xen0n
* Zormax (4pda, add EngineerMode)
* and others I might have forgotten to include
