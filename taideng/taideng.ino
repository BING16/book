/*
**输入台灯信息的格式："l255e"
*/

#include <SoftwareSerial.h>

SoftwareSerial BT(5, 6);
int led_z = 9;
int led_f = 10;
int ligh = 0;

void set_led(int ligh){
  analogWrite(led_z, ligh);
  if (ligh == 0){
    digitalWrite(led_f, HIGH);
  }
  else{
    digitalWrite(led_f, LOW);
  }
}

void set_led2(int ligh){
  analogWrite(led_z, ligh);
  digitalWrite(led_f, HIGH);
}

void setup(){
  BT.begin(9600);  //设置波特率
  Serial.begin(9600);//这里应该和你的模块通信波特率一致
  
  pinMode(led_z, OUTPUT);
  pinMode(led_f, OUTPUT);
  
  analogWrite(led_z, 255);
  digitalWrite(led_f, HIGH);
}

void loop(){
  if (BT.available()){
    char val = BT.read();
    Serial.println(val);
    if (val == 'l'){
      ligh = 0;
    }
    else if(val == 'e'){
      set_led(ligh);
    }
    else if(val == 'o'){
      set_led2(ligh);
      }
    else {
      ligh = ligh*10 + (val - '0');
    }
  }
}
