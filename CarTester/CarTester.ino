/*
 Name:		CarTester.ino
 Created:	14.01.2019 18:47:02
 Author:	sorokovikov
*/

//  RST     -|------|-  VCC
//  BUZZER  -|ATTiny|-  PROBE
//  VOLTAGE -|  85  |-  PULSE
//  GND     -|------|-  DISPLAY

//#define DEBUG

#include <WS2812.h>

#ifdef DEBUG
#include <TinyDebugSerial.h>
#endif


#define MULTIPLIER 56.57
#define VOLTAGE_AVERAGE 10
#define DELAY_VOLTAGE_PROBE 50
#define DELAY_DISPLAY 500
#define DELAY_ANIMATION 50
#define LIGHTNESS 127

#define PIN_VOLTAGE A2        // ATTiny85 pin 3
#define PIN_DISPLAY 0         // ATTiny85 pin 5
#define PIN_PULSE   1         // ATTiny85 pin 6
#define PIN_PROBE   A1        // ATTiny85 pin 7
#define PIN_TONE    3         // ATtiny85 pin 2

#define LEDS_COUNT 8

#ifdef DEBUG
  TinyDebugSerial mySerial = TinyDebugSerial(); // ATTiny85 pin 2
#endif

WS2812 Display(LEDS_COUNT);

unsigned long time_voltage_probe;
unsigned long time_display;
unsigned long time_animation;

float voltage;
bool  positive;
bool  playing;

byte state; // 0 - shortage, 1 - floating, 2 - voltage
// cRGB struct {g, r, b}

cRGB nPixel = { 0,0,0 };
cRGB rPixel = { 0,LIGHTNESS,0 };
cRGB gPixel = { LIGHTNESS,0,0 };

// the setup function runs once when you press reset or power the board
void setup() {
  // OSCCAL = 0x48; // ideal for 1Mhz / 115200
  // OSCCAL = 0x45; // ideal for 8Mhz / 115200
  
#ifdef DEBUG
  mySerial.begin(115200);
#else
  pinMode(PIN_TONE, OUTPUT);
#endif

  Display.setOutput(PIN_DISPLAY);
  ShowNull();

  pinMode(PIN_VOLTAGE, INPUT);
  pinMode(PIN_PROBE, INPUT);
  pinMode(PIN_PULSE, OUTPUT);

  ShowIntro();
}

// the loop function runs over and over again until power down or reset
void loop() {

  if (millis() - time_voltage_probe > DELAY_VOLTAGE_PROBE) {
    time_voltage_probe = millis();

    voltage = (voltage * (VOLTAGE_AVERAGE - 1) + analogRead(PIN_VOLTAGE)) / VOLTAGE_AVERAGE;

    if (voltage < 50) {
      digitalWrite(PIN_PULSE, HIGH);
      delay(10);
      state = digitalRead(PIN_PROBE) ? 0 : 1;
      delay(10);
      digitalWrite(PIN_PULSE, LOW);
    }
    else {
      state = 2;
      positive = digitalRead(PIN_PROBE);
    }
  }

#ifdef DEBUG
  if (millis() - time_display > DELAY_DISPLAY) {
    time_display = millis();

    if (positive) {
      mySerial.print("+");
    }
    else {
      mySerial.print("-");
    }
    mySerial.print(voltage / MULTIPLIER);
    mySerial.print("  ");
    mySerial.print(voltage);
    mySerial.print("  ");

    switch (state) {
    case 0:
      mySerial.println("shortage");
      break;
    case 1:
      mySerial.println("floating");
      break;
    case 2:
      mySerial.println("voltage");
      break;
    }
  }
#endif

  switch (state) {
  case 0:
    ShowShortage();
    playing = true;
    break;
  case 1:
    ShowNull();
    playing = false;
    break;
  case 2:
    ShowVoltage(voltage, positive);
    playing = false;
    break;
  }

#ifndef DEBUG
  loop_player();
#endif

}
