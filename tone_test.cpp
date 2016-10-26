#include "Particle.h";
#include "definitions.h";
#include "tone_test.h";

MusicPlayer::MusicPlayer() {
  timer = new Timer(0, &MusicPlayer::endNote, *this);
  noteInProgress = false;
  tuneInProgress = false;
  noteCount = 8;
}

void MusicPlayer::playTune() {
  if(noteInProgress) {
    return;
  }

  if(!tuneInProgress) {
    Particle.publish("Tone Test Play Start");
    tuneInProgress = true;
    noteIndex = 0;
    this->playNote(noteIndex);
    return;
  }

  if(!noteInProgress) {
    noteIndex++;
    if(noteIndex < noteCount) {
      this->playNote(noteIndex);
    } else {
      this->endPlay();
    }
  }
}

void MusicPlayer::endNote() {
  noteInProgress = false;
}

void MusicPlayer::playNote(int thisNote) {
  // notes in the melody:
  int melody[] = {1908,2551,2551,2273,2551,0,2024,1908}; //C4,G3,G3,A3,G3,0,B3,C4
  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int noteDurations[] = {4,8,8,4,4,4,4,4 };
  int noteCount = 8;

  noteInProgress = true;
  Particle.publish("Note Play", String(thisNote));

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

  timer->dispose();
  timer = new Timer(pauseBetweenNotes, &MusicPlayer::endNote, *this);
  timer->start();
}

void MusicPlayer::endPlay() {
  noTone(SPEAKER_PIN);
  timer->stop();
  timer->dispose();
  noteInProgress = false;
  tuneInProgress = false;
  Particle.publish("Tone Test Play End");
}
