/*
  Simulate engine sensors by setting PIN values.
  Note: if you need to set the pins, don't use digitalWrite, see: https://arduino.stackexchange.com/questions/44531/arduino-esp8266-direct-fast-control-of-the-digital-pins
  Tested on ESP8266 (Wemos D1 Mini V3: https://www.hobbyelectronica.nl/product/wemos-d1-mini-v2/)
  Instructions to setup serial driver and board in Arduino IDE: https://siytek.com/wemos-d1-mini-arduino-wifi/)
  Connect the pins defined below to the corresponsing pin on the Engine Hat board. Connect GND to GNS on the SH-ESP32.
  Toggle the tests (RPM and/or alarms) below to true to be included in the simulation.
*/

#define PIN_RPM 12 // D6 is Hz (*60 = rpm)
#define PIN_TEMP_ALARM 13 // D7 is overheating alarm
#define PIN_OIL_ALARM 14 // D5 is oil pressure alarm

int rpm_low = 600;
int rpm_high = 2500;
//float multiplier = 0.105; // the multiplier as set by the engine hat, counter it.
//int revs_low = rpm_low / 60 / multiplier / 2; // divided by 60 sec, and by 2 (because on and off in one cycle)
//int revs_high = rpm_high / 60 / multiplier / 2;
int revs_low = rpm_low / 60; // divided by 60 sec
int revs_high = rpm_high / 60;
int delayLow = 1000000 / revs_low / 2; // 1 miljoen microsecond in a second and divided by 2 for high and low signal
int delayHigh = 1000000 / revs_high / 2;
int counter = 0;
int timeTo2ndEvent = revs_low * 10; // The number of second to transition to a second event
int timeTo3rdEvent = revs_high * 20;

// Enable tests by setting them to true
bool testRPM = false;
bool testAlarms = true;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // Find pins on Wemos D1 mini: https://www.hobbyelectronica.nl/product/wemos-d1-mini-v2/
  //pinMode(16, OUTPUT); // D0
  pinMode(PIN_OIL_ALARM, OUTPUT);
  pinMode(PIN_TEMP_ALARM, OUTPUT);
  pinMode(PIN_RPM, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("rpms low: ");
  Serial.print(rpm_low);
  Serial.print(" / revs low: ");
  Serial.println(revs_low);
  Serial.print("rpms high: ");
  Serial.print(rpm_high);
  Serial.print(" / revs high: ");
  Serial.print(revs_high);
  Serial.print(" / interval: ");
  Serial.println(delayLow);

  analogWriteFreq(revs_low);
}

// the loop function runs over and over again forever
void loop() {
  // Test RPMs
  /* old on/off code by using digitalWrite, instead use PWM with analogWrite
    if (testRPM == true) {
    if (counter < timeTo2ndEvent) {
      digitalWrite(PIN_RPM, HIGH);
      delayMicroseconds(delayLow);
      digitalWrite(PIN_RPM, LOW);
      delayMicroseconds(delayLow);
    } else {
      digitalWrite(PIN_RPM, HIGH);
      delayMicroseconds(delayHigh);
      digitalWrite(PIN_RPM, LOW);
      delayMicroseconds(delayHigh);
    }
    } */
  if (testRPM == true) {
    if (counter == 0) {
      analogWriteFreq(revs_low);
    }
    if (counter == timeTo2ndEvent) {
      analogWriteFreq(revs_high);
    }
    analogWrite(PIN_RPM, 127); // PWN: write half duty (255/2=127) to PIN (=on/off)
  }

  counter++;
  if (testAlarms == true) {
    if (counter > timeTo3rdEvent) {
      // set alarms (engine rpm will drop)
      // Overheating
      Serial.println("ALARM: engine is overheating!");
      digitalWrite(PIN_TEMP_ALARM, HIGH);
      delay(5000);
      digitalWrite(PIN_TEMP_ALARM, LOW);
      Serial.println("Overheating alarm cleared.");
      delay(1000);
      Serial.println("ALARM: oil pressure low!");
      digitalWrite(PIN_OIL_ALARM, HIGH);
      delay(5000);
      digitalWrite(PIN_OIL_ALARM, LOW);
      Serial.println("Oil pressure alarm cleared.");
      delay(1000);
      Serial.println("ALARM: oil pressure low AND engine overheating!");
      digitalWrite(PIN_TEMP_ALARM, HIGH);
      digitalWrite(PIN_OIL_ALARM, HIGH);
      delay(5000);
      digitalWrite(PIN_TEMP_ALARM, LOW);
      digitalWrite(PIN_OIL_ALARM, LOW);
      Serial.println("Oil pressure alarm cleared.");
      delay(1000);
      counter = 0;
    }
  }
  delay (1);
}
