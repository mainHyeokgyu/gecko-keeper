#include <Arduino.h>
#include <DHT.h>

#define DHT_PIN     4
#define DHT_TYPE    DHT22
#define RELAY_PIN   5

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
    Serial.begin(115200);
    dht.begin();
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);   // Active HIGH — 초기 OFF
}

void loop() {
    float humidity    = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("DHT22 read failed");
        delay(2000);
        return;
    }

    Serial.printf("Temp: %.1f°C  Humidity: %.1f%%\n", temperature, humidity);
    delay(1000);
}
