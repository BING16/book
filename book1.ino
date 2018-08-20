#include <SoftwareSerial.h>
#include <SD.h>

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
#define BAUDRATE 57600
#define DEBUGOUTPUT 0
#define trigPin 8  //超声波
#define echoPin 7  //超声波
#define touch 9  //触摸传感器
#define vibration 3  //震动马达模块 数字引脚
int Photo = A0;  //光敏电阻 模拟
SoftwareSerial BT_led(3, 6);  //新建对象，接收脚为3(对应蓝牙的T)，发送脚为6(对应蓝牙的R)
//wifi模块引脚未定

File myFile;

// checksum variables
byte generatedChecksum = 0;
byte checksum = 0; 
int payloadLength = 0;
byte payloadData[64] = {0};
byte poorQuality = 0;
byte attention = 0;
byte meditation = 0;

// system variables
long lastReceivedPacket = 0;
boolean bigPacket = false;

void send_ligh(int ligh);
void get_brain();
int get_distance();
int get_ligh();
byte ReadOneByte();
void Open_File(String Name);
void Close_File();
void Write_File(String line);

int get_distance(){
	long duration, distance;
	digitalWrite(trigPin, LOW);  // Added this line
	delayMicroseconds(2); // Added this line
	digitalWrite(trigPin, HIGH);
  
	delayMicroseconds(10); // Added this line
	digitalWrite(trigPin, LOW);
	duration = pulseIn(echoPin, HIGH);
	distance = (duration / 2) / 29.1;

 Serial.print("distance: ");
  Serial.println(distance);
	return(distance);
}

int get_ligh(){
	val = analogRead(Photo);            // reads the value of the potentiometer (value between 0 and 1023)
	val = map(val, 0, 1023, 0, 255);
}

void Open_File(String Name) {
	Serial.print("Initializing SD card...");

	if (!SD.begin(4)) {
		Serial.println("initialization failed!");
		return;
	}
	Serial.println("initialization done.");

	// open the file. note that only one file can be open at a time,
	// so you have to close this one before opening another.
	// re-open the file for reading:
	myFile = SD.open(Name, FILE_WRITE);
	if (myFile) {
		// read from the file until there's nothing else in it:
		while (myFile.available()) {
			Serial.println("File OK!");
			// close the file:
			//myFile.close();
		}
	}
}

void Close_File() {
	myFile.close();
}

void Write_File(String line){
	myFile.print(line);
 Serial.print("line: ");
 Serial.println(line);
}


byte ReadOneByte() {
  int ByteRead;

  while(!Serial.available());//{
  ByteRead = Serial.read();

#if DEBUGOUTPUT  
  //Serial.print((char)ByteRead);   // echo the same byte out the USB serial (for debug purposes)
#endif

  return ByteRead;
  //}
  //else{
  //  return 0;
  //  }
}


void send_ligh(int ligh){
  BT_led.write(ligh);
  }

void setup() {
  // put your setup code here, to run once:
	
	Serial.begin(BAUDRATE);           // USB
	BT_led.begin(9600);
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
	pinMode(vibration,OUTPUT);
	pinMode(Photo, INPUT);
	pinMode(touch, OUTPUT);

	Open_File("brain.txt");
}

