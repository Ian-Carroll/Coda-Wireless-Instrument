#include "IRremote.h"
#include "IRremoteInt.h"
#include <LiquidCrystal.h>

// Initialize arrays of note frequencies and names, starts at note C7
const int notes[13] = {2125, 2217, 2349, 2489, 2637, 2793, 2959, 3135, 3322, 3520, 3729, 3951, 4186};
const char* note_names[13] = {"C7", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "C8"};

// Initialize array of remote inputs which matches notes array
const long long int remote_inputs[13] = {16728765,16730805,16732845,16716015,16726215,16734885,16724175,16718055,16743045,16738455,16750695,16756815,16769055};

// Assign LCD pins
const int d7=2,d6=3,d5=4,d4=5,en=11,rs=12;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

// Assign IR sensor, and passive buzzer pins 
const int ir_recv_pin=8,buzzer=9;

// Set up IR receiver sensor
IRrecv irrecv(ir_recv_pin);
decode_results results;

int last_note;

void setup() {
  pinMode(ir_recv_pin, INPUT);
  lcd.begin(16,2);
  irrecv.enableIRIn();
}

void loop() {
  lcd.setCursor(0,0);
  lcd.print("Last note: ");

  // If signal detected from IR sensor, device actuations occur
  if (irrecv.decode(&results))
  {
    // Loop through array of notes and play them if it matches the remote input
    for (int i=0; i<13; i++)
    {
      if (results.value == remote_inputs[i])
      {
        // Clear the screen and print appropriate note
        lcd.clear();
        lcd.print("Last note: ");
        lcd.print(note_names[i]);
        
        last_note = notes[i];    // Assign last note in case of holding a note

        // Play note and reset lcd
        tone(buzzer, notes[i]);
        delay(150);
        noTone(buzzer);
        lcd.setCursor(0,0);
      }
    }

    // If the IR sensor receives the hold signal, hold the last note played
    if (results.value == 4294967295)
    {
      tone(buzzer, last_note);
      delay(150);
      noTone(buzzer);
    }
    irrecv.resume();
  }

  else
  {
    noTone(buzzer);
  }
}
