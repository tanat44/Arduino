/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>
#include <EEPROM.h>

// SERVO
Servo myservo;
int servoValue = 0;
int servoValueOffset = 0;
typedef enum Direction {
  up, upTen,
  down, downTen,
};
int servoSaveAddr = 0;

// NETWORK
const char* ssid = "Tanut";
const char* password = "NnNnNnNn";

// LED
const int ledPin = 2;
String ledState;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


void updateServo(Direction d) {
  if (d == upTen){
    servoValue += 10;
  } else if (d == up){
    servoValue += 1;
  } else if (d == downTen){
    servoValue -= 10;
  } else if (d == down){
    servoValue -= 1;
  }
  myservo.write(servoValue + servoValueOffset);
}

void loadServoValue() {
  int v;
  EEPROM.get(servoSaveAddr, v);
  Serial.print("Read value = ");
  Serial.println(v);
  if (v < -180 || v > 180) {
    servoValueOffset = 0;
  } else {
    servoValueOffset = v;  
  }  
  Serial.print("SERVO OFFSET = ");
  Serial.println(servoValueOffset);
}

void saveServoValue() {
  int v = servoValueOffset + servoValue;
  Serial.print("Value to save = ");
  Serial.println(v);
  EEPROM.put(servoSaveAddr, v);
  EEPROM.commit();
}

String processor(const String& var){
  if(var == "VALUE"){
    return String(servoValue);
  }
}

void setup(){
  Serial.begin(115200);
  EEPROM.begin(512);
  
  // Servo
  Serial.println("=== START SERVO ===");
  loadServoValue();
  myservo.attach(5); 
  myservo.write(servoValueOffset);

  // LED
  pinMode(ledPin, OUTPUT);

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/up", HTTP_GET, [](AsyncWebServerRequest *request){
    updateServo(up);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  server.on("/upTen", HTTP_GET, [](AsyncWebServerRequest *request){
    updateServo(upTen);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  server.on("/down", HTTP_GET, [](AsyncWebServerRequest *request){
    updateServo(down);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/downTen", HTTP_GET, [](AsyncWebServerRequest *request){
    updateServo(downTen);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/save", HTTP_GET, [](AsyncWebServerRequest *request){
    saveServoValue();    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Start server
  server.begin();
}
 
void loop(){
  
}



//  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send_P(200, "text/plain", getTemperature().c_str());
//  });
//  
//  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send_P(200, "text/plain", getHumidity().c_str());
//  });
//  
//  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send_P(200, "text/plain", getPressure().c_str());
//  });