void loop() {
	long distance = get_distance();
	
	//脑电部分
  for(int i=0;i<=0;i++){
    Serial.print("ReadOneByte(): ");
    byte readonebyte = ReadOneByte();
    Serial.println(readonebyte);
 if(readonebyte == 170) {
    if(ReadOneByte() == 170) {

      payloadLength = ReadOneByte();
      Serial.print("payloadLength:");
      Serial.println(payloadLength);
      if(payloadLength > 169)                      //Payload length can not be greater than 169
          //return;
          break;

      generatedChecksum = 0;        
      for(int i = 0; i < payloadLength; i++) {  
        payloadData[i] = ReadOneByte();            //Read payload into memory
        generatedChecksum += payloadData[i];
      }   

      checksum = ReadOneByte();                      //Read checksum byte from stream      
      generatedChecksum = 255 - generatedChecksum;   //Take one's compliment of generated checksum

        if(checksum == generatedChecksum) {    

        poorQuality = 200;
        attention = 0;
        meditation = 0;

        for(int i = 0; i < payloadLength; i++) {    // Parse the payload
          Serial.print("payloadData[");
          Serial.print(i);
          Serial.print("]: ");
          Serial.println(payloadData[i]);
          switch (payloadData[i]) {
          case 2:
            i++;        
                      Serial.print("payloadData[");
          Serial.print(i);
          Serial.print("]: ");
          Serial.println(payloadData[i]);    
            poorQuality = payloadData[i];
            bigPacket = true;            
            break;
          case 4:
            i++;
                      Serial.print("payloadData[");
          Serial.print(i);
          Serial.print("]: ");
          Serial.println(payloadData[i]);
            attention = payloadData[i];                        
            break;
          case 5:
            i++;
                      Serial.print("payloadData[");
          Serial.print(i);
          Serial.print("]: ");
          Serial.println(payloadData[i]);
            meditation = payloadData[i];
            break;
          case 0x80:
            i = i + 3;
            break;
          case 0x83:
            i = i + 25;      
            break;
          default:
            break;
          } // switch
        } // for loop

#if !DEBUGOUTPUT

        // *** Add your code here ***

        Serial.print("bigPacket: ");
        Serial.println(bigPacket);
        bigPacket = 1;
        if(bigPacket) {
          if(poorQuality == 0){
            //digitalWrite(LED, HIGH);
			Write_File(String(attention));
			Write_File("\n");
		  }
          //else
            //digitalWrite(LED, LOW);
          Serial.print("PoorQuality: ");
          Serial.print(poorQuality, DEC);
          Serial.print(" Attention: ");
          Serial.print(attention, DEC);
          Serial.print(" Time since last packet: ");
          Serial.print(millis() - lastReceivedPacket, DEC);
          lastReceivedPacket = millis();
          Serial.print("\n");
          }                     
        
#endif        
        bigPacket = false;        
      }
      else {
        // Checksum Error
      }  // end if else for checksum
    } // end if read 0xAA byte
  } // end if read 0xAA byte
  }
	
	touchstate = digitalRead(touch);
	if (touchstate == 1){   //按下才亮灯
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
    //Serial.println(readonebyte);
 if(readonebyte == 170) {
    if(ReadOneByte() == 170) {

      payloadLength = ReadOneByte();
      //Serial.print("payloadLength:");
      //Serial.println(payloadLength);
      if(payloadLength > 169)                      //Payload length can not be greater than 169
          //return;
          break;

      generatedChecksum = 0;        
      for(int i = 0; i < payloadLength; i++) {  
        payloadData[i] = ReadOneByte();            //Read payload into memory
        generatedChecksum += payloadData[i];
      }   

      checksum = ReadOneByte();                      //Read checksum byte from stream      
      generatedChecksum = 255 - generatedChecksum;   //Take one's compliment of generated checksum

        if(checksum == generatedChecksum) {    

        poorQuality = 200;
        attention = 0;
        meditation = 0;

        for(int i = 0; i < payloadLength; i++) {    // Parse the payload
          //Serial.print("payloadData[");
          //Serial.print(i);
          //Serial.print("]: ");
          //Serial.println(payloadData[i]);
          switch (payloadData[i]) {
          case 2:
            i++;        
                      //Serial.print("payloadData[");
          //Serial.print(i);
          //Serial.print("]: ");
          //Serial.println(payloadData[i]);    
            poorQuality = payloadData[i];
            bigPacket = true;            
            break;
          case 4:
            i++;
                      //Serial.print("payloadData[");
          //Serial.print(i);
          //Serial.print("]: ");
          //Serial.println(payloadData[i]);
            attention = payloadData[i];                        
            break;
          case 5:
            i++;
                      //Serial.print("payloadData[");
          //Serial.print(i);
          //Serial.print("]: ");
          //Serial.println(payloadData[i]);
            meditation = payloadData[i];
            break;
          case 0x80:
            i = i + 3;
            break;
          case 0x83:
            i = i + 25;      
            break;
          default:
            break;
          } // switch
        } // for loop

#if !DEBUGOUTPUT

        // *** Add your code here ***

        //Serial.print("bigPacket: ");
        //Serial.println(bigPacket);
        bigPacket = 1;
        if(bigPacket) {
          if(poorQuality == 0){
            //digitalWrite(LED, HIGH);
			Write_File(String(attention));
			Write_File("\n");
		  }
          //else
            //digitalWrite(LED, LOW);
          Serial.print("PoorQuality: ");
          Serial.print(poorQuality, DEC);
          Serial.print(" Attention: ");
          Serial.print(attention, DEC);
          Serial.print(" Time since last packet: ");
          Serial.print(millis() - lastReceivedPacket, DEC);
          lastReceivedPacket = millis();
          Serial.print("\n");
          }                     
        
#endif        
        bigPacket = false;        
      }
      else {
        // Checksum Error
      }  // end if else for checksum
    } // end if read 0xAA byte
  } // end if read 0xAA byte
  }
	
	touchstate = digitalRead(touch);
	if (touchstate == 1){   //按下才亮灯
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
