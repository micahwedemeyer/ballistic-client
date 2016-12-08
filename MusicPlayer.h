#include "pitches.h";

class MusicPlayer {
public:
  MusicPlayer();
  bool playInProgress();
  void playTune(String tuneId);
  void tick();

private:
  Timer *timer;
  bool noteInProgress;
  bool tuneInProgress;
  int noteIndex;
  int melodyIndex;

  int noteCounts[2] = {5,3};
  int notes[2][9] = {
    {NOTE_AS6, NOTE_DS7, NOTE_DS7, NOTE_CS7, NOTE_DS7}, // Hit
    {NOTE_D5, NOTE_D5, NOTE_D7} // Go Live
    // {0,0,0,0,0,0,0,0,0}, // Hit
    // {0,0,0,0,0  } // Go Live

  };
  int noteDurations[2][9] = {
    {4,8,8,4,4},
    {8,8,2}
  };

  void playNote(int note, int duration);
  void endNote();
  void endPlay();
};
