// ---------------------------------------------------------------------------
// Cascade 48
// MIDI Capacitive Touch 48 Key Keyboard Firmware
// Cascade Firmware v2.0
// David Cool
// http://davidcool.com
//
// Firmware for a MIDI (Musical Instrument Digital Interface) keyboard controller
// using 4 MPR121 capacitive touch sensors, Arduino Pro Micro 32u4 (Leonardo), SSD1306 I2C OLED display
// and CD74HC4067 16-Channel Analog Digital Multiplexer
// 
// Uses the MIDIUSB library: https://github.com/arduino-libraries/MIDIUSB
// Uses the Adafruit MPR121 library: https://github.com/adafruit/Adafruit_MPR121
// Uses the Adafruit SSD-1306library: https://github.com/adafruit/Adafruit_SSD1306
//
// Convert your bitmap logo to HEX here: https://javl.github.io/image2cpp/
//
// ---------------------------------------------------------------------------

#include "MIDIUSB.h"          // MIDIUSB library
#include "Wire.h"             // I2C data bus library
#include "Adafruit_MPR121.h"  // touch sensor library
#include "Controller.h"       // class for controlling buttons / pots / muxes
#include "Adafruit_GFX.h"     // Graphics core library for OLED
#include "Adafruit_SSD1306.h" // SSD1306 display library

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     21 // 21=A3 Reset pin # (or -1 if sharing Arduino reset pin), if using -1 this prevents you from uploading code updates!
#define SCREEN_ADDRESS 0x3C // Address is 0x3C for our screen
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Bitmap of david cool logo image in HEX
// convert your image to HEX code here: https://javl.github.io/image2cpp/
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
#define MENU_ITEMS 2
byte menu_state = 0;
byte menu_count = 0;

// set default "set keys" level menu variables
#define MENU_ITEMS_KEYS 2
byte menu_state_keys = 0;
byte menu_count_keys = 0;

// set default MIDI channel to 1
// channel 0 = MIDI channel 1 and so on
byte channel1 = 0; // keys 1-24
byte channel2 = 0; // keys 25-48

//---How many buttons are connected directly to pins?---------
#define NUMBER_BUTTONS 3

//---How many potentiometers are connected to a multiplexer?--
#define NUMBER_MUX_POTS 14

// analog mux
// Mux Name(Port#, num mux inputs, true=analog | false=digital)
Mux M1(A0, 16, true); //Analog multiplexer on Arduino analog pin A0

//***DEFINE DIRECTLY CONNECTED BUTTONS*******************************
//Button (Pin Number, Command, Note Number, Channel, Debounce Time)
//** Command parameter 0=NOTE  1=CC  2=Toggle CC **

Button BU1(9, 1, 60, 1, 5); //Button 1
Button BU2(10, 1, 61, 1, 5); //Button 2
Button BU3(0, 1, 62, 1, 5); //Button 3
Button *BUTTONS[] {&BU1, &BU2, &BU3};

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

#define TOUCH_BUTTONS  12     // defins the number of electrodes on each touch sensor
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

// pitch class set
// C = 0 | C# = 1 | D = 2 | D# = 3 | E = 4 | F = 5 | F# = 6 | G = 7 | G# = 8 | A = 9 | A# = 10 | B = 11

//******* setup all 50 scales **********

// 12 major scales
// C Major Scale ( C, D, E, F, G, A, B ) = ( 0, 2, 4, 5, 7, 9, 11 )
// D Major Scale ( D, E, F#, G, A, B, C# ) = ( 2, 4, 6, 7, 9, 11, 1 )
// E Major Scale ( E, F#, G#, A, B, C#, D# ) = ( 4, 6, 8, 9, 11, 1, 3 )
// F Major Scale ( F, G, A, A#, C, D, E ) = ( 5, 7, 9, 10, 0, 2, 4 )
// G Major Scale ( G, A, B, C, D, E, F# ) = ( 7, 9, 11, 0, 2, 4, 6 )
// A Major Scale ( A, B, C#, D, E, F#, G# ) = ( 9, 11, 1, 2, 4, 6, 8 )
// B Major Scale ( B, C#, D#, E, F#, G#, A# ) = ( 11, 1, 3, 4, 6, 8, 10 )
// C Sharp/D Flat Major Scale ( C#, D#, F, F#, G#, A#, C ) = ( 1, 3, 5, 6, 8, 10, 0 )
// D Sharp/E Flat Major Scale ( D#, F, G, G#, A#, C, D ) = ( 3, 5, 7, 8, 10, 0, 2 )
// F Sharp/G Flat Major Scale ( F#, G#, A#, B, C#, D#, F ) = ( 6, 8, 10, 11, 1, 3, 5 )
// G Sharp/A Flat Major Scale ( G#, A#, C, C#, D#, F, G ) = ( 8, 10, 0, 1, 3, 5, 7 )
// A Sharp/B Flat Major Scale ( A#, C, D, D#, F, G, A ) = ( 10, 0, 2, 3, 5, 7, 9 )

