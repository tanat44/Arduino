

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
SoftwareSerial UnoSerial(D3, D2); // RX | TX


#ifndef STASSID
#define STASSID "j"
#define STAPSK  "JjJjJjJj"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);
DynamicJsonDocument doc(100);
const int led = LED_BUILTIN;

void handleRoot() {
  if (server.method() != HTTP_POST) {
    digitalWrite(led, 1);
    server.send(405, "text/plain", "Method Not Allowed");
    digitalWrite(led, 0);
  } else {
    String body = server.arg("plain");
    deserializeJson(doc, body);
    const char * command = doc["command"];
    UnoSerial.print(command);
    Serial.print(command);
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "{ok: true}"); 
  }
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  UnoSerial.begin(38400);

  server.on("/", handleRoot);
  
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
