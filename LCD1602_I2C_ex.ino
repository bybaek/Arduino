#include <Wire.h>
#include <LiquidCrystal_I2C.h> // 라이브러리 관리에서 검색 후 추가

// I2C 주소 초기값 0x27
// SDA A4(Analog pin 4)
// SCL A5(Analog pin 5)
LiquidCrystal_I2C lcd(0x27,16,2); // set LCD address library 


void setup() {
  lcd.init();

  // Print a message to the LCD.
  lcd.backlight(); // turn on backlight
  lcd.print("Hello, world!");
}

void loop() {
  // put your main code here, to run repeatedly:

}
