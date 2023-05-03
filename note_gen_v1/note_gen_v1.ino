///////////////////////////////
// MIDI scale generator
// v1.0
// david cool
// 2021
///////////////////////////////

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

// EXAMPLES:
// eDm Pentatonic D, F, G, A, C = 2, 5, 7, 9, 0
// E minor E, F#, G, A, B, C, D = 4, 6, 7, 9, 11, 0, 2

#define KEYBOARD_NOTES 48   // number of notes on your keyboard
#define START_OCTIVE 0      // starting octive (0-10)
#define SCALE_NOTES 5       // number of notes in your scale
byte scale[SCALE_NOTES] = { 7, 8, 0, 1, 5 }; // scale as pitch class set

byte current_octive = START_OCTIVE;
byte note_count = 0;
byte loops = 11 - START_OCTIVE;        // MIDI has a total range of 11 octaves
byte midi_notes_map[11][12] = {
  {0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11},
  {12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23},
  {24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35},
  {36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47},  
  {48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59},
  {60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71},
  {72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83},
  {84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95},
  {96,  97,  98,  99,  100, 101, 102, 103, 104, 105, 106, 107},
  {108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119},
  {120, 121, 122, 123, 124, 125, 126, 127}
};

// final array of notes for the keyboard
byte output_notes[KEYBOARD_NOTES];

void setup() {

  Serial.begin(9600);
  while (!Serial) {
        ; // wait for serial port to connect. Needed for Leonardo only
    } 
  
  Serial.println("Creating note set... ");

  for (byte i = 0; i < loops; i = i + 1) {
    for (byte j = 0; j < SCALE_NOTES; j = j + 1) {
      if ( scale[j] < scale[0] ) {
        output_notes[note_count] = midi_notes_map[current_octive + 1][scale[j]];
      } else {
        output_notes[note_count] = midi_notes_map[current_octive][scale[j]];
      }
      note_count++;
    }
    current_octive++;
  }

  Serial.print("{ ");
  for (byte i = 0; i < KEYBOARD_NOTES; i = i + 1) {
   Serial.print(output_notes[i]);
   if (i == KEYBOARD_NOTES - 1) {
   } else {
    Serial.print(", ");
   }
  }
  Serial.print(" }");
  Serial.println();
  Serial.println("Finished!");

}

void loop() {
  // silence is golden
}
