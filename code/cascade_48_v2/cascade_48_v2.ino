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
#include "Controller.h"

//---How many buttons are connected directly to pins?---------
byte NUMBER_BUTTONS = 2;
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

Button BU1(9, 1, 60, 1, 5 ); //Button 1
Button BU2(10, 1, 61, 1, 5 ); //Button 2
Button *BUTTONS[] {&BU1, &BU2};

Button *MUXBUTTONS[] {};

Pot MPO01(M1, 0, 0, 45, 1); //Potentiometer 01
Pot MPO02(M1, 1, 0, 46, 1); //Potentiometer 02
Pot MPO03(M1, 2, 0, 47, 1); //Potentiometer 03
Pot MPO04(M1, 3, 0, 48, 1); //Potentiometer 04
Pot MPO05(M1, 4, 0, 49, 1); //Potentiometer 05
Pot MPO06(M1, 5, 0, 50, 1); //Potentiometer 06
Pot MPO07(M1, 6, 0, 51, 1); //Potentiometer 07
Pot MPO08(M1, 7, 0, 52, 1); //Potentiometer 08
Pot MPO09(M1, 8, 0, 53, 1); //Potentiometer 09
Pot MPO10(M1, 9, 0, 54, 1); //Potentiometer 10
Pot MPO11(M1, 10, 0, 55, 1); //Potentiometer 11
Pot MPO12(M1, 11, 0, 56, 1); //Potentiometer 12
Pot MPO13(M1, 12, 0, 57, 1); //Potentiometer 13
Pot MPO14(M1, 13, 0, 58, 1); //Potentiometer 14
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

// prime dynamic values
int channel = 0;

// C3 major ( C, D, E, F, G, A, B, C )
// F# / Gb Major ( F#, G#, A#, B, C#, D#, F, F# )
// E4 major ( E, F#, G#, A, B, C#, D#, E )
// Dm Pentatonic Scale ( D, F, G, A, C, D )

int pitch[][48] = { 
                    {12, 14, 16, 17, 19, 21, 23, 24, 26, 28, 29, 31, 33, 35, 36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67, 69, 71, 72, 74, 76, 77, 79, 81, 83, 84, 86, 88, 89, 91, 93},
                    {30, 32, 34, 35, 37, 39, 41, 42, 44, 46, 47, 49, 51, 53, 54, 56, 58, 59, 61, 63, 65, 66, 68, 70, 71, 73, 75, 77, 78, 80, 82, 83, 85, 87, 89, 90, 92, 94, 95, 97, 99, 101, 102, 104, 106, 107, 109, 111},
                    {28, 30, 32, 33, 35, 37, 39, 40, 42, 44, 45, 47, 49, 51, 52, 54, 56, 57, 59, 61, 63, 64, 66, 68, 69, 71, 73, 75, 76, 78, 80, 81, 83, 85, 87, 88, 90, 92, 93, 95, 97, 99, 100, 102, 104, 105, 107, 109},
                    {14, 17, 19, 21, 24, 26, 29, 31, 33, 36, 38, 41, 43, 45, 48, 50, 53, 55, 57, 60, 62, 65, 67, 69, 72, 74, 77, 79, 81, 84, 86, 89, 91, 93, 96, 98, 101, 103, 105, 108, 110, 113, 115, 117, 120, 122, 125, 127}
                  };

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
      noteOn(channel, pitch[3][i], vel[i]);

    }

    // note off check Cap 1
    if ( !(currtouched1 & _BV(i)) && (lasttouched1 & _BV(i)) ) {

      // play note off
      noteOff(channel, pitch[3][i], vel[i]);

    }

    // note on check Cap 2
    if ((currtouched2 & _BV(i)) && !(lasttouched2 & _BV(i))) {

      // play pressed note
      noteOn(channel, pitch[3][i+12], vel[i+12]);

    }

    // note off check Cap 2
    if ( !(currtouched2 & _BV(i)) && (lasttouched2 & _BV(i)) ) {

      // play note off
      noteOff(channel, pitch[3][i+12], vel[i+12]);

    }

    // note on check Cap 3
    if ((currtouched3 & _BV(i)) && !(lasttouched3 & _BV(i))) {

      // play pressed note
      noteOn(channel, pitch[3][i+24], vel[i+24]);

    }

    // note off check Cap 3
    if ( !(currtouched3 & _BV(i)) && (lasttouched3 & _BV(i)) ) {

      // play note off
      noteOff(channel, pitch[3][i+24], vel[i+24]);

    }

    // note on check Cap 4
    if ((currtouched4 & _BV(i)) && !(lasttouched4 & _BV(i))) {

      // play pressed note
      noteOn(channel, pitch[3][i+36], vel[i+36]);

    }

    // note off check Cap 4
    if ( !(currtouched4 & _BV(i)) && (lasttouched4 & _BV(i)) ) {

      // play note off
      noteOff(channel, pitch[3][i+36], vel[i+36]);

    }

  }

}

//*****************************************************************
void updateButtons() {

  // Cycle through Button array
  for (int i = 0; i < NUMBER_BUTTONS; i = i + 1) {
    byte message = BUTTONS[i]->getValue();

    //  Button is pressed
    if (message == 0) {
      switch (BUTTONS[i]->Bcommand) {
        case 0: //Note
          //noteOn(BUTTONS[i]->Bchannel, BUTTONS[i]->Bvalue, 127 );
          break;
        case 1: //CC
          //controlChange(BUTTONS[i]->Bchannel, BUTTONS[i]->Bvalue, 127);
          break;
        case 2: //Toggle
          if (BUTTONS[i]->Btoggle == 0) {
            //controlChange(BUTTONS[i]->Bchannel, BUTTONS[i]->Bvalue, 127);
            BUTTONS[i]->Btoggle = 1;
          }
          else if (BUTTONS[i]->Btoggle == 1) {
            //controlChange(BUTTONS[i]->Bchannel, BUTTONS[i]->Bvalue, 0);
            BUTTONS[i]->Btoggle = 0;
          }
          break;
      }
    }

    //  Button is not pressed
    if (message == 1) {
      
      switch (BUTTONS[i]->Bcommand) {
        case 0:
          //noteOff(BUTTONS[i]->Bchannel, BUTTONS[i]->Bvalue, 0);
          break;
        case 1:
          //controlChange(BUTTONS[i]->Bchannel, BUTTONS[i]->Bvalue, 0);
          break;
      }
    }
  }
}

//*******************************************************************
void updateMuxButtons() {

  // Cycle through Mux Button array
  for (int i = 0; i < NUMBER_MUX_BUTTONS; i = i + 1) {

    MUXBUTTONS[i]->muxUpdate();
    byte message = MUXBUTTONS[i]->getValue();

  }

}

//***********************************************************************
void updatePots() {
  for (int i = 0; i < NUMBER_POTS; i = i + 1) {
    byte potmessage = POTS[i]->getValue();
    if (potmessage != 255) controlChange(POTS[i]->Pchannel, POTS[i]->Pcontrol, potmessage);
  }
}

//***********************************************************************
void updateMuxPots() {
  for (int i = 0; i < NUMBER_MUX_POTS; i = i + 1) {
    MUXPOTS[i]->muxUpdate();
    byte potmessage = MUXPOTS[i]->getValue();
    if (potmessage != 255) controlChange(MUXPOTS[i]->Pchannel, MUXPOTS[i]->Pcontrol, potmessage);
  }
}
