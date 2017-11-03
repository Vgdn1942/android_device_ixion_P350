# LineageOS 13.0 device tree for DEXP Ixion P350

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
- Bluetooth
- Audio
- Modem
- SMS
- Voice
- USSD
- Mobile data
- Sensors
- Lights
- Camera (only photos)
- USB connection (ADB/MTP/PTP)
- SDcard

Not tested:
- GPS
- Wi-Fi tethering
- Second SIM

Not working/Bugs:
- Reboots after deepsleep (when screen off) (temporary fix 99wakelock_for_no_reboots script, but the processor does not go to sleep)
- Video recording
- Poweroff charge

# Acknowledgements

(in alphabetical order)

* aiden09
* Besik13
* ccfxny
* dee3000
* DerTeufel
* ferhung
* fire855
* hyperion70
* varunchitre15
* xen0n
* Zormax
* others I might have forgotten to include
