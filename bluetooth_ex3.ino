#include <SoftwareSerial.h> // 시리얼통신 라이브러리

int blueTx = 4; //Tx (보내는 핀 설정) Digital Pin 4
int blueRx = 5; //Rx (받는 핀 설정) Digital Pin 5

SoftwareSerial mySerial(blueTx, blueRx); //시리얼 통신을 위한 객체선언
String myString=""; // 받는 문자열

void setup() {
  Serial.begin(9600); //시리얼모니터
  mySerial.begin(9600); //블루투스 시리얼 개방
  pinMode(13, OUTPUT); // Pin 13을 OUTPUT 으로 설정(LED ON/OFF)
}

void loop() {
  while(mySerial.available()) // mySerial에 전송된 값이 있으면
  {
    char myChar = (char)mySerial.read(); // mySerial int 값을 char 형식으로 변환
    myString += myChar; // 수신되는 문자를 myString에 모두 붙임(1바이트씩 전송되는 것을 연결)
    delay(5); // 수신 문자열 끊김 방지
  }
  if(!myString.equals("")) // myString 값이 있다면
  {
    Serial.println("input value: " + myString); // 시리얼모니터에 myString값 출력

    if(myString == "on") // myString 값이 "on"이라면
    {
      digitalWrite(13, HIGH); //LED ON
      Serial.println("LED ON");
    } else if (myString == "off") // if myString value is "off"
    {
      digitalWrite(13, LOW); //LED OFF
      Serial.println("LED OFF");
    }

    myString = ""; // myString 변수값 초기화
  }
}
