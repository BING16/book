#include <LiquidCrystal_I2C.h>
#define trigPin 12
#define echoPin 13
int Photo = A0;//光敏电阻 模拟
int LED = 5;//照明灯

int val;
int val0 = 180;
int touch = 8;//触摸传感器
int touchstate = 0;
int vibration = 9;//震动马达模块 数字引脚
//int bookoc = XXX;//判断书本的开合状态
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
