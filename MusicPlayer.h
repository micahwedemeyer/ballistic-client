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

  int noteCounts[1] = {5};
  int notes[1][8] = {
    {1908,2551,2551,2273,2551}
  };
  int noteDurations[1][8] = {
    {4,8,8,4,4,4,4,4}
  };

  void playNote(int note, int duration);
  void endNote();
  void endPlay();
};
