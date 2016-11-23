#include "Particle.h";
#include "definitions.h";
#include "MusicPlayer.h";

MusicPlayer::MusicPlayer() {
  timer = new Timer(1000, &MusicPlayer::endNote, *this);
  noteInProgress = false;
  tuneInProgress = false;
}

bool MusicPlayer::playInProgress() {
  return tuneInProgress;
}

void MusicPlayer::playTune(String tuneId) {
  if(noteInProgress) {
    return;
  }

  if(tuneId == "win") {
    melodyIndex = 0;
  }

  if(!tuneInProgress) {
    tuneInProgress = true;
    noteIndex = -1;
    this->tick();
  }
}

void MusicPlayer::tick() {
  if(noteInProgress || !tuneInProgress) {
    return;
  }

  noteIndex++;
  if(noteIndex < noteCounts[melodyIndex]) {
    int note = notes[melodyIndex][noteIndex];
    int duration = noteDurations[melodyIndex][noteIndex];
    this->playNote(note, duration);
  } else {
    this->endPlay();
  }
}

void MusicPlayer::endNote() {
  noteInProgress = false;
}

void MusicPlayer::playNote(int note, int duration) {
  tone(SPEAKER_PIN, note, duration);
  noteInProgress = true;

  // to distinguish the notes, set a minimum time between them.
  // the note's duration + 30% seems to work well:
  int pauseBetweenNotes = duration * 1.30;

  timer->changePeriod(pauseBetweenNotes);
  timer->reset();
}

void MusicPlayer::endPlay() {
  timer->stop();
  noTone(SPEAKER_PIN);
  noteInProgress = false;
  tuneInProgress = false;
}
