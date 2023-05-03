// ---------------------------------------------------------------------------
// Cascade 48
// MIDI Capacitive Touch 48 Key Keyboard Firmware
// Cascade Firmware v1.0
// David Cool
// http://davidcool.com
//
// 
//
// Firmware for a MIDI (Musical Instrument Digital Interface) keyboard using an MPR121 touch sensor breakout
// and Arduino Pro Micro 32u4 (Leonardo) and an SSD1306 OLED for a display system
// 
// Uses the MIDIUSB library: https://github.com/arduino-libraries/MIDIUSB
// Uses the Adafruit MPR121 library: https://github.com/adafruit/Adafruit_MPR121
// Uses the Adafruit SSD-1306library: https://github.com/adafruit/Adafruit_SSD1306
//
// ---------------------------------------------------------------------------

#include "MIDIUSB.h"          // MIDIUSB library
#include "Wire.h"             // I2C data bus library
#include "Adafruit_MPR121.h"  // touch sensor library
#include "Controller.h"
#include "Adafruit_GFX.h"     // 
#include "Adafruit_SSD1306.h" // SSD1306 display driver

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     21 // 21=A3 Reset pin # (or -1 if sharing Arduino reset pin), if using -1 this prevents you from uploading code updates!
#define SCREEN_ADDRESS 0x3C // Address is 0x3C for our screen
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Bitmap of david cool logo image
const unsigned char logo [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xf8, 0x00, 0x00, 0x07, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x7f, 0x00, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x07, 0xc0, 0x00, 0xf8, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xc0, 0x01, 0xc0, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1f, 0x83, 0xf0, 0x01, 0xe0, 0x07, 0xe0, 0xfc, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x3c, 0x03, 0x60, 0x0f, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x03, 0x30, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x06, 0x30, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x86, 0x18, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x8c, 0x18, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x18, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x0d, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6c, 0x0d, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0xc7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3b, 0xe7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3b, 0xf6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3b, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0xe7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x0d, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6c, 0x0d, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x09, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xcc, 0x18, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x86, 0x18, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x06, 0x10, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x30, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x1c, 0x03, 0x30, 0x1c, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x78, 0x03, 0xe0, 0x0f, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xe0, 0x01, 0xc0, 0x03, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x01, 0xc0, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x0f, 0x80, 0x00, 0x7c, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x3f, 0x81, 0xfe, 0x00, 0x00, 0x1f, 0xe0, 0xff, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xf0, 0x00, 0x00, 0x03, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// set default 1st level menu variables
byte menu_items = 2;
byte menu_state = 0;
byte menu_count = 0;

// set default "set keys" level menu variables
byte menu_items_keys = 2;
byte menu_state_keys = 0;
byte menu_count_keys = 0;

// set default MIDI channel to 1
// channel 0 = MIDI channel 1 and so on
byte channel1 = 0; // keys 1-24
byte channel2 = 0; // keys 25-48

//---How many buttons are connected directly to pins?---------
byte NUMBER_BUTTONS = 3;
//---How many potentiometers are connected directly to pins?--
byte NUMBER_POTS = 0;
//---How many buttons are connected to a multiplexer?---------
byte NUMBER_MUX_BUTTONS = 0;
//---How many potentiometers are connected to a multiplexer?--
byte NUMBER_MUX_POTS = 14;

// analog mux
Mux M1(A0, 16, true); //Analog multiplexer on Arduino analog pin A0

Pot *POTS[] {};

//***DEFINE DIRECTLY CONNECTED BUTTONS*******************************
//Button (Pin Number, Command, Note Number, Channel, Debounce Time)
//** Command parameter 0=NOTE  1=CC  2=Toggle CC **

Button BU1(9, 1, 60, 1, 5); //Button 1
Button BU2(10, 1, 61, 1, 5); //Button 2
Button BU3(0, 1, 62, 1, 5); //Button 3
Button *BUTTONS[] {&BU1, &BU2, &BU3};

Button *MUXBUTTONS[] {};

//***DEFINE POTENTIOMETERS CONNECTED TO MULTIPLEXER*******************
//Pot::Pot(Mux mux, byte muxpin, byte command, byte control, byte channel)
//**Command parameter is for future use**

Pot MPO01(M1, 0, 0, 45, channel1); //Potentiometer 01
Pot MPO02(M1, 1, 0, 46, channel1); //Potentiometer 02
Pot MPO03(M1, 2, 0, 47, channel1); //Potentiometer 03
Pot MPO04(M1, 3, 0, 48, channel1); //Potentiometer 04
Pot MPO05(M1, 4, 0, 49, channel1); //Potentiometer 05
Pot MPO06(M1, 5, 0, 50, channel1); //Potentiometer 06
Pot MPO07(M1, 6, 0, 51, channel1); //Potentiometer 07
Pot MPO08(M1, 7, 0, 52, channel2); //Potentiometer 08
Pot MPO09(M1, 8, 0, 53, channel2); //Potentiometer 09
Pot MPO10(M1, 9, 0, 54, channel2); //Potentiometer 10
Pot MPO11(M1, 10, 0, 55, channel2); //Potentiometer 11
Pot MPO12(M1, 11, 0, 56, channel2); //Potentiometer 12
Pot MPO13(M1, 12, 0, 57, channel2); //Potentiometer 13
Pot MPO14(M1, 13, 0, 58, channel2); //Potentiometer 14
Pot *MUXPOTS[] {&MPO01, &MPO02, &MPO03, &MPO04, &MPO05, &MPO06, &MPO07, &MPO08, &MPO09, &MPO10, &MPO11, &MPO12, &MPO13, &MPO14};

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

// The MPR121 will use the IRQ to let the controller know something has changed
// *** BE SURE TO CUT THE ADDR TRACE ON THE BACK OF THE BOARD FOR ALL BUT THE FIRST SENSOR!!! ***

#define TOUCH_BUTTONS  12           // defins the number of electrodes on each touch sensor
#define IRQ_PIN1  4           // ADDR not connected is I2C address: 0x5A
#define IRQ_PIN2  5           // ADDR tied to 3V is I2C address: 0x5B
#define IRQ_PIN3  6           // ADDR tied to SDA is I2C address: 0x5C
#define IRQ_PIN4  7           // ADDR tied to SCL is I2C address: 0x5D

// mpr121 init for all 4 sensors
Adafruit_MPR121 cap1 = Adafruit_MPR121();
Adafruit_MPR121 cap2 = Adafruit_MPR121();
Adafruit_MPR121 cap3 = Adafruit_MPR121();
Adafruit_MPR121 cap4 = Adafruit_MPR121();

// keep track of touched keys
//cap1
uint16_t lasttouched1 = 0;
uint16_t currtouched1 = 0;
//cap2
uint16_t lasttouched2 = 0;
uint16_t currtouched2 = 0;
//cap3
uint16_t lasttouched3 = 0;
uint16_t currtouched3 = 0;
//cap4
uint16_t lasttouched4 = 0;
uint16_t currtouched4 = 0;

// MIDI note mapping
// Octave   Note Numbers
//          C   C#  D   D#  E   F   F#  G   G#  A   A#  B
//    0     0   1   2   3   4   5   6   7   8   9   10  11
//    1     12  13  14  15  16  17  18  19  20  21  22  23
//    2     24  25  26  27  28  29  30  31  32  33  34  35
//    3     36  37  38  39  40  41  42  43  44  45  46  47
//    4     48  49  50  51  52  53  54  55  56  57  58  59
//    5     60  61  62  63  64  65  66  67  68  69  70  71
//    6     72  73  74  75  76  77  78  79  80  81  82  83
//    7     84  85  86  87  88  89  90  91  92  93  94  95
//    8     96  97  98  99  100 101 102 103 104 105 106 107
//    9     108 109 110 111 112 113 114 115 116 117 118 119
//    10    120 121 122 123 124 125 126 127

//******* setup all the scales **********
// C3 Major Scale ( C, D, E, F, G, A, B, C )
// F# / Gb Major Scale ( F#, G#, A#, B, C#, D#, F, F# )
// E4 Major Scale ( E, F#, G#, A, B, C#, D#, E )
// Dm Pentatonic Scale ( D, F, G, A, C, D )
// C Major Pentatonic Scale ( C, D, E, G, A, C )
// D Egyptian Suspended Scale ( D, E, G, A, C, D )
// E Blues Minor Scale ( E, G, A, C, D, E )
// G Blues Major Scale ( G, A, C, D, E, G )
// A Minor Pentatonic Scale ( A, C, D, E, G, A )

// const unsigned char *scales[] PROGMEM = { };
char *scales[] = {
                    "C3\n\nMajor", 
                    "F#/Gb\n\nMajor", 
                    "E4\n\nMajor", 
                    "D Minor\n\nPentatonic"
                    /*
                    "C Major\n\nPentatonic",
                    "D Egyptian\n\nSuspended",
                    "E Blues\n\nMajor",
                    "G Blues\n\nMajor",
                    "A Minor\n\nPentatonic"
                    */
                  };

byte pitch[][48] = { 

                    {12, 14, 16, 17, 19, 21, 23, 24, 26, 28, 29, 31, 33, 35, 36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67, 69, 71, 72, 74, 76, 77, 79, 81, 83, 84, 86, 88, 89, 91, 93},
                    {30, 32, 34, 35, 37, 39, 41, 42, 44, 46, 47, 49, 51, 53, 54, 56, 58, 59, 61, 63, 65, 66, 68, 70, 71, 73, 75, 77, 78, 80, 82, 83, 85, 87, 89, 90, 92, 94, 95, 97, 99, 101, 102, 104, 106, 107, 109, 111},
                    {28, 30, 32, 33, 35, 37, 39, 40, 42, 44, 45, 47, 49, 51, 52, 54, 56, 57, 59, 61, 63, 64, 66, 68, 69, 71, 73, 75, 76, 78, 80, 81, 83, 85, 87, 88, 90, 92, 93, 95, 97, 99, 100, 102, 104, 105, 107, 109},
                    {14, 17, 19, 21, 24, 26, 29, 31, 33, 36, 38, 41, 43, 45, 48, 50, 53, 55, 57, 60, 62, 65, 67, 69, 72, 74, 77, 79, 81, 84, 86, 89, 91, 93, 96, 98, 101, 103, 105, 108, 110, 113, 115, 117, 120, 122, 125, 127}
                    /*
                    {12, 14, 16, 19, 21, 24, 26, 28, 31, 33, 36, 38, 40, 43, 45, 48, 50, 52, 55, 57, 60, 62, 64, 67, 69, 72, 74, 76, 79, 81, 84, 86, 88, 91, 93, 96, 98, 100, 103, 105, 108, 110, 112, 115, 117, 120, 122, 124},
                    {14, 16, 19, 21, 24, 26, 28, 31, 33, 36, 38, 40, 43, 45, 48, 50, 52, 55, 57, 60, 62, 64, 67, 69, 72, 74, 76, 79, 81, 84, 86, 88, 91, 93, 96, 98, 100, 103, 105, 108, 110, 112, 115, 117, 120, 122, 124, 127},
                    {2, 4, 7, 9, 12, 14, 16, 19, 21, 24, 26, 28, 31, 33, 36, 50 ,52, 55, 57, 60, 62, 64, 67, 69, 72, 74, 76, 79, 81, 84, 86, 88, 91, 93, 96, 98, 100, 103, 105, 108, 110, 112, 115, 117, 120, 122, 124, 127},
                    {7, 9, 12, 14, 16, 19, 21, 24, 26, 28, 31, 33, 36, 38, 40, 43, 45, 48, 50, 52, 55, 57, 60, 62, 64, 67, 69, 72, 74, 76, 79, 81, 84, 86, 88, 91, 93, 96, 98, 100, 103, 105, 108, 110, 112, 115, 117, 120},
                    {9, 12, 14, 15, 19, 21, 24, 26, 28, 31, 33, 36, 38, 40, 43, 45, 48, 50, 52, 55, 57, 60, 62, 64, 67, 69, 72, 74, 76, 79, 81, 84, 86, 88, 91, 93, 96, 98, 100, 103, 105, 108, 110, 112, 115, 117, 120, 122}
                    */
};
/*
byte pitch[][48] = { 
                    {12, 14, 16, 17, 19, 21, 23, 24, 26, 28, 29, 31, 33, 35, 36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67, 69, 71, 72, 74, 76, 77, 79, 81, 83, 84, 86, 88, 89, 91, 93},
                    {30, 32, 34, 35, 37, 39, 41, 42, 44, 46, 47, 49, 51, 53, 54, 56, 58, 59, 61, 63, 65, 66, 68, 70, 71, 73, 75, 77, 78, 80, 82, 83, 85, 87, 89, 90, 92, 94, 95, 97, 99, 101, 102, 104, 106, 107, 109, 111},
                    {28, 30, 32, 33, 35, 37, 39, 40, 42, 44, 45, 47, 49, 51, 52, 54, 56, 57, 59, 61, 63, 64, 66, 68, 69, 71, 73, 75, 76, 78, 80, 81, 83, 85, 87, 88, 90, 92, 93, 95, 97, 99, 100, 102, 104, 105, 107, 109},
                    {14, 17, 19, 21, 24, 26, 29, 31, 33, 36, 38, 41, 43, 45, 48, 50, 53, 55, 57, 60, 62, 65, 67, 69, 72, 74, 77, 79, 81, 84, 86, 89, 91, 93, 96, 98, 101, 103, 105, 108, 110, 113, 115, 117, 120, 122, 125, 127}
                    
                    {12, 14, 16, 19, 21, 24, 26, 28, 31, 33, 36, 38, 40, 43, 45, 48, 50, 52, 55, 57, 60, 62, 64, 67, 69, 72, 74, 76, 79, 81, 84, 86, 88, 91, 93, 96, 98, 100, 103, 105, 108, 110, 112, 115, 117, 120, 122, 124},
                    {14, 16, 19, 21, 24, 26, 28, 31, 33, 36, 38, 40, 43, 45, 48, 50, 52, 55, 57, 60, 62, 64, 67, 69, 72, 74, 76, 79, 81, 84, 86, 88, 91, 93, 96, 98, 100, 103, 105, 108, 110, 112, 115, 117, 120, 122, 124, 127},
                    {2, 4, 7, 9, 12, 14, 16, 19, 21, 24, 26, 28, 31, 33, 36, 50 ,52, 55, 57, 60, 62, 64, 67, 69, 72, 74, 76, 79, 81, 84, 86, 88, 91, 93, 96, 98, 100, 103, 105, 108, 110, 112, 115, 117, 120, 122, 124, 127},
                    {7, 9, 12, 14, 16, 19, 21, 24, 26, 28, 31, 33, 36, 38, 40, 43, 45, 48, 50, 52, 55, 57, 60, 62, 64, 67, 69, 72, 74, 76, 79, 81, 84, 86, 88, 91, 93, 96, 98, 100, 103, 105, 108, 110, 112, 115, 117, 120},
                    {9, 12, 14, 15, 19, 21, 24, 26, 28, 31, 33, 36, 38, 40, 43, 45, 48, 50, 52, 55, 57, 60, 62, 64, 67, 69, 72, 74, 76, 79, 81, 84, 86, 88, 91, 93, 96, 98, 100, 103, 105, 108, 110, 112, 115, 117, 120, 122}
                    
                  };
*/                  

byte numPitch = sizeof (pitch) / sizeof (pitch[0]);
byte currentPitch = 0;

byte vel[] = {80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80};

void setup() {

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    //Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer.
  display.clearDisplay();
  // Display bitmap
  display.drawBitmap(0, 0,  logo, 128, 64, WHITE);
  display.display();
  
  // set mpr121 IRQ pin to input
  pinMode(IRQ_PIN1, INPUT);
  pinMode(IRQ_PIN2, INPUT);
  pinMode(IRQ_PIN3, INPUT);
  pinMode(IRQ_PIN4, INPUT);  

  // endless loop if the mpr121 init fails
  // bail if the mpr121 init fails
  if (! cap1.begin(0x5A))
    while (1);
  if (! cap2.begin(0x5B))
    while (1);
  if (! cap3.begin(0x5C))
    while (1);
  if (! cap4.begin(0x5D))
    while (1);

  // set sensitivity of touch surface, lower numbers increase sensitivity
  cap1.setThreshholds(12, 1);
  cap2.setThreshholds(12, 1);
  cap3.setThreshholds(12, 1);
  cap4.setThreshholds(12, 1);

}

void loop() {

  if (NUMBER_BUTTONS != 0) updateButtons();
  if (NUMBER_POTS != 0) updatePots();
  if (NUMBER_MUX_BUTTONS != 0) updateMuxButtons();
  if (NUMBER_MUX_POTS != 0) updateMuxPots();

  // if mpr121 irq goes low, there have been
  // changes to the button states, so read the values
  if(digitalRead(IRQ_PIN1) == LOW)
    readButtons();
  if(digitalRead(IRQ_PIN2) == LOW)
    readButtons();
  if(digitalRead(IRQ_PIN3) == LOW)
    readButtons();
  if(digitalRead(IRQ_PIN4) == LOW)
    readButtons();

}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                       BUTTON PRESS AND MIDI FUNCTIONS                     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

// read the currently touched buttons
void readButtons() {

  // read current values
  currtouched1 = cap1.touched();
  currtouched2 = cap2.touched();
  currtouched3 = cap3.touched();
  currtouched4 = cap4.touched();

  handle_note();

  // save current values to compare against in the next loop
  lasttouched1 = currtouched1;
  lasttouched2 = currtouched2;
  lasttouched3 = currtouched3;
  lasttouched4 = currtouched4;

}

// deal with note on and off presses
void handle_note() {

  for (uint8_t i = 0; i < TOUCH_BUTTONS; i++) {

    // note on check Cap 1
    if ((currtouched1 & _BV(i)) && !(lasttouched1 & _BV(i))) {
      // play pressed note
      noteOn(channel1, pitch[currentPitch][i], vel[i]);
    }

    // note off check Cap 1
    if ( !(currtouched1 & _BV(i)) && (lasttouched1 & _BV(i)) ) {
      // play note off
      noteOff(channel1, pitch[currentPitch][i], vel[i]);
    }

    // note on check Cap 2
    if ((currtouched2 & _BV(i)) && !(lasttouched2 & _BV(i))) {
      // play pressed note
      noteOn(channel1, pitch[currentPitch][i+12], vel[i+12]);
    }

    // note off check Cap 2
    if ( !(currtouched2 & _BV(i)) && (lasttouched2 & _BV(i)) ) {
      // play note off
      noteOff(channel1, pitch[currentPitch][i+12], vel[i+12]);
    }

    // note on check Cap 3
    if ((currtouched3 & _BV(i)) && !(lasttouched3 & _BV(i))) {
      // play pressed note
      noteOn(channel2, pitch[currentPitch][i+24], vel[i+24]);
    }

    // note off check Cap 3
    if ( !(currtouched3 & _BV(i)) && (lasttouched3 & _BV(i)) ) {
      // play note off
      noteOff(channel2, pitch[currentPitch][i+24], vel[i+24]);
    }

    // note on check Cap 4
    if ((currtouched4 & _BV(i)) && !(lasttouched4 & _BV(i))) {
      // play pressed note
      noteOn(channel2, pitch[currentPitch][i+36], vel[i+36]);
    }

    // note off check Cap 4
    if ( !(currtouched4 & _BV(i)) && (lasttouched4 & _BV(i)) ) {
      // play note off
      noteOff(channel2, pitch[currentPitch][i+36], vel[i+36]);
    }

  }

}

//*****************************************************************
void updateButtons() {

  // Cycle through Button array
  for (int i = 0; i < NUMBER_BUTTONS; i = i + 1) {
  byte message = BUTTONS[i]->getValue();

  // button 0 pressed
  if (message == 0 && i == 0) {
        if (menu_state == 0 && menu_state_keys == 0) {
          drawScales(1);
        } 
        if (menu_state == 1 || menu_state_keys == 1) {
          byte menu_result = 0;
          menu_result = drawMenu(1);
          if (menu_result == 1) {
            menu_state = 0;
            menu_state_keys = 0;
            drawScales(0);
          }
        }
  } 
  // button 1 pressed
  if (message == 0 && i == 1) {
        if (menu_state == 0 && menu_state_keys == 0) {
          drawScales(2);
        }
        if (menu_state == 1 || menu_state_keys == 1) {
          byte menu_result = 0;
          menu_result = drawMenu(2);
          if (menu_result == 1) {
            menu_state = 0;
            menu_state_keys = 0;
            drawScales(0);
          }
        }
  }
  // button 2 pressed
  if (message == 0 && i == 2) {
    menu_state = 1;
    drawMenu(0);
  }
  
  } // end for
  
} // end updateButtons

//*******************************************************************
void updateMuxButtons() {

  // Cycle through Mux Button array
  for (byte i = 0; i < NUMBER_MUX_BUTTONS; i = i + 1) {

    MUXBUTTONS[i]->muxUpdate();
    byte message = MUXBUTTONS[i]->getValue();

  }

}

//***********************************************************************
void updatePots() {
  for (byte i = 0; i < NUMBER_POTS; i = i + 1) {
    byte potmessage = POTS[i]->getValue();
    if (potmessage != 255) controlChange(POTS[i]->Pchannel, POTS[i]->Pcontrol, potmessage);
  }
}

//***********************************************************************
void updateMuxPots() {
  for (byte i = 0; i < NUMBER_MUX_POTS; i = i + 1) {
    MUXPOTS[i]->muxUpdate();
    byte potmessage = MUXPOTS[i]->getValue();
    if (potmessage != 255) controlChange(MUXPOTS[i]->Pchannel, MUXPOTS[i]->Pcontrol, potmessage);
  }
}

//***************** DISPLAY FUNCTIONS ***********************************

void drawScales(byte value) {
  if (value == 1) {
    if (currentPitch == (numPitch - 1)) {
      currentPitch = 0;
    } else {
      currentPitch++;
    }
  }
  
  if (value == 2) {
    if (currentPitch == 0) {
      currentPitch = (numPitch - 1);
    } else {
      currentPitch--;
    }
  }
  
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  //byte sLength = strlen(scales[3]);
  //display.setCursor((SCREEN_WIDTH /2) - (sLength * 2),(SCREEN_HEIGHT / 2) - 4);             // Start at top-left corner
  display.setCursor(0,8);
  //display.setCursor(0,8);
  display.println(scales[currentPitch]);
  display.display();
}

byte drawMenu(byte value) {

    // default menu no press
    if (value == 0 && menu_state_keys == 0) {
      display.clearDisplay();
      display.setTextSize(2);             // Normal 1:1 pixel scale
      display.setCursor(5,0);             // Start at top-left corner
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
      display.println("set scales");
      display.setCursor(5,24);
      display.setTextColor(SSD1306_WHITE);        // Draw white text
      display.println("set keys"); 
      display.display();
      menu_count = 0;

      return 0;
    }

    // keys menu no press
    if (value == 0 && menu_state_keys == 1) {
      display.clearDisplay();
      display.setTextSize(2);             // Normal 1:1 pixel scale
      display.setCursor(5,0);             // Start at top-left corner
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
      display.println("1-48 CH1");
      display.setCursor(5,24);
      display.setTextColor(SSD1306_WHITE);        // Draw white text
      display.println("1-24 CH1\n25-48 CH2");
      display.display();
      menu_count = 0;
      menu_count_keys = 0;

      return 0;
    }

    // default menu button 1 press
    if (value == 1 && menu_state_keys == 0) {
      if(menu_count == (menu_items - 1)) {
        menu_count = 0;
      } else {
        menu_count++;
      }
      display.clearDisplay();
      display.setTextSize(2);             // Normal 1:1 pixel scale
      display.setCursor(5,0);             // Start at top-left corner
      if (menu_count == 0) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
      } else {
        display.setTextColor(SSD1306_WHITE);        // Draw white text
      }
      display.println("set scales");
      display.setCursor(5,24);
      if (menu_count == 1) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
      } else {
        display.setTextColor(SSD1306_WHITE);        // Draw white text
      }
      display.println("set keys"); 
      display.display();   
      
      
      return 0;
    }

    // keys menu button 1 press
    if (value == 1 && menu_state_keys == 1) {
      if(menu_count_keys == (menu_items_keys - 1)) {
        menu_count_keys = 0;
      } else {
        menu_count_keys++;
      }
      display.clearDisplay();
      display.setTextSize(2);             // Normal 1:1 pixel scale
      display.setCursor(5,0);             // Start at top-left corner
      if (menu_count_keys == 0) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
      } else {
        display.setTextColor(SSD1306_WHITE);        // Draw white text
      }
      display.println("1-48 CH1");
      display.setCursor(5,24);
      if (menu_count_keys == 1) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
      } else {
        display.setTextColor(SSD1306_WHITE);        // Draw white text
      }
      display.println("1-24 CH1\n25-48 CH2"); 
      display.display();   
      
      
      return 0;
    }

    // default menu button 2 press
    if (value == 2 && menu_state_keys == 0) {
      if (menu_count == 0) {
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);        // Draw white text
        byte sLength = strlen("scales");
        display.setCursor((SCREEN_WIDTH /2) - (sLength * 6),(SCREEN_HEIGHT / 2) - 8);             // Start at top-left corner
        display.invertDisplay(true);
        display.println("scales");
        display.display();
        delay(1000);
        display.invertDisplay(false);
        
        return 1;
      }
      if (menu_count == 1) {
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);        // Draw white text
        byte sLength = strlen("keys");
        display.setCursor((SCREEN_WIDTH /2) - (sLength * 6),(SCREEN_HEIGHT / 2) - 8);             // Start at top-left corner
        display.invertDisplay(true);
        display.println("keys");
        display.display();
        delay(1000);
        display.invertDisplay(false);
        menu_state_keys = 1;

        display.clearDisplay();
        display.setTextSize(2);             // Normal 1:1 pixel scale
        display.setCursor(5,0);             // Start at top-left corner
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
        display.println("1-48 CH1");
        display.setCursor(5,24);
        display.setTextColor(SSD1306_WHITE);        // Draw white text
        display.println("1-24 CH1\n25-48 CH2");
        display.display();
        menu_count = 0;
        menu_count_keys = 0;

        return 0;
      }
    }

    // keys menu button 2 press
    if (value == 2 && menu_state_keys == 1) {
      if (menu_count_keys == 0) {
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);        // Draw white text
        byte sLength = strlen("1-48 CH1");
        display.setCursor((SCREEN_WIDTH /2) - (sLength * 6),(SCREEN_HEIGHT / 2) - 8);             // Start at top-left corner
        display.invertDisplay(true);
        display.println("1-48 CH1");
        display.display();
        delay(1000);
        display.invertDisplay(false);
        menu_state_keys = 0;
        channel1 = 0;
        channel2 = 0;
        
        return 1;
      }
      if (menu_count_keys == 1) {
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);        // Draw white text
        byte sLength1 = strlen("1-24 CH1");
        byte sLength2 = strlen("25-48 CH2");
        display.setCursor((SCREEN_WIDTH /2) - (sLength1 * 6),(SCREEN_HEIGHT / 2) - 16);             // Start at top-left corner
        display.invertDisplay(true);
        display.println("1-24 CH1");
        display.setCursor((SCREEN_WIDTH /2) - (sLength2 * 6),(SCREEN_HEIGHT / 2));             // Start at top-left corner
        display.println("25-48 CH2");
        display.display();
        delay(1000);
        display.invertDisplay(false);
        menu_state_keys = 0;
        channel1 = 0;
        channel2 = 1;

        return 1;
      }
    }

}
