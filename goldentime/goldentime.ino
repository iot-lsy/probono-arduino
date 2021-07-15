#include "LedControl.h"
#include <SoftwareSerial.h>

#define RX 2
#define TX 3

SoftwareSerial mySerial(RX, TX);


// Din 핀을 12번, ClK핀을 11번 CS핀을 10번에 연결
// 매트릭스는 1개를 사용 선언
LedControl lc = LedControl(12,11,10,1);

int voice_recogn = 0;

byte data[ 5 ][ 8 ] =
{
// 무표정 이미지
{
B11111111,
B10000001,
B10100101,
B10000001,
B10000001,
B10111101,
B10000001,
B11111111
},
// 웃는 표정 이미지
{
B11111111,
B10000001,
B10100101,
B10000001,
B10100101,
B10111101,
B10000001,
B11111111
},

// 십자가 모양
{
B00011000,
B00011000,
B00011000,
B11111111,
B11111111,
B00011000,
B00011000,
B00011000
},
// 하트 모양
{
B00000000,
B01100110,
B11111111,
B11111111,
B11111111,
B01111110,
B00111100,
B00011000
},
// 동그란 웃는모양
{
B00111100,
B01000010,
B10100101,
B10000001,
B10100101,
B10011001,
B01000010,
B00111100
}
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);

  lc.shutdown(0, false);
  lc.setIntensity(0, 1);
  lc.clearDisplay(0);

  Serial.println("wait settings are in progress");
  delay(1000);
  mySerial.write(0xAA); // compact mode
  mySerial.write(0x37);
  delay(1000);
  mySerial.write(0xAA);
  mySerial.write(0x21);

  Serial.println("The settings are complete");
}

void loop() {
  // put your main code here, to run repeatedly:

  while(mySerial.available()){
    Serial.println("Voice input");
    voice_recogn = mySerial.read();
    Serial.print("voice_recogn:");
    Serial.println(voice_recogn);
    
    switch(voice_recogn){
      case 0x11:
        for(int i=0; i<8; i++){
            lc.setRow(0, i, data[0][i]);
        }
        Serial.println("무표정 이미지 출력");
        break;

      case 0x12:
        for(int i=0; i<8; i++){
          lc.setRow(0, i, data[1][i]);
        }
        Serial.println("웃는 표정 이미지 출력");
        break;

      case 0x13:
        for(int i=0; i<8; i++){
          lc.setRow(0, i, data[2][i]);
        }
        Serial.println("십자가 모양 이미지 출력");
        break;

      case 0x14:
        for(int i = 0 ; i < 8 ; i ++){
          lc.setRow( 0, i, data[3][i]); // 하트 모양 이미지 출력
        }
        Serial.println("하트 모양 이미지 출력");
        break ;
      case 0x15 :
        for(int j=0; j<8; j++){
          lc.setRow( 0 ,j,data[ 4 ][j]); // 동그란 웃는 표정 이미지출력
        }
        Serial.println("동글 웃는 이미지 출력");
        break;
    }
  }
}
