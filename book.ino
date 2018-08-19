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
SoftwareSerial BT_led(3, 6);  //新建对象，接收脚为3(对应蓝牙的T)，发送脚为6(对应蓝牙的R)
SoftwareSerial BT_brain(5, 10);   //新建对象，接收脚为5(对应蓝牙的T)，发送脚为10(对应蓝牙的R)
//wifi模块引脚未定

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void send_ligh(int ligh);
void get_brain();
void show_lcd(String l);
int get_distance();
int get_ligh();

int get_distance(){
	long duration, distance;
	digitalWrite(trigPin, LOW);  // Added this line
	delayMicroseconds(2); // Added this line
	digitalWrite(trigPin, HIGH);
  
	delayMicroseconds(10); // Added this line
	digitalWrite(trigPin, LOW);
	duration = pulseIn(echoPin, HIGH);
	distance = (duration / 2) / 29.1;
	
	return(distance);
}

int get_ligh(){
	val = analogRead(Photo);            // reads the value of the potentiometer (value between 0 and 1023)
	val = map(val, 0, 1023, 0, 255);
}

void show_lcd(String l){
	lcd.backlight();
	
	lcd.setCursor(0, 0);
	lcd.setCursor(0, 1);
	lcd.print("luminosity");
	lcd.print(l);
	Serial.print(distance);
	Serial.println(" cm");
	delay(10);
	lcd.clear();
}

void setup() {
  // put your setup code here, to run once:
	
	Serial.begin(9600);
	BT_brain.begin(57600);
	BT_led.begin(9600);
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
	pinMode(vibration,OUTPUT);
	pinMode(Photo, INPUT);
	pinMode(touch, INPUT);
	lcd.init();                      // initialize the lcd
}

void loop() {
	long distance = get_distance();
	
	touchstate = digitalRead(touch);
	if (touchstate == HIGH){   //按下才亮灯
		show_lcd(String (val));
		if (val <= 150) {
			send_ligh(val);
		}
		else {
			send_ligh(0);
		}
	}
	else{
		send_ligh(0);
	}
	

	if (distance < 30 && distance>5) {
		digitalWrite(vibration, HIGH);
	} 
	else{
		digitalWrite(vibration,LOW); 
	}
	
	
}
