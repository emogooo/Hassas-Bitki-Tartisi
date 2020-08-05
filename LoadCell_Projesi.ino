#include <RealTimeClockDS1307.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#include "HX711.h"

HX711 sensor1;
HX711 sensor2;
HX711 sensor3;
HX711 sensor4;
HX711 sensor5;
HX711 sensorKopya;

RealTimeClockDS1307 zaman;

int sensorlerDT[5]={15,17,2,0,0};   //ÜSTTEKİNİ BUNA GÖRE AYARLA
int sensorlerSCK[5]={16,18,3,0,0};
float cf[5] = {380,439 ,441 ,441 ,441 };
int gecikme = 12;
float veri1 = 0, veri2 = 0, veri3 = 0, veri4 = 0, veri5 = 0;

float sensor_1(){
   
  sensor1.set_scale(cf[0]);
  veri1 = 0;
  for(int i = 0 ; i<gecikme ; i++){
   veri1 += sensor1.get_units();  // Veri al
  }
  veri1 = veri1 / gecikme;
  return veri1;  
}

float sensor_2(){
   
  sensor2.set_scale(cf[1]);
  veri2 = 0;
  for(int i = 0 ; i<gecikme ; i++){
   veri2 += sensor2.get_units();  // Veri al
  }
  veri2 = veri2 / gecikme;
  return veri2;  
}

float sensor_3(){
   
  sensor3.set_scale(cf[2]);
  veri3 = 0;
  for(int i = 0 ; i<gecikme ; i++){
   veri3 += sensor3.get_units();  // Veri al
  }
  veri3 = veri3 / gecikme;
  return veri3;  
}

float sensor_4(){
   
  sensor4.set_scale(cf[3]);
  veri4 = 0;
  for(int i = 0 ; i<gecikme ; i++){
   veri4 += sensor4.get_units();  // Veri al
  }
  veri4 = veri4 / gecikme;
  return veri4;  
}

float sensor_5(){
   
  sensor5.set_scale(cf[4]);
  veri5 = 0;
  for(int i = 0 ; i<gecikme ; i++){
   veri5 += sensor5.get_units();  // Veri al
  }
  veri5 = veri5 / gecikme;
  return veri5;  
}

void yazSD(float veri1, float veri2, float veri3, float veri4, float veri5){
 
 String gun,ay,dosyaAdi;
 zaman.readClock();
 if(zaman.getDate() < 10){
  gun = "0" + String(zaman.getDate());
 }else{
  gun = String(zaman.getDate());
 }
 
 if(zaman.getMonth() < 10){
  ay = "0" + String(zaman.getMonth());
 }else{
  ay = String(zaman.getMonth());
 }

 dosyaAdi = gun + ay + String(zaman.getYear()) + ".txt";
 Serial.println(dosyaAdi);
 File dosya = SD.open(dosyaAdi, FILE_WRITE);
 dosya.print(String(zaman.getHours()));
 dosya.print(":");
 dosya.print(String(zaman.getMinutes()));
 dosya.print(":");
 dosya.print(String(zaman.getSeconds()));
 dosya.print(" ----- ");
 dosya.print("Ağırlık 1 = ");
 dosya.print(String(veri1));
 dosya.print("   "); 
 dosya.print("Ağırlık 2 = ");
 dosya.print(String(veri2));
 dosya.print("   ");
 dosya.print("Ağırlık 3 = ");
 dosya.print(String(veri3));
 dosya.print("   ");
 dosya.print("Ağırlık 4 = ");
 dosya.print(String(veri4));
 dosya.print("   ");
 dosya.print("Ağırlık 5 = ");
 dosya.print(String(veri5));
 dosya.println(""); 
 dosya.close();
}

void setup() {
  Serial.begin(9600);
  SD.begin(10);
  sensor1.begin(sensorlerDT[0], sensorlerSCK[0]);
  sensor1.tare();
  sensor1.tare();
  
  sensor2.begin(sensorlerDT[1], sensorlerSCK[1]);
  sensor2.tare();
  sensor2.tare();
  
  sensor3.begin(sensorlerDT[2], sensorlerSCK[2]);
  sensor3.tare();
  sensor3.tare();
  
  sensor4.begin(sensorlerDT[3], sensorlerSCK[3]);
  sensor4.tare();
  sensor4.tare();
  
  sensor5.begin(sensorlerDT[4], sensorlerSCK[4]);
  sensor5.tare();
  sensor5.tare();
}
void loop() {
  
  veri1=sensor_1();
  veri2=sensor_2(); 
  veri3=sensor_3();
  veri4=sensor_4();
  veri5=sensor_5();
  yazSD(veri1, veri2, veri3, veri4, veri5);
  
  char tut = Serial.read();
  if(tut =='*'){
    
    sensor1.tare();
    sensor2.tare();
    sensor3.tare();
    sensor4.tare();
    sensor5.tare();
    
  }else if(tut == '1' || tut == '2' || tut == '3' || tut == '4' || tut == '5'){
    
    kalibrasyonFaktoruAyarla(tut); // 6.8 grama göre ayarlar(sensöre 1 lira koy)
    
  }else if(tut == '+'){
    
    gecikme += 12;
    
  }else if(tut == '-'){
    
    if(gecikme != 12){
      
      gecikme -= 12;
      
    }else{
      
      Serial.println("Hesap süresi 1 saniyeden az olamaz");
      
    }
  }
}

void kalibrasyonFaktoruAyarla(char tut){
  int sensorSecici = (int)tut - 49;
  float x = 0;
  if(sensorSecici == 0){
    
    sensorKopya=sensor1;
    
  }else if(sensorSecici == 1){
    
    sensorKopya=sensor2;
    
  }else if(sensorSecici == 2){
    
    sensorKopya=sensor3;
    
  }else if(sensorSecici == 3){
    
    sensorKopya=sensor4;
    
  }else if(sensorSecici == 4){
    
    sensorKopya=sensor5;
    
  }
  Serial.println("İşleme başlanıyor");
  
   for(float i = (cf[sensorSecici]-10); i < (cf[sensorSecici]+10); i+=0.1){
    sensorKopya.set_scale(i);
    Serial.println(i);
    if(sensorKopya.get_units() > 8.34 && sensorKopya.get_units() < 8.36){
      x=i;
      break;
    }
  }
  Serial.print("Bulunan KALIBRASYON DEGERI = ");
  Serial.print(x,1);
  cf[sensorSecici] = x;
}
  