// 12 minor scales
// A Minor Scale ( A, B, C, D, E, F, G ) = ( 9, 11, 0, 2, 4, 5, 7 )
// A Sharp/B Flat Minor Scale ( A#, C, C#, D#, F, F#, G# ) = ( 10, 0, 1, 3, 5, 6, 8 )
// B Minor Scale ( B, C#, D, E, F#, G, A ) = ( 11, 1, 2, 4, 6, 7, 9 )
// C Minor Scale ( C, D, D#, F, G, G#, A# ) = ( 0, 2, 3, 5, 7, 8, 10 )
// C Sharp/D Flat Minor Scale ( C#, D#, E, F#, G#, A, B ) = ( 1, 3, 4, 6, 8, 9, 11 )
// D Minor Scale ( D, E, F, G, A, A#, C ) = ( 2, 4, 5, 7, 9, 10, 0 )
// D Sharp/E Flat Minor Scale ( D#, F, F#, G#, A#, B, C# ) = ( 3, 5, 6, 8, 10, 11, 1 )
// E Minor Scale ( E, F#, G, A, B, C, D ) = ( 4, 6, 7, 9, 11, 0, 2 )
// F Minor Scale ( F, G, G#, A#, C, C#, D# ) = ( 5, 7, 8, 10, 0, 1, 3 )
// F Sharp/G Flat Minor Scale ( F#, G#, A, B, C#, D, E ) = ( 6, 8, 9, 11, 1, 2, 4 )
// G Minor Scale ( G, A, A#, C, D, D#, F ) = ( 7, 9, 10, 0, 2, 3, 5 )
// G Sharp/A Flat Minor Scale ( G#, A#, B, C#, D#, E, F# ) = ( 8, 10, 11, 1, 3, 4, 6 )

// 12 pentatonic scales
// C Minor Pentatonic Scale ( C, D#, F, G, A# ) = ( 0, 3, 5, 7, 10 )
// C Sharp Minor/D Flat Minor Pentatonic Scale ( C#, E, F#, G#, B ) = ( 1, 4, 6, 8, 11 )
// D Minor Pentatonic Scale ( D, F, G, A, C ) = ( 2, 5, 7, 9, 0 )
// D Sharp Minor/E Flat Minor Pentatonic Scale ( D#, F#, G#, A#, C# ) = ( 3, 6, 8, 10, 1 )
// E Minor Pentatonic Scale ( E, G, A, B, D ) = ( 4, 7, 9, 11, 2 )
// F Minor Pentatonic Scale ( F, G#, A#, C, D# ) = ( 5, 8, 10, 0, 3 )
// F Sharp Minor/G Flat Minor Pentatonic Scale ( F#, A, B, C#, E ) = ( 6, 9, 11, 1, 4 )
// G Minor Pentatonic Scale ( G, A#, C, D, F ) = ( 7, 10, 0, 2, 5 )
// G Sharp Minor/A Flat Minor Pentatonic Scale ( G#, B, C#, D#, F# ) = ( 8, 11, 1, 3, 6 )
// A Minor Pentatonic Scale ( A, C, D, E, G ) = ( 9, 0, 2, 4, 7 )
// A Sharp Minor/B Flat Minor Pentatonic Scale ( A#, C#, D#, F, G# ) = ( 10, 1, 3, 5, 8 )
// B Minor Pentatonic Scale ( B, D, E, F#, A ) = ( 11, 2, 4, 6, 9 )

// 24 exotic scales
// C Sharp/D Flat Algerian Scale ( C#, D#, E, F#, G, G#, A, C ) = ( 1, 3, 4, 6, 7, 8, 9, 0 )
// D Insen Ursa Minor Scale ( A, A#, D, E, F, G ) = ( 9, 10, 2, 4, 5, 7 )
// C Ritsu Japanese Scale ( C, C#, D#, F, G#, A# ) = ( 0, 1, 3, 5, 8, 10 )
// C Dorian Flat 2 Scale ( C, C#, D#, F, G, A, A# ) = ( 0, 1, 3, 5, 7, 9, 10 )
// C Raga Chhaya Todi Scale ( C, C#, D#, F#, G# ) = ( 0, 1, 3, 6, 8 )
// E Aeolian Dominant Scale ( E, F#, G#, A, B, C, D ) = ( 4, 6, 8, 9, 11, 0, 2 )
// A Arabic Scale ( A, B, C#, D, D#, F, G ) = ( 9, 11, 1, 2, 3, 5, 7 )
// F Augmented Scale ( F, G#, A, C, C#, E ) = ( 5, 8, 9, 0, 1, 4 )
// D Balinese Scale ( D, D#, F, A, A# ) = ( 2, 3, 5, 9, 10 )
// G Byzantine Scale ( G, G#, B, C, D, D#, F# ) = ( 7, 8, 11, 0, 2, 3, 6 )
// D Egyptian Suspended Scale ( D, E, G, A, C ) = ( 2, 4, 7, 9, 0 )
// D Romanian Minor Scale ( D, E, F, G#, A, B, C ) = ( 2, 4, 5, 8, 9, 11, 0 )
// F Lydian Dominant Scale ( F, G, A, B, C, D, D# ) = ( 5, 7, 9, 11, 0, 2, 3 )
// B Enigmatic Scale ( B, C, D#, F, G, A, A# ) = ( 11, 0, 3, 5, 7, 9, 10 )
// G Spanish Gypsy Scale ( G, G#, B, C, D, D#, F ) = ( 7, 8, 11, 0, 2, 3, 5 )
// E Prometheus Scale ( E, F#, G#, A#, C#, D ) = ( 4, 6, 8, 10, 1, 2 )
// F Oriental Scale ( F, F#, A, A#, B, D, D# ) = ( 5, 6, 9, 10, 11, 2, 3 )
// D#/Eb Octatonic Scale ( D#, E, F#, G, A, A#, C, C# ) = ( 3, 4, 6, 7, 9, 10, 1 )
// D Maqam Shahnaz Scale ( D, D#, F#, G, A, A#, C# ) = ( 2, 3, 6, 7, 9, 10, 1 )
// G Iwato Japanese Scale ( G, G#, C, C#, F ) = ( 7, 8, 0, 1, 5 )
// C Gypsy Hungarian  Scale( C, D, D#, F#, G, G#, B ) = ( 0, 2, 3, 6, 7, 8, 11 )
// A Hawaiian Scale ( A, B, C, D, E, F#, G# ) = ( 9, 11, 0, 2, 4, 6, 8 )
// F#/Gb Chinese Scale ( F#, A#, C, C#, F ) = ( 6, 10, 0, 1, 5 )
// G#/Ab Hirajoshi ( G#, A#, B, D#, E ) = ( 8, 10, 11, 3, 4 )

