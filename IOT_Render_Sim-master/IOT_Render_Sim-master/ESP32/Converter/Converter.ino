#include "ModelLoader.hpp"
#include "SD.h"
#include "SPI.h"
#include "Protocol.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>  // Include HTTPClient library

JsonObject j_son;
Model* ImpModel;
DynamicJsonDocument jsonDoc(2048);
WiFiClient client;

const char* ssid     = "RedboxVR";
const char* password = "r3db0xVR";

const String host = "192.168.1.203:3000/api/";

#define SD_CS_PIN  5
#define SD_MOSI_PIN  23
#define SD_MISO_PIN  19
#define SD_SCK_PIN  18

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");


void setup() {
  Serial.begin(115200);
  SD.begin(); 

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
  }

  sendGetRequest("http://192.168.1.203:3000/status");

  ImpModel = new Model("/sd/raven");
  j_son = jsonDoc.to<JsonObject>();
  
  SerializeModel(*ImpModel, j_son);
  serializeJson(j_son, Serial);
  sendPostRequest("http://192.168.1.203:3000/api/image", j_son);
  j_son.clear();

  Serial.print("\n");
}

void sendPostRequest(String url, JsonObject& jsonDoc) {
  // Create a buffer to store the serialized JSON
  const size_t capacity = JSON_OBJECT_SIZE(10); // Adjust the size as per your JSON structure
  char buffer[capacity];

  // Serialize the JSON document to the buffer
  size_t len = serializeJson(jsonDoc, buffer, sizeof(buffer));

  HTTPClient http;
  http.begin(url);  // Specify the URL
  http.addHeader("Content-Type", "application/json");  // Specify content-type header

  // Send the POST request with the serialized JSON buffer
  int httpResponseCode = http.sendRequest("POST", len);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println(httpResponseCode);  // Print return code
    Serial.println(response);  // Print request response payload
  } else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }

  http.end();  // Free resources
}

void sendGetRequest(String url) {
  if(WiFi.status() == WL_CONNECTED) {  // Check WiFi connection status
    HTTPClient http;
    http.begin(url);  // Specify the URL

    int httpResponseCode = http.GET();  // Make the GET request

    if (httpResponseCode > 0) {
      String response = http.getString();  // Get the response payload
      Serial.println(httpResponseCode);  // Print return code
      Serial.println(response);  // Print response payload
    } else {
      Serial.print("Error on sending GET: ");
      Serial.println(httpResponseCode);
    }

    http.end();  // Free resources
  } else {
    Serial.println("Error in WiFi connection");
  }
}

void PostImage(Model & Owner){
 SerializeImage(Owner.Texture, j_son);
 String payload;
 serializeJson(j_son, payload);
 sendPostRequest(host + "image", payload);  // Replace with your URL
 Img_Sent = true;
 j_son.clear();
}

void PostSound(Model & Owner){
 SerializeSound(Owner.SFX, j_son);
 String payload;
 serializeJson(j_son, payload);
 sendPostRequest(host + "sound", payload);  // Replace with your URL
 Sfx_Sent = true;
 j_son.clear();
}

void PostMesh(Model & Owner){
 SerializeMesh(Owner.Meshes[MeshIndex], j_son);
 String payload;
 serializeJson(j_son, payload);
 sendPostRequest(host + "mesh", payload);  // Replace with your URL
 MeshIndex++;
 j_son.clear();
}

void PostBone(Model & Owner){
 SerializeBone(Owner.Bones[BoneIndex], j_son);
 String payload;
 serializeJson(j_son, payload);
 sendPostRequest(host + "bone", payload);  // Replace with your URL
 BoneIndex++;
 j_son.clear();
}

void PostTrack(Model & Owner){
 SerializeAnimation(Owner.Tracks[TrackIndex], j_son);
 String payload;
 serializeJson(j_son, payload);
 sendPostRequest(host + "track", payload);  // Replace with your URL
 TrackIndex++;
 j_son.clear();
}

void loop() {
  if(!Ready){
    if(Serial.available()){

       delay(100);
      
      String Response = Serial.readStringUntil('\n');

      if(Response.equals("NEXT")){

        if(!Img_Sent){
          PostImage(*ImpModel);
        } else if(!Sfx_Sent){
          PostSound(*ImpModel);
        } else if(MeshIndex < ImpModel->Meshes.size()){
          PostMesh(*ImpModel);
        } else if (BoneIndex < ImpModel->Bones.size()){
          PostBone(*ImpModel);
        } else if (TrackIndex < ImpModel->Tracks.size()){
          PostTrack(*ImpModel);
        } else {
          Ready = true;
        }
       
        Serial.write('\n');
      }
    }
  }  
}
