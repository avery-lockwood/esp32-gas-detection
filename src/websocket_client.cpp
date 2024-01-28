#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
//web vars
const char* ssid = "Brickhouse";
const char* password = "Nopass4us!";
const char*  IP = "192.168.86.249";
const int serverPort = 8080; // WebSocket server port
//web vars

//vars for data collection
const int sample_size = 100;
const int mq7_pin = 32;
const int mq135_pin = 33;
const int mq5_pin = 34;  

const int mq7_light_pin = 14;
const int mq135_light_pin = 27;
const int mq5_light_pin = 26;

const int mq7_thresh = 3500;
const int mq135_thresh = 125;
const int mq5_thresh = 1750;
//vars for data collection

WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    Serial.printf("Received text: %s\n", payload);
    // Process the received data here
  }
}


float GetSensorData(int pin){
  float sensorValue = 0;
  for (int i = 0; i < sample_size; i++)
  {
    sensorValue += analogRead(pin);
  }
  sensorValue = sensorValue / sample_size;
  return sensorValue;
}




void setup() {
  //setting up pins
  pinMode(mq7_pin, INPUT);
  pinMode(mq135_pin, INPUT);
  pinMode(mq5_pin, INPUT);
  pinMode(mq7_light_pin, OUTPUT);
  pinMode(mq135_light_pin, OUTPUT);
  pinMode(mq5_light_pin, OUTPUT);

  //setting up pins
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  IPAddress localIP = WiFi.localIP();
  Serial.println(localIP);

  webSocket.begin(IP, serverPort, "/websocket");
 


  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000); // Reconnect every 5 seconds


  // No need for a separate connect() call, the connection is established automatically
}

void loop() {
  delay(300);
  webSocket.loop();
  float mq7_value = GetSensorData(mq7_pin);
  float mq135_value = GetSensorData(mq135_pin);
  float mq5_value = GetSensorData(mq5_pin);

  if (mq7_value > mq7_thresh){ digitalWrite(mq7_light_pin, HIGH);} else{digitalWrite(mq7_light_pin, LOW);}
  if (mq135_value > mq135_thresh){ digitalWrite(mq135_light_pin, HIGH);} else{digitalWrite(mq135_light_pin, LOW);}
  if (mq5_value > mq5_thresh){ digitalWrite(mq5_light_pin, HIGH);} else{digitalWrite(mq5_light_pin, LOW);}
 
 
  Serial.print("mq7:");
  Serial.println(mq7_value);
  Serial.print("mq135:");
  Serial.println(mq135_value);
  Serial.print("mq5:");
  Serial.println(mq5_value);
  

  String message = String(mq7_value);
  if(!webSocket.sendTXT(message)){
    Serial.println("Message did not send");
  }
  delay(100);
}