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
// and Arduino Pro Micro 32u4 (Leonardo)
// 
// Uses the MIDIUSB library: https://github.com/arduino-libraries/MIDIUSB
// Uses the Adafruit MPR121 library: https://github.com/adafruit/Adafruit_MPR121
//
// ---------------------------------------------------------------------------

#include "MIDIUSB.h"          // MIDIUSB library
#include "Wire.h"             // I2C data bus library
#include "Adafruit_MPR121.h"  // touch sensor library

// The MPR121 will use the IRQ to let the controller know something has changed
// *** BE SURE TO CUT THE ADDR TRACE ON THE BACK OF THE BOARD FOR ALL BUT THE FIRST SENSOR!!! ***

#define BUTTONS  12           // defins the number of electrodes on each touch sensor
#define IRQ_PIN1  6           // ADDR not connected is I2C address: 0x5A
#define IRQ_PIN2  7           // ADDR tied to 3V is I2C address: 0x5B
#define IRQ_PIN3  8           // ADDR tied to SDA is I2C address: 0x5C
#define IRQ_PIN4  9           // ADDR tied to SCL is I2C address: 0x5D

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

// prime dynamic values
int channel = 0;

// C3 major ( C, D, E, F, G, A, B, C )
//int pitch[] = {36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67, 69, 71, 72, 74, 76};

// F# / Gb Major ( F#, G#, A#, B, C#, D#, F, F# )
//int pitch[] = {66, 68, 70, 71, 73, 75, 77, 78, 80, 82, 83, 85, 87, 89, 90, 92, 94, 95, 97, 99, 101, 102, 104, 106};

// E4 major ( E, F#, G#, A, B, C#, D#, E ) 
//int pitch[] = {52, 54, 56, 57, 59, 61, 63, 64, 66, 68, 69, 71, 73, 75, 76, 78, 80, 81, 83, 85, 87, 88, 90, 92};

// Dm Pentatonic Scale ( D, F, G, A, C, D )
//int pitch[] = {14, 17, 19, 21, 24, 26, 29, 31, 33, 36, 38, 41, 43, 45, 48, 50, 53, 55, 57, 60, 62, 65, 67, 69, 72, 74, 77, 79, 81, 84, 86, 89, 91, 93, 96, 98, 101, 103, 105, 108, 110, 113, 115, 117, 120, 122, 125, 127};

// Dm Scale ( D, E, F, G, A, A#, C ) = ( 2, 4, 5, 7, 9, 10, 0 )
//int pitch[] = {26, 28, 29, 31, 33, 34, 36, 38, 40, 41, 43, 45, 46, 48, 50, 52, 53, 55, 57, 58, 60, 62, 64, 65, 67, 69, 70, 72, 74, 76, 77, 79, 81, 82, 84, 86, 88, 89, 91, 93, 94, 96, 98, 100, 101, 103, 105, 106};

// D Minor Pentatonic Scale ( D, F, G, A, C ) = ( 2, 5, 7, 9, 0 )
int pitch[] = {2, 5, 7, 9, 12, 14, 17, 19, 21, 24, 26, 29, 31, 33, 36, 38, 41, 43, 45, 48, 50, 53, 55, 57, 60, 62, 65, 67, 69, 72, 74, 77, 79, 81, 84, 86, 89, 91, 93, 96, 98, 101, 103, 105, 108, 110, 113, 115};

int vel[] = {80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80};

void setup() {

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

  for (uint8_t i = 0; i < BUTTONS; i++) {

    // note on check Cap 1
    if ((currtouched1 & _BV(i)) && !(lasttouched1 & _BV(i))) {

      // play pressed note
      noteOn(channel, pitch[i], vel[i]);

    }

    // note off check Cap 1
    if ( !(currtouched1 & _BV(i)) && (lasttouched1 & _BV(i)) ) {

      // play note off
      noteOff(channel, pitch[i], vel[i]);

    }

    // note on check Cap 2
    if ((currtouched2 & _BV(i)) && !(lasttouched2 & _BV(i))) {

      // play pressed note
      noteOn(channel, pitch[i+12], vel[i+12]);

    }

    // note off check Cap 2
    if ( !(currtouched2 & _BV(i)) && (lasttouched2 & _BV(i)) ) {

      // play note off
      noteOff(channel, pitch[i+12], vel[i+12]);

    }

    // note on check Cap 3
    if ((currtouched3 & _BV(i)) && !(lasttouched3 & _BV(i))) {

      // play pressed note
      noteOn(channel, pitch[i+24], vel[i+24]);

    }

    // note off check Cap 3
    if ( !(currtouched3 & _BV(i)) && (lasttouched3 & _BV(i)) ) {

      // play note off
      noteOff(channel, pitch[i+24], vel[i+24]);

    }

    // note on check Cap 4
    if ((currtouched4 & _BV(i)) && !(lasttouched4 & _BV(i))) {

      // play pressed note
      noteOn(channel, pitch[i+36], vel[i+36]);

    }

    // note off check Cap 4
    if ( !(currtouched4 & _BV(i)) && (lasttouched4 & _BV(i)) ) {

      // play note off
      noteOff(channel, pitch[i+36], vel[i+36]);

    }

  }

}

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
