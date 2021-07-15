#include <SoftwareSerial.h>

#define voice_recogn_RX 2
#define voice_recogn_TX 3
#define button_IN 12
#define pir_IN 10
#define bluetooth_RX 6
#define bluetooth_TX 7

#define voice_emergency 111 // 음성 응급호출 111 전송
#define button_emergency 112 // 버튼 응급호출 112 전송
#define person_in 121 // pir센서 들어오는 경우 121 전송
#define person_out 122 // pir센서 나가는 경우 122 전송

SoftwareSerial mySerial_voice(voice_recogn_RX, voice_recogn_TX);
SoftwareSerial BT(bluetooth_RX, bluetooth_TX);

int voice_input_hex = 0;
byte buffer[128];

void start_voice_recogn();
void voice_recogn();
void button_pushed();
void pir_recogn();
void start_bluetooth();

void setup() {
  
  pinMode(button_IN, INPUT);
  pinMode(pir_IN, INPUT);
  Serial.begin(9600);
  BT.begin(9600);
  mySerial_voice.begin(9600);
  set_voice_recogn();
  
}

void loop() {

  if(BT.available()){
  voice_recogn();
  button_pushed();
  pir_recogn();
  }
  
  delay(1000);
}

void set_voice_recogn(){ // 음성인식 모듈 활성화
  Serial.println("wait settings are in progress");
  delay(1000);
  mySerial_voice.write(0xAA); // compact mode
  mySerial_voice.write(0x37);
  delay(1000);
  mySerial_voice.write(0xAA);
  mySerial_voice.write(0x21);

  Serial.println("The settings are complete");
}

void voice_recogn(){ // 음성인식 
  while(mySerial_voice.available()){
    Serial.println("Voice input");
    voice_input_hex = mySerial_voice.read();
    Serial.print("voice_recogn:");
    Serial.println(voice_input_hex);
    
    switch(voice_input_hex){
      case 0x11:
      case 0x12:
      case 0x13:
      case 0x14:
      case 0x15 :
        Serial.println("음성 응급호출");
        BT.write(buffer, voice_emergency);
        break;
    }
  }
}

void button_pushed(){ // 응급호출 버튼
  int value = digitalRead(button_IN);

  if(value == HIGH){
    Serial.println("Emergency Button Pushed");
    BT.write(buffer, button_emergency);
  }
}

void pir_recogn(){ // (!!!! 수정 필요<계속 감지되는 경우에 대한 예외처리 해야함> !!!!)
  int value = digitalRead(pir_IN);
  int count = 0;

  if(value == HIGH && count==0){ // 들어오는 경우
    Serial.println("Movement recognition");
    BT.write(buffer, person_in);
    count++;
  }else if(value == HIGH && count!=0){ // 나가는 경우
    Serial.println("Movement recognition");
    BT.write(buffer, person_out);
    count = 0;
  }
}
