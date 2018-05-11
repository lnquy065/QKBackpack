#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include<DHT.h>  
#include <Adafruit_Sensor.h>    
#include "MQ135.h"
#include <MFRC522.h>
#include <SPI.h>

/*
* Chương trình đọc RFID từ RFID-RC522
* Cắm chân (Arduino Uno):
* MOSI: Pin 11 / ICSP-4
* MISO: Pin 12 / ICSP-1
* SCK: Pin 13 / ISCP-3
* SDA: Pin 10
* RST: Pin 9
*/

#define SS_PIN 10
#define RST_PIN 9
#define MY_CARD "214 224 214 43"
#define PIN_MQ135 A2

bool isRightCard();
void printDec(byte *buffer, byte bufferSize);
bool boolValueOf(String);
void alert(bool);

//int code[] = {212,138,223,43}; //This is the stored UID
//int codeRead = 0; 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
String uidString;

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif


//Security
bool vib = false;
bool rfi = false;
bool thi = false;
bool fid = false;
bool canOff = false;

//PIN setup
int pin_Vib = 2;
int pin_Vin = A1;
int pin_UV = A0;
int pin_Alert = 8;
int pin_Switch = 6;

int averageAnalogRead(int aPin, int nSample) {
  int ret = 0;
  for (int i=0; i < nSample; i++) ret+= analogRead(aPin);
  return ret/nSample;
}

int getUV(int sPin, int rPin) {
  //sPin đọc vào sẽ tham chiếu dưới mức 0-5v ( 5/1024 ) nhưng chúng ta cần lấy ở tham chiếu mức 0-3.3v
  //Nên ta tính thêm vol ở mỗi mức trong hệ 3.3v ( 3.3/699), sau đó nhân với sPin.
  //VD: sPin = 213, rPin = 699 => vOut = (3.3 / 699) * 213 = ~ 1v
  float uv_VolOutput = 3.3 / averageAnalogRead(rPin,8) * averageAnalogRead(sPin,8);
  int uvMax = 15, uvMin = 0;
  //Cường độ UV tương ứng với các mức điện áp từ 0.99 -> 2.8. Ta map giá trị 0.99-2.8 sang 0-15
  int uv_Intensity = (uv_VolOutput - 0.99) * (2.8 - 0.99) / (uvMax - uvMin) + uvMin;
  return uv_Intensity;
}

//Rung
const int DHTPIN = 3;       //Đọc dữ liệu từ DHT11 ở chân 2 trên mạch Arduino
const int DHTTYPE = DHT11;  //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
DHT dht(DHTPIN, DHTTYPE);

// int NoiseSensor = A0;9

SoftwareSerial bluetooth(4,5);
unsigned int screenRefeshTime = 1;
unsigned long lastRefesh = - screenRefeshTime*1000;

MQ135 mq135_sensor = MQ135(PIN_MQ135);

void setup()
{
  
  //set up bluetooth
  Serial.begin(9600);
  while(!Serial);
  bluetooth.begin(38400);
  while (!bluetooth);

  dht.begin();         // Khởi động cảm biến

  //set up RFID
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  pinMode(pin_Alert,OUTPUT);
  pinMode(pin_Vin, INPUT);
  pinMode(pin_UV, INPUT);
  pinMode(pin_Switch, INPUT);


  //Cân bằng MQ135, cần 60s để làm nóng MQ135
  mq135_sensor.calibrate();


}



void loop()
{
   if(millis() - lastRefesh > screenRefeshTime*1000 ){
    bluetooth.print(String(dht.readTemperature())+"|"+dht.readHumidity()+"|"+mq135_sensor.readCO2()+"|"+getUV(pin_UV, pin_Vin)+"\n");
  //  Serial.print(String(dht.readTemperature())+"|"+dht.readHumidity()+"|"+mq135_sensor.readCO2()+"|"+getUV(pin_UV, pin_Vin)+"\n");
   
  //  Serial.println("sent");
    lastRefesh = millis();
  }

  if (fid == true) alert(true);
  else if (vib==false || canOff==true)  alert(false);


  //Kiem tra bảo mật
  if( rfid.PICC_IsNewCardPresent() && rfi == true)
  {

      if (isRightCard() )  {
        Serial.println("Dung card");
        alert(false);
        fid = false;
      }
      
  }

  if (digitalRead(pin_Vib)==1 && vib == true) {
    alert(true);
  }





  //Bật tắt chế độ khóa
  if (bluetooth.available()) {
    
    String in = bluetooth.readStringUntil('\n');
    in.trim();
    Serial.println(in);
    Serial.println(in.substring(3));
    bool val = boolValueOf(in.substring(3));
    String key = in.substring(0, 3);
    Serial.println(key);
     Serial.println(val);
    if (key == "vib" ) {
      vib = val;
    } else if (key=="rfi") {
      rfi = val;
    } else if (key=="fin") {
      
      fid = !fid;
      if (fid==false && vib==true) canOff=true;
      else canOff =false;
    }
  }

}


void alert(bool b) {
  if (b==true) digitalWrite(pin_Alert, 1);
  else digitalWrite(pin_Alert, 0);
}


bool boolValueOf(String b) {
  if (b == "true") return true;
  else return false;
}

bool isRightCard()
{
  
  rfid.PICC_ReadCardSerial();
  Serial.print(F("\nPICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

    Serial.println("Scanned PICC's UID:");

    printDec(rfid.uid.uidByte, rfid.uid.size);

    uidString = String(rfid.uid.uidByte[0])+" "+String(rfid.uid.uidByte[1])+" "+String(rfid.uid.uidByte[2])+ " "+String(rfid.uid.uidByte[3]);
    bool ret = false;
    Serial.print(uidString);
    if(uidString == MY_CARD){
      ret =  true;
    }
    
    // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
  return ret;
}

void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}




