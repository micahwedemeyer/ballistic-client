class MusicPlayer {
public:
  MusicPlayer();
  bool playInProgress();
  void playTune();
  void tick();

private:
  Timer *timer;
  bool noteInProgress;
  bool tuneInProgress;
  int noteIndex;
  int noteCount;

  void playNote(int thisNote);
  void endNote();
  void endPlay();
};
