# engine_simulator
Arduino simulator of engine RPM and/or Alarms. Used to test SH-ESP32 and/or its Engine Hat add-on board.

This simulator sets PINs on an Arduino board high in a certain frequency to simulate engine RPMs and alarms for low oil pressure and high cooling water temperature.
This was tested with an [WeMos D1 Mini](https://www.hobbyelectronica.nl/product/wemos-d1-mini-v2/) ESP8266.

GPIO pin mapping available on link above.

The mapping i used:
Yellow on pin 2 (rpm)
Not sure what i did for oil and temp alarm.

WEMOS
D5 bruin - D3 oliealarm
D6 rood - D2 RPM
D7 groen - D1 koelwater
DNG zwart - GND D1