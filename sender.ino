
#include <ESP8266WiFi.h>
#include <espnow.h>

// REPLACE WITH RECEIVER MAC Address
// 4C:75:25:37:1F:2C
uint8_t broadcastAddress[] = {0x4C, 0x75, 0x25, 0x37, 0x1F, 0x2C};

// Structure example to send data
// Must match the receiver structure
typedef struct analog_messagedata {

  uint16_t potentiometer = 0;

  
} analog_messagedata;

// Create a struct_message called myanalogData
analog_messagedata myanalogData;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print(". Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println(". Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
    delay(20);

    
    myanalogData.potentiometer = analogRead(A0);


    // Send message via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &myanalogData, sizeof(myanalogData));

}