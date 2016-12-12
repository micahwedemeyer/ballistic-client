#include "Particle.h";
#include "pitches.h";
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
  } else if(tuneId == "live") {
    melodyIndex = 1;
  } else if(tuneId == "lose") {
    melodyIndex = 2;
  }

  noteIndex = 0;
  tuneInProgress = true;
  noteInProgress = false;
}

void MusicPlayer::tick() {
  if(noteInProgress || !tuneInProgress) {
    return;
  }

  int note = notes[melodyIndex][noteIndex];
  int duration = 1000 / noteDurations[melodyIndex][noteIndex];
  this->playNote(note, duration);
}

void MusicPlayer::playNote(int note, int duration) {
  noteInProgress = true;
  Log.trace("Start Note");

  // Note!! This is non-blocking! The tone is automatically stopped by an interrupt.
  tone(SPEAKER_PIN, note, duration);

  // Include a pause of the length of the note
  int durationWithRest = duration * 2;

  timer->changePeriod(durationWithRest);
  timer->reset();
}

void MusicPlayer::endNote() {
  timer->stop();
  Log.trace("End Note");
  noteInProgress = false;

  noteIndex++;
  if(noteIndex >= noteCounts[melodyIndex]) {
    this->endPlay();
  }
}

void MusicPlayer::endPlay() {
  noteInProgress = false;
  tuneInProgress = false;
}
