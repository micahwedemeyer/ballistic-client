// I/O Pins
#define SPEAKER_PIN A4
#define NEOPIXEL_PIN A5
#define IMPACT_PIN A0
#define LED_PIN D7

// LED Strip
#define NEOPIXEL_COUNT 60
#define NEOPIXEL_TYPE WS2812
#define NEOPIXEL_BRIGHTNESS 128

// MQTT Broker
// Currently we run Mosquitto MQTT broker on the Kegalytics R-Pi3
#define MQTT_BROKER_HOST_B1 192
#define MQTT_BROKER_HOST_B2 168
//#define MQTT_BROKER_HOST_B3 10
//#define MQTT_BROKER_HOST_B4 105
#define MQTT_BROKER_HOST_B3 11
#define MQTT_BROKER_HOST_B4 99
#define MQTT_BROKER_PORT 1883
#define MQTT_HITS_TOPIC "hits"

// Various Constants
#define IMPACT_THRESHOLD 700
#define HIT_DELAY_MS 300