const char s0[] PROGMEM = "C\n\nMajor";
const char s1[] PROGMEM = "D\n\nMajor";
const char s2[] PROGMEM = "E\n\nMajor";
const char s3[] PROGMEM = "F\n\nMajor";
const char s4[] PROGMEM = "G\n\nMajor";
const char s5[] PROGMEM = "A\n\nMajor";
const char s6[] PROGMEM = "B\n\nMajor";
const char s7[] PROGMEM = "C#/Db\n\nMajor";
const char s8[] PROGMEM = "D#/Eb\n\nMajor";
const char s9[] PROGMEM = "F#/Gb\n\nMajor";
const char s10[] PROGMEM = "G#/Ab\n\nMajor";
const char s11[] PROGMEM = "A#/Bb\n\nMajor";

const char s12[] PROGMEM = "A\n\nMinor";
const char s13[] PROGMEM = "A#/Bb\n\nMinor";
const char s14[] PROGMEM = "B\n\nMinor";
const char s15[] PROGMEM = "C\n\nMinor";
const char s16[] PROGMEM = "C#/Db\n\nMinor";
const char s17[] PROGMEM = "D\n\nMinor";
const char s18[] PROGMEM = "D#/Eb\n\nMinor";
const char s19[] PROGMEM = "E\n\nMinor";
const char s20[] PROGMEM = "F\n\nMinor";
const char s21[] PROGMEM = "F#/Gb\n\nMinor";
const char s22[] PROGMEM = "G\n\nMinor";
const char s23[] PROGMEM = "G#/Ab\n\nMinor";

const char s24[] PROGMEM = "C Minor\n\nPentatonic";
const char s25[] PROGMEM = "C#/DbMinor\n\nPentatonic";
const char s26[] PROGMEM = "D Minor\n\nPentatonic";
const char s27[] PROGMEM = "D#/EbMinor\n\nPentatonic";
const char s28[] PROGMEM = "E Minor\n\nPentatonic";
const char s29[] PROGMEM = "F Minor\n\nPentatonic";
const char s30[] PROGMEM = "F#/GbMinor\n\nPentatonic";
const char s31[] PROGMEM = "G Minor\n\nPentatonic";
const char s32[] PROGMEM = "G#/AbMinor\n\nPentatonic";
const char s33[] PROGMEM = "A Minor\n\nPentatonic";
const char s34[] PROGMEM = "A#/BbMinor\n\nPentatonic";
const char s35[] PROGMEM = "B Minor\n\nPentatonic";

const char s36[] PROGMEM = "C#/Db\n\nAlgerian";
const char s37[] PROGMEM = "D Insen\n\nUrsa Minor";
const char s38[] PROGMEM = "C Ritsu\n\nJapanese";
const char s39[] PROGMEM = "C Dorian\n\nFlat 2";
const char s40[] PROGMEM = "C Raga\n\nChhayaTodi";
const char s41[] PROGMEM = "E Aeolian\n\nDominant";
const char s42[] PROGMEM = "A\n\nArabic";
const char s43[] PROGMEM = "F\n\nAugmented";
const char s44[] PROGMEM = "D\n\nBalinese";
const char s45[] PROGMEM = "G\n\nByzantine";
const char s46[] PROGMEM = "D Egyptian\n\nSuspended";
const char s47[] PROGMEM = "D Romanian\n\nMinor";
const char s48[] PROGMEM = "F Lydian\n\nDominant";
const char s49[] PROGMEM = "B\n\nEnigmatic";
const char s50[] PROGMEM = "G Gypsy\n\nSpanish";
const char s51[] PROGMEM = "E\n\nPrometheus";
const char s52[] PROGMEM = "F\n\nOriental";
const char s53[] PROGMEM = "D#/Eb\n\nOctatonic";
const char s54[] PROGMEM = "D Maqam\n\nShahnaz";
const char s55[] PROGMEM = "G Iwato\n\nJapanese";
const char s56[] PROGMEM = "C Gypsy\n\nHungarian";
const char s57[] PROGMEM = "A\n\nHawaiian";
const char s58[] PROGMEM = "F#/Gb\n\nChinese";
const char s59[] PROGMEM = "G#/Ab\n\nHirajoshi";

const char *const string_table[] PROGMEM = {s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16, s17, s18, s19, s20, s21, s22, s23, s24, s25, s26, s27, s28, s29, s30, s31, s32, s33, s34, s35, s36, s37, s38, s39, s40, s41, s42, s43, s44, s45, s46, s47, s48, s49, s50, s51, s52, s53, s54, s55, s56, s57, s58, s59};
char current_scale[25];  // make sure this is large enough for the largest string it must hold from the scales above

