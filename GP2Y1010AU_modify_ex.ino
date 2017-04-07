int dustSensorMeasurePin = 0; //Connect dust sensor to Arduino A0 pin
int dustSensorLedPower = 2; //Connect 3 led driver pins of dust sensor to Arduino D2

void setup(){

  Serial.begin(9600);
  pinMode(dustSensorLedPower,OUTPUT);
}

 void loop(){

  float dustValue;

  if(getDustValue(dustValue)) {
    Serial.print("Dust Density: ");
    Serial.println(dustValue);
  }
  

  delay(1000);
}

float getDustValue(float &dustValue){

  // 5초 평균의 먼지 센서 값을 받아오기 위한 count
  static int count;
  int dustSensorSamplingTime = 280;
  int dustSensorDeltaTime = 40;
  int dustSensorSleepTime = 9680;

  float voMeasured = 0;
  float calcVoltage = 0;
  float dustDensity = 0;

  // when count is 0, then initialize dustValue
  if(count == 0)
    dustValue = 0;
  
  digitalWrite(dustSensorLedPower,LOW); // dust sensor power on the LED
  delayMicroseconds(dustSensorSamplingTime);

  voMeasured = analogRead(dustSensorMeasurePin); // read the dust value

  delayMicroseconds(dustSensorDeltaTime);
  digitalWrite(dustSensorLedPower,HIGH); // turn the LED off
  delayMicroseconds(dustSensorSleepTime);

  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (5.0 / 1024.0);
  
  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  dustDensity = (0.17 * calcVoltage - 0.1) * 1000;

  dustValue += dustDensity;

  // unit: mg/m3 
  // 1mg = 1000μg(micro gram)
  
  if(++count == 1){ // 5번 평균
    dustValue /= count;
    count = 0;
    return dustValue;
  } else {
    return 0;
  }
}

