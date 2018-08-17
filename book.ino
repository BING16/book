#include <LiquidCrystal_I2C.h>
int val;
int val0 = 180;
int touchstate = 0;

//引脚定义
/*
* SD card attached to SPI bus as follows:
** MOSI - pin 11
** MISO - pin 12
** CLK - pin 13x
** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)
*/
int trigPin = 4;  //超声波
int echoPin = 7;  //超声波
int touch = 8;  //触摸传感器
int vibration = 9;  //震动马达模块 数字引脚
int Photo = A0;  //光敏电阻 模拟
SoftwareSerial BT(5, 6);  //新建对象，接收脚为5(对应蓝牙的T)，发送脚为6(对应蓝牙的R)
//wifi模块引脚未定


LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(vibration,OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(Photo, INPUT);
  pinMode(touch, INPUT);
  //pinMode(bookoc, INPUT);
  lcd.init();                      // initialize the lcd
  lcd.init();
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  val = analogRead(Photo);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 0, 255);
  lcd.backlight();
  //delay(15);
  lcd.setCursor(0, 0);
  lcd.setCursor(0, 1);
  lcd.print("luminosity");
  lcd.print(val);
  Serial.print(distance);
  Serial.println(" cm");
  delay(10);
  lcd.clear();
  touchstate = digitalRead(touch);
  if (val <= 150) {
    analogWrite(LED, ~val);

  }
  else {
    analogWrite(LED, LOW);
  }
      if (touchstate == HIGH) //touch-sensor controls the LED
   {
    digitalWrite(LED, HIGH);
   }
  if (distance < 30&& distance>5) {
    digitalWrite(vibration, HIGH);
    delay(100);
  } 
  else{
     digitalWrite(vibration,LOW); }
  //analogWrite(LED, val);
    Serial.println(val);
    Serial.println(~val);
    Serial.println(touchstate);
    delay(10);
}