const byte pitch[60][48] PROGMEM = { 
  // major scales
  { 24, 26, 28, 29, 31, 33, 35, 36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67, 69, 71, 72, 74, 76, 77, 79, 81, 83, 84, 86, 88, 89, 91, 93, 95, 96, 98, 100, 101, 103, 105 },
  { 26, 28, 30, 31, 33, 35, 37, 38, 40, 42, 43, 45, 47, 49, 50, 52, 54, 55, 57, 59, 61, 62, 64, 66, 67, 69, 71, 73, 74, 76, 78, 79, 81, 83, 85, 86, 88, 90, 91, 93, 95, 97, 98, 100, 102, 103, 105, 107 },
  { 28, 30, 32, 33, 35, 37, 39, 40, 42, 44, 45, 47, 49, 51, 52, 54, 56, 57, 59, 61, 63, 64, 66, 68, 69, 71, 73, 75, 76, 78, 80, 81, 83, 85, 87, 88, 90, 92, 93, 95, 97, 99, 100, 102, 104, 105, 107, 109 },
  { 29, 31, 33, 34, 36, 38, 40, 41, 43, 45, 46, 48, 50, 52, 53, 55, 57, 58, 60, 62, 64, 65, 67, 69, 70, 72, 74, 76, 77, 79, 81, 82, 84, 86, 88, 89, 91, 93, 94, 96, 98, 100, 101, 103, 105, 106, 108, 110 },
  { 31, 33, 35, 36, 38, 40, 42, 43, 45, 47, 48, 50, 52, 54, 55, 57, 59, 60, 62, 64, 66, 67, 69, 71, 72, 74, 76, 78, 79, 81, 83, 84, 86, 88, 90, 91, 93, 95, 96, 98, 100, 102, 103, 105, 107, 108, 110, 112 },
  { 33, 35, 37, 38, 40, 42, 44, 45, 47, 49, 50, 52, 54, 56, 57, 59, 61, 62, 64, 66, 68, 69, 71, 73, 74, 76, 78, 80, 81, 83, 85, 86, 88, 90, 92, 93, 95, 97, 98, 100, 102, 104, 105, 107, 109, 110, 112, 114 },
  { 35, 37, 39, 40, 42, 44, 46, 47, 49, 51, 52, 54, 56, 58, 59, 61, 63, 64, 66, 68, 70, 71, 73, 75, 76, 78, 80, 82, 83, 85, 87, 88, 90, 92, 94, 95, 97, 99, 100, 102, 104, 106, 107, 109, 111, 112, 114, 116 },
  { 25, 27, 29, 30, 32, 34, 36, 37, 39, 41, 42, 44, 46, 48, 49, 51, 53, 54, 56, 58, 60, 61, 63, 65, 66, 68, 70, 72, 73, 75, 77, 78, 80, 82, 84, 85, 87, 89, 90, 92, 94, 96, 97, 99, 101, 102, 104, 106 },
  { 27, 29, 31, 32, 34, 36, 38, 39, 41, 43, 44, 46, 48, 50, 51, 53, 55, 56, 58, 60, 62, 63, 65, 67, 68, 70, 72, 74, 75, 77, 79, 80, 82, 84, 86, 87, 89, 91, 92, 94, 96, 98, 99, 101, 103, 104, 106, 108 },
  { 30, 32, 34, 35, 37, 39, 41, 42, 44, 46, 47, 49, 51, 53, 54, 56, 58, 59, 61, 63, 65, 66, 68, 70, 71, 73, 75, 77, 78, 80, 82, 83, 85, 87, 89, 90, 92, 94, 95, 97, 99, 101, 102, 104, 106, 107, 109, 111 },
  { 32, 34, 36, 37, 39, 41, 43, 44, 46, 48, 49, 51, 53, 55, 56, 58, 60, 61, 63, 65, 67, 68, 70, 72, 73, 75, 77, 79, 80, 82, 84, 85, 87, 89, 91, 92, 94, 96, 97, 99, 101, 103, 104, 106, 108, 109, 111, 113 },
  { 34, 36, 38, 39, 41, 43, 45, 46, 48, 50, 51, 53, 55, 57, 58, 60, 62, 63, 65, 67, 69, 70, 72, 74, 75, 77, 79, 81, 82, 84, 86, 87, 89, 91, 93, 94, 96, 98, 99, 101, 103, 105, 106, 108, 110, 111, 113, 115 },
  // minor scales
  { 33, 35, 36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67, 69, 71, 72, 74, 76, 77, 79, 81, 83, 84, 86, 88, 89, 91, 93, 95, 96, 98, 100, 101, 103, 105, 107, 108, 110, 112, 113 },
  { 34, 36, 37, 39, 41, 42, 44, 46, 48, 49, 51, 53, 54, 56, 58, 60, 61, 63, 65, 66, 68, 70, 72, 73, 75, 77, 78, 80, 82, 84, 85, 87, 89, 90, 92, 94, 96, 97, 99, 101, 102, 104, 106, 108, 109, 111, 113, 114 },
  { 35, 37, 38, 40, 42, 43, 45, 47, 49, 50, 52, 54, 55, 57, 59, 61, 62, 64, 66, 67, 69, 71, 73, 74, 76, 78, 79, 81, 83, 85, 86, 88, 90, 91, 93, 95, 97, 98, 100, 102, 103, 105, 107, 109, 110, 112, 114, 115 },
  { 24, 26, 27, 29, 31, 32, 34, 36, 38, 39, 41, 43, 44, 46, 48, 50, 51, 53, 55, 56, 58, 60, 62, 63, 65, 67, 68, 70, 72, 74, 75, 77, 79, 80, 82, 84, 86, 87, 89, 91, 92, 94, 96, 98, 99, 101, 103, 104 },
  { 25, 27, 28, 30, 32, 33, 35, 37, 39, 40, 42, 44, 45, 47, 49, 51, 52, 54, 56, 57, 59, 61, 63, 64, 66, 68, 69, 71, 73, 75, 76, 78, 80, 81, 83, 85, 87, 88, 90, 92, 93, 95, 97, 99, 100, 102, 104, 105 },
  { 26, 28, 29, 31, 33, 34, 36, 38, 40, 41, 43, 45, 46, 48, 50, 52, 53, 55, 57, 58, 60, 62, 64, 65, 67, 69, 70, 72, 74, 76, 77, 79, 81, 82, 84, 86, 88, 89, 91, 93, 94, 96, 98, 100, 101, 103, 105, 106 },
  { 27, 29, 30, 32, 34, 35, 37, 39, 41, 42, 44, 46, 47, 49, 51, 53, 54, 56, 58, 59, 61, 63, 65, 66, 68, 70, 71, 73, 75, 77, 78, 80, 82, 83, 85, 87, 89, 90, 92, 94, 95, 97, 99, 101, 102, 104, 106, 107 },
  { 28, 30, 31, 33, 35, 36, 38, 40, 42, 43, 45, 47, 48, 50, 52, 54, 55, 57, 59, 60, 62, 64, 66, 67, 69, 71, 72, 74, 76, 78, 79, 81, 83, 84, 86, 88, 90, 91, 93, 95, 96, 98, 100, 102, 103, 105, 107, 108 },
  { 29, 31, 32, 34, 36, 37, 39, 41, 43, 44, 46, 48, 49, 51, 53, 55, 56, 58, 60, 61, 63, 65, 67, 68, 70, 72, 73, 75, 77, 79, 80, 82, 84, 85, 87, 89, 91, 92, 94, 96, 97, 99, 101, 103, 104, 106, 108, 109 },
  { 30, 32, 33, 35, 37, 38, 40, 42, 44, 45, 47, 49, 50, 52, 54, 56, 57, 59, 61, 62, 64, 66, 68, 69, 71, 73, 74, 76, 78, 80, 81, 83, 85, 86, 88, 90, 92, 93, 95, 97, 98, 100, 102, 104, 105, 107, 109, 110 },
  { 31, 33, 34, 36, 38, 39, 41, 43, 45, 46, 48, 50, 51, 53, 55, 57, 58, 60, 62, 63, 65, 67, 69, 70, 72, 74, 75, 77, 79, 81, 82, 84, 86, 87, 89, 91, 93, 94, 96, 98, 99, 101, 103, 105, 106, 108, 110, 111 },
  { 32, 34, 35, 37, 39, 40, 42, 44, 46, 47, 49, 51, 52, 54, 56, 58, 59, 61, 63, 64, 66, 68, 70, 71, 73, 75, 76, 78, 80, 82, 83, 85, 87, 88, 90, 92, 94, 95, 97, 99, 100, 102, 104, 106, 107, 109, 111, 112 },
  // pentatonic scales
  { 0, 3, 5, 7, 10, 12, 15, 17, 19, 22, 24, 27, 29, 31, 34, 36, 39, 41, 43, 46, 48, 51, 53, 55, 58, 60, 63, 65, 67, 70, 72, 75, 77, 79, 82, 84, 87, 89, 91, 94, 96, 99, 101, 103, 106, 108, 111, 113 },
  { 1, 4, 6, 8, 11, 13, 16, 18, 20, 23, 25, 28, 30, 32, 35, 37, 40, 42, 44, 47, 49, 52, 54, 56, 59, 61, 64, 66, 68, 71, 73, 76, 78, 80, 83, 85, 88, 90, 92, 95, 97, 100, 102, 104, 107, 109, 112, 114 },
  { 2, 5, 7, 9, 12, 14, 17, 19, 21, 24, 26, 29, 31, 33, 36, 38, 41, 43, 45, 48, 50, 53, 55, 57, 60, 62, 65, 67, 69, 72, 74, 77, 79, 81, 84, 86, 89, 91, 93, 96, 98, 101, 103, 105, 108, 110, 113, 115 },
  { 3, 6, 8, 10, 13, 15, 18, 20, 22, 25, 27, 30, 32, 34, 37, 39, 42, 44, 46, 49, 51, 54, 56, 58, 61, 63, 66, 68, 70, 73, 75, 78, 80, 82, 85, 87, 90, 92, 94, 97, 99, 102, 104, 106, 109, 111, 114, 116 },
  { 4, 7, 9, 11, 14, 16, 19, 21, 23, 26, 28, 31, 33, 35, 38, 40, 43, 45, 47, 50, 52, 55, 57, 59, 62, 64, 67, 69, 71, 74, 76, 79, 81, 83, 86, 88, 91, 93, 95, 98, 100, 103, 105, 107, 110, 112, 115, 117 },
  { 5, 8, 10, 12, 15, 17, 20, 22, 24, 27, 29, 32, 34, 36, 39, 41, 44, 46, 48, 51, 53, 56, 58, 60, 63, 65, 68, 70, 72, 75, 77, 80, 82, 84, 87, 89, 92, 94, 96, 99, 101, 104, 106, 108, 111, 113, 116, 118 },
  { 6, 9, 11, 13, 16, 18, 21, 23, 25, 28, 30, 33, 35, 37, 40, 42, 45, 47, 49, 52, 54, 57, 59, 61, 64, 66, 69, 71, 73, 76, 78, 81, 83, 85, 88, 90, 93, 95, 97, 100, 102, 105, 107, 109, 112, 114, 117, 119 },
  { 7, 10, 12, 14, 17, 19, 22, 24, 26, 29, 31, 34, 36, 38, 41, 43, 46, 48, 50, 53, 55, 58, 60, 62, 65, 67, 70, 72, 74, 77, 79, 82, 84, 86, 89, 91, 94, 96, 98, 101, 103, 106, 108, 110, 113, 115, 118, 120 },
  { 8, 11, 13, 15, 18, 20, 23, 25, 27, 30, 32, 35, 37, 39, 42, 44, 47, 49, 51, 54, 56, 59, 61, 63, 66, 68, 71, 73, 75, 78, 80, 83, 85, 87, 90, 92, 95, 97, 99, 102, 104, 107, 109, 111, 114, 116, 119, 121 },
  { 9, 12, 14, 16, 19, 21, 24, 26, 28, 31, 33, 36, 38, 40, 43, 45, 48, 50, 52, 55, 57, 60, 62, 64, 67, 69, 72, 74, 76, 79, 81, 84, 86, 88, 91, 93, 96, 98, 100, 103, 105, 108, 110, 112, 115, 117, 120, 122 },
  { 10, 13, 15, 17, 20, 22, 25, 27, 29, 32, 34, 37, 39, 41, 44, 46, 49, 51, 53, 56, 58, 61, 63, 65, 68, 70, 73, 75, 77, 80, 82, 85, 87, 89, 92, 94, 97, 99, 101, 104, 106, 109, 111, 113, 116, 118, 121, 123 },
  { 11, 14, 16, 18, 21, 23, 26, 28, 30, 33, 35, 38, 40, 42, 45, 47, 50, 52, 54, 57, 59, 62, 64, 66, 69, 71, 74, 76, 78, 81, 83, 86, 88, 90, 93, 95, 98, 100, 102, 105, 107, 110, 112, 114, 117, 119, 122, 124 },
  // 24 exotic scales
  { 25, 27, 28, 30, 31, 32, 33, 36, 37, 39, 40, 42, 43, 44, 45, 48, 49, 51, 52, 54, 55, 56, 57, 60, 61, 63, 64, 66, 67, 68, 69, 72, 73, 75, 76, 78, 79, 80, 81, 84, 85, 87, 88, 90, 91, 92, 93, 96 },
  { 21, 22, 26, 28, 29, 31, 33, 34, 38, 40, 41, 43, 45, 46, 50, 52, 53, 55, 57, 58, 62, 64, 65, 67, 69, 70, 74, 76, 77, 79, 81, 82, 86, 88, 89, 91, 93, 94, 98, 100, 101, 103, 105, 106, 110, 112, 113, 115 },
  { 24, 25, 27, 29, 32, 34, 36, 37, 39, 41, 44, 46, 48, 49, 51, 53, 56, 58, 60, 61, 63, 65, 68, 70, 72, 73, 75, 77, 80, 82, 84, 85, 87, 89, 92, 94, 96, 97, 99, 101, 104, 106, 108, 109, 111, 113, 116, 118 },
  { 24, 25, 27, 29, 31, 33, 34, 36, 37, 39, 41, 43, 45, 46, 48, 49, 51, 53, 55, 57, 58, 60, 61, 63, 65, 67, 69, 70, 72, 73, 75, 77, 79, 81, 82, 84, 85, 87, 89, 91, 93, 94, 96, 97, 99, 101, 103, 105 },
  { 12, 13, 15, 18, 20, 24, 25, 27, 30, 32, 36, 37, 39, 42, 44, 48, 49, 51, 54, 56, 60, 61, 63, 66, 68, 72, 73, 75, 78, 80, 84, 85, 87, 90, 92, 96, 97, 99, 102, 104, 108, 109, 111, 114, 116, 120, 121, 123 },
  { 28, 30, 32, 33, 35, 36, 38, 40, 42, 44, 45, 47, 48, 50, 52, 54, 56, 57, 59, 60, 62, 64, 66, 68, 69, 71, 72, 74, 76, 78, 80, 81, 83, 84, 86, 88, 90, 92, 93, 95, 96, 98, 100, 102, 104, 105, 107, 108 },
  { 33, 35, 37, 38, 39, 41, 43, 45, 47, 49, 50, 51, 53, 55, 57, 59, 61, 62, 63, 65, 67, 69, 71, 73, 74, 75, 77, 79, 81, 83, 85, 86, 87, 89, 91, 93, 95, 97, 98, 99, 101, 103, 105, 107, 109, 110, 111, 113 },
  { 17, 20, 21, 24, 25, 28, 29, 32, 33, 36, 37, 40, 41, 44, 45, 48, 49, 52, 53, 56, 57, 60, 61, 64, 65, 68, 69, 72, 73, 76, 77, 80, 81, 84, 85, 88, 89, 92, 93, 96, 97, 100, 101, 104, 105, 108, 109, 112 },
  { 14, 15, 17, 21, 22, 26, 27, 29, 33, 34, 38, 39, 41, 45, 46, 50, 51, 53, 57, 58, 62, 63, 65, 69, 70, 74, 75, 77, 81, 82, 86, 87, 89, 93, 94, 98, 99, 101, 105, 106, 110, 111, 113, 117, 118, 122, 123, 125 },
  { 19, 20, 23, 24, 26, 27, 30, 31, 32, 35, 36, 38, 39, 42, 43, 44, 47, 48, 50, 51, 54, 55, 56, 59, 60, 62, 63, 66, 67, 68, 71, 72, 74, 75, 78, 79, 80, 83, 84, 86, 87, 90, 91, 92, 95, 96, 98, 99 },
  { 2, 4, 7, 9, 12, 14, 16, 19, 21, 24, 26, 28, 31, 33, 36, 38, 40, 43, 45, 48, 50, 52, 55, 57, 60, 62, 64, 67, 69, 72, 74, 76, 79, 81, 84, 86, 88, 91, 93, 96, 98, 100, 103, 105, 108, 110, 112, 115 },
  { 14, 16, 17, 20, 21, 23, 24, 26, 28, 29, 32, 33, 35, 36, 38, 40, 41, 44, 45, 47, 48, 50, 52, 53, 56, 57, 59, 60, 62, 64, 65, 68, 69, 71, 72, 74, 76, 77, 80, 81, 83, 84, 86, 88, 89, 92, 93, 95 },
  { 29, 31, 33, 35, 36, 38, 39, 41, 43, 45, 47, 48, 50, 51, 53, 55, 57, 59, 60, 62, 63, 65, 67, 69, 71, 72, 74, 75, 77, 79, 81, 83, 84, 86, 87, 89, 91, 93, 95, 96, 98, 99, 101, 103, 105, 107, 108, 110 },
  { 35, 36, 39, 41, 43, 45, 46, 47, 48, 51, 53, 55, 57, 58, 59, 60, 63, 65, 67, 69, 70, 71, 72, 75, 77, 79, 81, 82, 83, 84, 87, 89, 91, 93, 94, 95, 96, 99, 101, 103, 105, 106, 107, 108, 111, 113, 115, 117 },
  { 31, 32, 35, 36, 38, 39, 41, 43, 44, 47, 48, 50, 51, 53, 55, 56, 59, 60, 62, 63, 65, 67, 68, 71, 72, 74, 75, 77, 79, 80, 83, 84, 86, 87, 89, 91, 92, 95, 96, 98, 99, 101, 103, 104, 107, 108, 110, 111 },
  { 16, 18, 20, 22, 25, 26, 28, 30, 32, 34, 37, 38, 40, 42, 44, 46, 49, 50, 52, 54, 56, 58, 61, 62, 64, 66, 68, 70, 73, 74, 76, 78, 80, 82, 85, 86, 88, 90, 92, 94, 97, 98, 100, 102, 104, 106, 109, 110 },
  { 17, 18, 21, 22, 23, 26, 27, 29, 30, 33, 34, 35, 38, 39, 41, 42, 45, 46, 47, 50, 51, 53, 54, 57, 58, 59, 62, 63, 65, 66, 69, 70, 71, 74, 75, 77, 78, 81, 82, 83, 86, 87, 89, 90, 93, 94, 95, 98 },
  { 27, 28, 30, 31, 33, 34, 37, 39, 40, 42, 43, 45, 46, 49, 51, 52, 54, 55, 57, 58, 61, 63, 64, 66, 67, 69, 70, 73, 75, 76, 78, 79, 81, 82, 85, 87, 88, 90, 91, 93, 94, 97, 99, 100, 102, 103, 105, 106 },
  { 26, 27, 30, 31, 33, 34, 37, 38, 39, 42, 43, 45, 46, 49, 50, 51, 54, 55, 57, 58, 61, 62, 63, 66, 67, 69, 70, 73, 74, 75, 78, 79, 81, 82, 85, 86, 87, 90, 91, 93, 94, 97, 98, 99, 102, 103, 105, 106 },
  { 7, 8, 12, 13, 17, 19, 20, 24, 25, 29, 31, 32, 36, 37, 41, 43, 44, 48, 49, 53, 55, 56, 60, 61, 65, 67, 68, 72, 73, 77, 79, 80, 84, 85, 89, 91, 92, 96, 97, 101, 103, 104, 108, 109, 113, 115, 116, 120 },
  { 24, 26, 27, 30, 31, 32, 35, 36, 38, 39, 42, 43, 44, 47, 48, 50, 51, 54, 55, 56, 59, 60, 62, 63, 66, 67, 68, 71, 72, 74, 75, 78, 79, 80, 83, 84, 86, 87, 90, 91, 92, 95, 96, 98, 99, 102, 103, 104 },
  { 33, 35, 36, 38, 40, 42, 44, 45, 47, 48, 50, 52, 54, 56, 57, 59, 60, 62, 64, 66, 68, 69, 71, 72, 74, 76, 78, 80, 81, 83, 84, 86, 88, 90, 92, 93, 95, 96, 98, 100, 102, 104, 105, 107, 108, 110, 112, 114 },
  { 6, 10, 12, 13, 17, 18, 22, 24, 25, 29, 30, 34, 36, 37, 41, 42, 46, 48, 49, 53, 54, 58, 60, 61, 65, 66, 70, 72, 73, 77, 78, 82, 84, 85, 89, 90, 94, 96, 97, 101, 102, 106, 108, 109, 113, 114, 118, 120 },
  { 8, 10, 11, 15, 16, 20, 22, 23, 27, 28, 32, 34, 35, 39, 40, 44, 46, 47, 51, 52, 56, 58, 59, 63, 64, 68, 70, 71, 75, 76, 80, 82, 83, 87, 88, 92, 94, 95, 99, 100, 104, 106, 107, 111, 112, 116, 118, 119 }
};                

