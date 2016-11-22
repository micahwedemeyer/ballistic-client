#include "Particle.h";

class ImpactSensor {
public:
  ImpactSensor(int sensorPin, int ledPin, int thold, int hitDelayMs, std::function<void()> callback);
  bool isHitProcessing();
  void tick();

private:
  int sensorPin;
  int ledPin;
  int impactThreshold;
  bool hitProcessing;
  Timer *hitDelayer;
  std::function<void()> callback;

  void checkHitSensor();
  bool isHit(int impactReading);
  void processHit();
  void endProcessHit();
};
