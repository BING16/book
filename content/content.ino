#include "LiquidCrystal_I2C.h"
#include "dht11.h"
dht11 DHT11;

#define DHT11PIN 2
int dustPin=0;
float dustVal=0;
int ledPower=3;
int delayTime=280;
int delayTime2=40;
float offTime=9680;
float calcVoltage=0;
float dustDensity=0;
int sensorPin = A1;
int sensorValue = 0; 
int ledPin = 6;
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// 露点（点在此温度时，空气饱和并产生露珠）
// 参考: http://wahiduddin.net/calc/density_algorithms.htm 
double dewPoint(double celsius, double humidity)
{
        double A0= 373.15/(273.15 + celsius);
        double SUM = -7.90298 * (A0-1);
        SUM += 5.02808 * log10(A0);
        SUM += -1.3816e-7 * (pow(10, (11.344*(1-1/A0)))-1) ;
        SUM += 8.1328e-3 * (pow(10,(-3.49149*(A0-1)))-1) ;
        SUM += log10(1013.246);
        double VP = pow(10, SUM-3) * humidity;
        double T = log(VP/0.61078);   // temp var
        return (241.88 * T) / (17.558-T);
}

// 快速计算露点，速度是5倍dewPoint()
// 参考: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
        double a = 17.271;
        double b = 237.7;
        double temp = (a * celsius) / (b + celsius) + log(humidity/100);
        double Td = (b * temp) / (a - temp);
        return Td;
}

void setup()
{ 
  Serial.begin(9600);
   pinMode(6, OUTPUT);
   lcd.init();
   lcd.backlight();                     // initialize the lcd 
   // Print a message to the LCD.
    pinMode(ledPower,OUTPUT);
    pinMode(dustPin,INPUT);
    pinMode(sensorPin,INPUT);
}


void loop()
{
     lcd.init();
   lcd.backlight();                     // initialize the lcd 
    //sensorValue = analogRead(sensorPin);
    //Serial.println(sensorValue);
    int i = random();
  //if (sensorValue >= 400){
  //  sensorValue = sensorValue *255/1024;
    analogWrite(ledPin,i%255 );
  //delay(500);
  //}else{
  //   digitalWrite(ledPin, LOW);}
  //Serial.println(sensorValue, DEC);
  
  int chk = DHT11.read(DHT11PIN);
//
digitalWrite(ledPower,LOW);
delayMicroseconds(delayTime);
dustVal=analogRead(dustPin);
delayMicroseconds(delayTime2);
digitalWrite(ledPower,HIGH);
delayMicroseconds(offTime);
calcVoltage=dustVal*(5.0/1024.0);
dustDensity=0.17*calcVoltage-0.1;
dustDensity=abs(dustDensity*1000);
//
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("PM2.5(ug/m3):");
   if(dustDensity>0){
   lcd.setCursor(10,1);
   lcd.print(String(dustDensity));}
   lcd.setCursor(13,0);
   delay(1500);
  //温度显示
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("TEMP:");
   lcd.setCursor(10,0);
   lcd.print(String(DHT11.temperature - 2));
   lcd.setCursor(13,0);
   lcd.print("oC");
  //湿度显示
   lcd.setCursor(0,1);
   lcd.print("humidity:");
   lcd.setCursor(10,1);
   lcd.print(String(DHT11.humidity));
   lcd.setCursor(14,1);
   lcd.print("%");
   delay(1500);
}
