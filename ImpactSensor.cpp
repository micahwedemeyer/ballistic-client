#include "Particle.h";
#include "ImpactSensor.h";

ImpactSensor::ImpactSensor(int pin, int lpin, int thold, int hitDelayMs, std::function<void(int)> cb) {
  sensorPin = pin;
  ledPin = lpin;
  impactThreshold = thold;
  hitProcessing = false;
  callback = cb;
  hitDelayer = new Timer(hitDelayMs, &ImpactSensor::endProcessHit, *this);
}

void ImpactSensor::tick() {
  if(isHitProcessing()) {
    return;
  }

  checkHitSensor();
}

bool ImpactSensor::isHitProcessing() {
  return hitProcessing;
}

void ImpactSensor::checkHitSensor() {
  int impactSensorReading = analogRead(sensorPin);
  if(isHit(impactSensorReading)) {
    processHit(impactSensorReading);
  }
}

bool ImpactSensor::isHit(int impactReading) {
  return impactReading >= impactThreshold;
}

void ImpactSensor::processHit(int sensorReading) {
  Log.trace("Processing Hit");
  hitProcessing = true;
  hitDelayer->reset();
  digitalWrite(ledPin, HIGH);
  callback(sensorReading);
}

void ImpactSensor::endProcessHit() {
  hitProcessing = false;
  digitalWrite(ledPin, LOW);
}
