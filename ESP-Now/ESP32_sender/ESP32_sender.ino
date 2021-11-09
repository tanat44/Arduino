#include <esp_now.h>
#include <WiFi.h>

// Receiver address
uint8_t broadcastAddress[] = {0x50, 0x02, 0x91, 0xDE, 0xA4, 0x0F};

typedef struct struct_message {
  char a[32];
  int accel;
  int steer;
} struct_message;

// Create a struct_message called myData
struct_message myData;

const int ACCEL_PIN = 39;
const int STEER_PIN = 36;

int accel = 0;
int steer = 0;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void readAnalog(){
  accel = analogRead(ACCEL_PIN);
  steer = analogRead(STEER_PIN);
  Serial.print("ACC ");
  Serial.print(accel);
  Serial.print(" STEER ");
  Serial.println(steer);
}
 
void loop() {
  readAnalog();
  strcpy(myData.a, "THIS IS A CHAR");
  myData.accel = accel;
  myData.steer = steer;
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(2000);
}
