#include "Particle.h";
#include "definitions.h";

int noteIndex;

void playNote(int thisNote);
void endPlay();

void nextNote() {
  noteIndex++;
  if(noteIndex < 8) {
    playNote(noteIndex);
  } else {
    endPlay();
  }
}

void playNote(int thisNote) {
  // notes in the melody:
  int melody[] = {1908,2551,2551,2273,2551,0,2024,1908}; //C4,G3,G3,A3,G3,0,B3,C4

  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int noteDurations[] = {4,8,8,4,4,4,4,4 };

  // stop the tone playing:
  noTone(SPEAKER_PIN);

  // to calculate the note duration, take one second
  // divided by the note type.
  //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
  int noteDuration = 1000/noteDurations[thisNote];
  tone(SPEAKER_PIN, melody[thisNote],noteDuration);

  // to distinguish the notes, set a minimum time between them.
  // the note's duration + 30% seems to work well:
  int pauseBetweenNotes = noteDuration * 1.30;
  Timer t(pauseBetweenNotes, nextNote, true);
  t.start();
}

void endPlay() {
  Particle.publish("Tone Test Play End");
}

void toneTest() {
  Particle.publish("Tone Test Play Start");

  noteIndex = 0;
  playNote(noteIndex);
}
