int position_pin[] = {1,2,3,4};               //4자리 결정 핀
int segment_pin[] = {5,6,7,8,9,10,11,12};     //세그먼트 제어 핀
const int delayTime = 5;                      //일시정지 시간

bool timer = true; // 타이머 시작 설정
int buttonPin = 0; //버튼핀 0

//0 ~ 9를 표현하는 세그먼트 값
byte data[] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xE6};

void setup() {
  //4자리 결정 핀 출력용으로 설정
  for(int i = 0; i < 4; i++) {
     pinMode(position_pin[i], OUTPUT);
  }

  //세그먼트 제어 핀 출력용으로 설정
  for(int i = 0; i < 8; i++) {
    pinMode(segment_pin[i], OUTPUT);
  }

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(13, OUTPUT);
}

void loop() {

  minus_count();
  /*
  if(digitalRead(buttonPin) == HIGH){
    timer = true;
    digitalWrite(13, LOW);
    
  } else if(digitalRead(buttonPin) == LOW){
    timer = false;
    digitalWrite(13, HIGH);
  }
  */

}

void show(int position, int number) {
  //4자리 중 원하는 자리 선택
  for(int i = 0; i < 4; i++) {
    if(i + 1 == position){
      digitalWrite(position_pin[i], HIGH);
    } else {
      digitalWrite(position_pin[i], LOW);
    }
  }

  //8개 세그먼트를 제어해서 원하는 숫자 출력
  
  for(int i = 0; i < 8; i++){
     byte segment = (data[number] & (0x01 << i)) >> i;
     if(segment == 0){
       digitalWrite(segment_pin[7 - i], HIGH);
     } else {
       digitalWrite(segment_pin[7 - i], LOW);
     }
  }
}

void minus_count(){
  for(int i = 5; i >= 0; i--) {
    for(int j = 9; j >= 0; j--) {
      for(int k = 9; k >= 0; k--) {
        for(int l = 9; l >= 0; l--) {
          show(1,i);
          delay(delayTime);
          show(2,j);
          delay(delayTime);
          show(3,k);
          delay(delayTime);
          show(4,l);
          delay(delayTime);

          if(digitalRead(buttonPin) == HIGH){
            
            digitalWrite(13, LOW);
            
          } else if(digitalRead(buttonPin) == LOW){
            i=5;
            j = 9;
            k = 9;
            l = 9;
            digitalWrite(13, HIGH);
          }
        }
      }
    }
  }
  delay(1000);  
}

