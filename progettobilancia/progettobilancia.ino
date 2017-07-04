#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <EEPROM.h>
#include "config.h"
#include <sapiArduino.h>
#include <CSVwriter.h>

 
//jsessionid & validationkey
Session keys;

//info file
FileInfo file;

//variables declarations
String date;
String weight;
int h = 1;
int conta = 0;
int statusCode = -1;
String Id = "";
String a;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
  }
  EEPROM.begin(512);
}

void loop() {
  if(((((millis()/h)/1000)/60)==14)||(h==1)){
    statusCode = doLogin(username, password, &keys);
    Serial.print("statusCode: ");
    Serial.println(statusCode);
    h++;
  }
  int err = 1000;
  int d = 5000;
  if(conta == 0){
    Id = readId();
    if(Id != ""){
        file.content = fileContent(fileGet(dowloadWithId(Id, &file, keys)));
        Serial.println("ready");
    }
    conta = 1;
  }
  if(digitalRead(LED_BUILTIN)){
    buildCsvInfo(&file);
    if(statusCode == 0){
      while(err == 1000){
          if( Id == ""){
            Id = sendMetadata(keys, file, Id);
            storageId(Id);
          }else{
            a = "\"id\":\""+ Id + "\",";
            sendMetadata(keys, file, a);
          }
          err = saveFile(keys, file, Id);
          if(err == 1000){
            delay(d);
            d += 2000;
          }
      }
    }
  }
  delay(50);
}


