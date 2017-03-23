#include "MQTT.h";
#include "ArduinoJson.h";

class MQTTClient {
public:
  MQTTClient(MQTT *conn, String devID);
  void subscribeToTopics();
  void publishIntroduction();
  void publishHit();
  void connect();
  void reconnect();
  void tick();
  bool isConnected();

private:
  MQTT *connection;
  String deviceID;

  String darterTopic(const char* lastElement);
};
