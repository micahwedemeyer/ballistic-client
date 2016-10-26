class MusicPlayer {
public:
  MusicPlayer();
  void playTune();

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