byte numPitch = sizeof (pitch) / sizeof (pitch[0]);
byte currentPitch = 0;

// key velocity
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

/* Instead of printing bitmap, could save memory and print text instead
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  byte sLength = strlen("CASCADE 48");
  display.setCursor((SCREEN_WIDTH /2) - (sLength * 6),(SCREEN_HEIGHT / 2) - 8);
  display.println(F("CASCADE 48"));
  display.display();
*/

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
      noteOn(channel1, pgm_read_byte(&(pitch[currentPitch][i])), vel[i]);
    }

    // note off check Cap 1
    if ( !(currtouched1 & _BV(i)) && (lasttouched1 & _BV(i)) ) {
      // play note off
      noteOff(channel1, pgm_read_byte(&(pitch[currentPitch][i])), vel[i]);
    }

    // note on check Cap 2
    if ((currtouched2 & _BV(i)) && !(lasttouched2 & _BV(i))) {
      // play pressed note
      noteOn(channel1, pgm_read_byte(&(pitch[currentPitch][i+12])), vel[i+12]);
    }

    // note off check Cap 2
    if ( !(currtouched2 & _BV(i)) && (lasttouched2 & _BV(i)) ) {
      // play note off
      noteOff(channel1, pgm_read_byte(&(pitch[currentPitch][i+12])), vel[i+12]);
    }

    // note on check Cap 3
    if ((currtouched3 & _BV(i)) && !(lasttouched3 & _BV(i))) {
      // play pressed note
      noteOn(channel2, pgm_read_byte(&(pitch[currentPitch][i+24])), vel[i+24]);
    }

    // note off check Cap 3
    if ( !(currtouched3 & _BV(i)) && (lasttouched3 & _BV(i)) ) {
      // play note off
      noteOff(channel2, pgm_read_byte(&(pitch[currentPitch][i+24])), vel[i+24]);
    }

    // note on check Cap 4
    if ((currtouched4 & _BV(i)) && !(lasttouched4 & _BV(i))) {
      // play pressed note
      noteOn(channel2, pgm_read_byte(&(pitch[currentPitch][i+36])), vel[i+36]);
    }

    // note off check Cap 4
    if ( !(currtouched4 & _BV(i)) && (lasttouched4 & _BV(i)) ) {
      // play note off
      noteOff(channel2, pgm_read_byte(&(pitch[currentPitch][i+36])), vel[i+36]);
    }

  }

}

