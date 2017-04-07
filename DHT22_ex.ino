#include <DHT.h> // 온습도센서 라이브러리

#define DHTPIN 2 // data pin to DHT22
#define DHTTYPE DHT22 // using DHT 22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
   Serial.begin(9600); // 통신속도 설정

   dht.begin();
}

void loop() {
  
  float tmp = dht.readTemperature();
  float hum = dht.readHumidity();
  
  if(isnan(tmp) || isnan(hum))
  {
    Serial.print("Reading error");
  }
  else
  {
    Serial.print("Temperature:");
    Serial.print(tmp);
    Serial.print("C\t");
    Serial.print(" humidity:");
    Serial.print(hum);
    Serial.println(" %");
  }
  delay(1000); // 1초마다 측정
}
