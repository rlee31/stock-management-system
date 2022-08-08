
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>

#define trig D4
#define echo D5
#define trig__ D6
#define echo__ D7
#define led D0
#define led__ D8
#define BLYNK_PRINT Serial

float dispenser_height = 50;
float full_height = 40; 

LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = "mBuYVAx65xsdGbok3w6rRmR0PWVVFQVF";
char ssid[] = "KT_GiGA_Logn";//Enter your WIFI name
char pass[] = "Codingisfun1";//Enter your WIFI password

long distancepast = 0;
long distancenow = 0;

long distancepast__ = 0;
long distancenow__ = 0;

BlynkTimer timer;

int flag1 = 0;
int flag2 = 0;

// variable indicating percentage
float stockleft = 0;
float stockright = 0;

String text = "Stock Management System by LSY"; 

void setup() {
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(trig__, OUTPUT);
  pinMode(echo__, INPUT);
  pinMode(led, OUTPUT);
  pinMode(led__, OUTPUT);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  delay(4000);

//Call the functions
  timer.setInterval(100L, ultrasonic_1);
  timer.setInterval(100L, ultrasonic_2);
//  timer.setInterval(100L, BLYNK_WRITE);

  lcd.init();
  // I2C LCD의 백라이트를 켜줍니다.
}

BLYNK_WRITE(V8)
{
  text = param.asString();
}

//Get the ultrasonic sensor values
void ultrasonic_1() {
  if(flag2 == 1)
  {
    return;
  }
  digitalWrite(trig, LOW);
  delayMicroseconds(4);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  long t = pulseIn(echo, HIGH);
  long cm = t / 29 / 2;

  Serial.println(dispenser_height - cm);
  stockleft = int((dispenser_height - cm)/full_height * 100);
  Serial.println(int(stockleft));
  
  distancenow = cm;

  
  if(distancenow - distancepast < 10 && distancenow - distancepast > -10)
  {
    if (cm >= 30) {
    Blynk.notify("Warning! Dispenser Out of Stock");
    Serial.println("Warning! Dispenser Out of Stock");
    digitalWrite(led, HIGH);
    
    } else {
      digitalWrite(led, LOW);
    }
    
    if(distancenow - distancepast > 1)
    {
      lcd.clear();
      lcd.backlight();
      lcd.setCursor(0,0);           // 0번째 줄 0번째 셀부터 입력하게 합니다.
      lcd.print("Have a good day!");       // 문구를 출력합니다.
      lcd.setCursor(0,1);           // 0번째 줄 0번째 셀부터 입력하게 합니다.
      String stocktext = "Pad:"+ String(int(stockleft)) + "% " + "Tam:" + String(int(stockright)) + "%";
      lcd.print(stocktext);
      
      flag1 = 1;
      delay(2000);
      flag1 = 0;
    } else {
      lcd.noBacklight();
      lcd.clear();
      lcd.print(text.substring(0, 16));
      lcd.setCursor(0,1);
      lcd.print(text.substring(16));
         
    }
    
    Blynk.virtualWrite(V0, stockleft);
    delay(10);
    
  } else {
    Serial.println("Invalid distance");
  }
  distancepast = distancenow;
  delay(2000);
}

void ultrasonic_2() {
  if(flag1 == 1)
  {
    return;
  }
  digitalWrite(trig__, LOW);
  delayMicroseconds(4);
  digitalWrite(trig__, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig__, LOW);
  
  long t__ = pulseIn(echo__, HIGH);
  long cm__ = t__ / 29 / 2;

  stockright = int((dispenser_height - cm__)/full_height * 100);
  
  distancenow__ = cm__;

  if(distancenow__ - distancepast__ < 10 && distancenow__ - distancepast__ > -10)
  {
    if (cm__ >= 30) {
    Blynk.notify("Warning! Dispenser Out of Stock");
    Serial.println("Warning! Dispenser Out of Stock");
    
    digitalWrite(led__, HIGH); 
    
    } else {
      digitalWrite(led__, LOW);
    }
    
    if(distancenow__ - distancepast__ > 1)
    {
      lcd.clear();
      lcd.backlight();
      lcd.setCursor(0,0);           // 0번째 줄 0번째 셀부터 입력하게 합니다.
      lcd.print("Have a good day!");       // 문구를 출력합니다.
      lcd.setCursor(0,1);           // 0번째 줄 0번째 셀부터 입력하게 합니다.
      String stocktext = "Pad:"+ String(int(stockleft)) + "% " + "Tam:" + String(int(stockright)) + "%";
      lcd.print(stocktext);
      
      flag2 = 1;
      delay(2000);
      flag2 = 0;
    } else {
      lcd.clear();
      lcd.noBacklight();
      lcd.setCursor(0,0);           // 0번째 줄 0번째 셀부터 입력하게 합니다.
      lcd.print(text.substring(0, 16));
      lcd.setCursor(0,1);
      lcd.print(text.substring(16));
//      String stocktext = "Pad: "+ int(stockleft) + "%  " + "Tam: " + int(stockright) + "%";
//      lcd.print(stocktext);
    
    Blynk.virtualWrite(V1, stockright);
    Serial.print("sonic 2 :");
    Serial.println(cm__);
  
    delay(10);
  } 
  } else {
    Serial.println("Invalid distance");
  }
  
  distancepast__ = distancenow__;
}

void loop() { 

  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
     // 문구를 출력합니다.
 
}