// Button press functions
void updateButtons() {

  // Cycle through Button array
  for (byte i = 0; i < NUMBER_BUTTONS; i = i + 1) {
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
    
  }
  
}

// Multiplexer pot functions - send MIDI CC when pots turned
void updateMuxPots() {
  for (byte i = 0; i < NUMBER_MUX_POTS; i = i + 1) {
    if (i < (NUMBER_MUX_POTS / 2)) { 
      MUXPOTS[i]->Pchannel = channel1; 
      } else {
        MUXPOTS[i]->Pchannel = channel2;
        };
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
  display.setCursor(0,8);
  // pull the character string out of the string table in PROGMEM
  strcpy_P(current_scale, (char *)pgm_read_word(&(string_table[currentPitch])));
  display.println(current_scale);
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
      if(menu_count == (MENU_ITEMS - 1)) {
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
      if(menu_count_keys == (MENU_ITEMS_KEYS - 1)) {
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
        //byte sLength = strlen("scales");
        //display.setCursor((SCREEN_WIDTH /2) - (sLength * 6),(SCREEN_HEIGHT / 2) - 8);
        display.setCursor((SCREEN_WIDTH /2) - (6 * 6),(SCREEN_HEIGHT / 2) - 8);
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
        //byte sLength = strlen("keys");
        //display.setCursor((SCREEN_WIDTH /2) - (sLength * 6),(SCREEN_HEIGHT / 2) - 8);
        display.setCursor((SCREEN_WIDTH /2) - (4 * 6),(SCREEN_HEIGHT / 2) - 8);
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
        //byte sLength = strlen("1-48 CH1");
        //display.setCursor((SCREEN_WIDTH /2) - (sLength * 6),(SCREEN_HEIGHT / 2) - 8);
        display.setCursor((SCREEN_WIDTH /2) - (8 * 6),(SCREEN_HEIGHT / 2) - 8);
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
        //byte sLength1 = strlen("1-24 CH1");
        //byte sLength2 = strlen("25-48 CH2");
        //display.setCursor((SCREEN_WIDTH /2) - (sLength1 * 6),(SCREEN_HEIGHT / 2) - 16);
        display.setCursor((SCREEN_WIDTH /2) - (8 * 6),(SCREEN_HEIGHT / 2) - 16);
        display.invertDisplay(true);
        display.println("1-24 CH1");
        //display.setCursor((SCREEN_WIDTH /2) - (sLength2 * 6),(SCREEN_HEIGHT / 2));
        display.setCursor((SCREEN_WIDTH /2) - (9 * 6),(SCREEN_HEIGHT / 2));
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
