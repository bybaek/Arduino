/*
Use Sensors and LCD : 
GP2Y1010AU(Dust Sensor), 
DHT22(Temperature Humidity Sensor),
16x2LCD(LiquidCrystalDisplay) & LCD Interface Converter
*/

// NAME AirQuality
// PIN 0000

#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <SoftwareSerial.h>

#define DHTPIN 3 // Connect DHT22 sensor to Arduino Digital Pin 3
#define BLUETOOTHTXD 4 // Connect Bluetooth TXD Digital Pin 4
#define BLUETOOTHRXD 5 // Connect Bluetooth RXD Digital Pin 5
#define DHTTYPE DHT22 // Connect DHT22 sensor Type to Arduino
#define DUSTSENSORMEASUREPIN 0 // Connect dust sensor to Arduino Analog Pin 0
#define DUSTSENSORLEDPOWERPIN 2 // Connect 3 led driver pins of dust sensor to Arduino Digital Pin 2

// I2C 주소 초기값 0x27
// SDA A4(Analog Pin 4)
// SCL A5(Analog Pin 5)
LiquidCrystal_I2C lcd(0x27,16,2); // set LCD address library

// Temperature, Humidity Sensor DHT 22 
DHT dht(DHTPIN, DHTTYPE);

// Bluetooth
SoftwareSerial BTSerial(BLUETOOTHTXD, BLUETOOTHRXD);

void setup()
{

  // 특수 문자는 LCD에서 지원해주지 않으므로 직접 제작하여 사용.
  // ℃ 표현 문자
  byte SpecialChar0[8] = {
    B11000,
    B11000,
    B00110,
    B01001,
    B01000,
    B01000,
    B01001,
    B00110
  };

  // μg(마이크로 그램)/㎥
  //////////////////////////////////// start
  byte SpecialChar1[8] = {
    B00000,
    B00000,
    B00000,
    B10001,
    B10001,
    B10001,
    B11110,
    B10000
  };

  byte SpecialChar2[8] = {
    B00000,
    B00000,
    B00000,
    B01111,
    B10001,
    B01111,
    B00001,
    B11110
  };

  byte SpecialChar3[8] = {
    B01100,
    B10010,
    B00100,
    B10010,
    B01100,
    B00000,
    B00000,
    B00000
  };
  //////////////////////////////////// end
  
  // set up the LCD's number of columns and rows:
  //lcd.begin(16,2);
  lcd.init();

  // Print a message to the LCD
  lcd.backlight(); // turn on backlight

  // set createChar
  lcd.createChar(0, SpecialChar0);
  lcd.createChar(1, SpecialChar1);
  lcd.createChar(2, SpecialChar2);
  lcd.createChar(3, SpecialChar3);
  
  pinMode(DUSTSENSORLEDPOWERPIN,OUTPUT);

  dht.begin();

  BTSerial.begin(9600); // BluetoothSerial
}

void loop()
{

  float dustValue;
  float tmp = dht.readTemperature();
  float hum = dht.readHumidity();
 
  if(isnan(tmp) || isnan(hum))
  {
    lcd.setCursor(0,0);
    lcd.print("temperature, humidity error");
  } 
  else 
  {
    lcd.setCursor(0,0);
    lcd.print(tmp);
    lcd.write(byte(0));
    lcd.print("   ");
    lcd.print(hum);
    lcd.print("%   ");
  }

  if(getDustValue(dustValue)) 
  {
    lcd.setCursor(0,1);
    lcd.print("   ");
    lcd.print(dustValue);
    lcd.write(byte(1));
    lcd.write(byte(2));
    lcd.print("/m");
    lcd.write(byte(3));
    lcd.print("    ");
    BTSerial.print("|tempValue:");
    BTSerial.print(tmp);
    BTSerial.print("|humValue:");
    BTSerial.print(hum);
    BTSerial.print("|dustValue:");
    BTSerial.print(dustValue);
    BTSerial.print('_');
  }
  delay(500);
}

int getDustValue(float &dustValue)
{

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
  
  digitalWrite(DUSTSENSORLEDPOWERPIN,LOW); // dust sensor power on the LED
  delayMicroseconds(dustSensorSamplingTime);

  voMeasured = analogRead(DUSTSENSORMEASUREPIN); // read the dust value

  delayMicroseconds(dustSensorDeltaTime);
  digitalWrite(DUSTSENSORLEDPOWERPIN,HIGH); // turn the LED off
  delayMicroseconds(dustSensorSleepTime);

  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (5.0 / 1024.0);
  
  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  dustDensity = (0.17 * calcVoltage - 0.1) * 1000;

  dustValue += dustDensity;

  // unit: mg/m3 
  // 1mg = 1000μg(micro gram)
  if(++count == 5)
  {
    dustValue /= count; // 평균 dustValue 
    count = 0;
    return 1;
  } 
  else 
  {
    return 0;
  }
}

void reset()
{
  asm volatile(" jmp 0");
}

