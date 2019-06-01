#include <SPI.h>
#include <BridgeClient.h>
#include <PubSubClient.h>

void callback(char* topic, byte* payload, unsigned int length);
//Creating Sensor Pins
const int pinTemp = A0, trigPin = 13, echoPin = 12;
//Global Variables
char message_buff[100];
//Prepare functions
void callback(char* topic, byte* payload, unsigned int length);
String buildClientName();
String buildDistJson(double distance);
String buildJson(double distance);
float getDistance();
String getPlate();

//IBM Watson IoT Platform Details
char orgName[] = "gcqkx2";
char macstr[] = "arduinoyun";
char server[] = "gcqkx2.messaging.internetofthings.ibmcloud.com";
char type[] = "arduino";
char token[] = "oF(z!2)Cc&TwZN6UPq";
int port = 1883;
String clientName = buildClientName();
//Default topic name of Watson IoT Platform
String topicName = String("iot-2/cmd/+/fmt/json");

String ID = String("1");
// Plate information
String current_car_plate = String("34 ABC 123");
int state = 0; // 0: no car, 1: full
String action_type;

// Limits
int car_near_limit = 5;
int car_away_limit = 15;

//Objects for Arduino works
BridgeClient yunClient;
PubSubClient client(server, port, callback, yunClient);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Bridge.begin();
  pinMode(pinTemp, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.println("SETUP");
  delay(3000);

}


//Running code
void loop() {
  delay(1500);
  char clientStr[34];
  clientName.toCharArray(clientStr, 34);
  char topicStr[26];
  topicName.toCharArray(topicStr, 26);
  if (!client.connected()) {
    //digitalWrite(pinRed, HIGH); //Device could not connect
    //digitalWrite(pinGreen, LOW);
    Serial.print("Trying to connect to: ");
    Serial.println(clientStr);
    client.connect(clientStr, "use-token-auth", token);
    if (client.connect(clientStr, "use-token-auth", token)) {
      client.setCallback(callback);
      Serial.println("Connection successful ...");
      if (client.subscribe("iot-2/cmd/cid/fmt/json", 0)) {
        Serial.println("Subscribed to iot-2/cmd/cid/fmt/json");
      } else {
        Serial.println("NOT Subscribed to iot-2/cmd/cid/fmt/json");
      }
    }
  }
  if (client.connected()) {
    //digitalWrite(pinRed, LOW);
    //digitalWrite(pinGreen, HIGH); //Successful connection
    //Serial.println("Online...Begin transmit...");
    // Build the JSON data to publish.
    double distance = getDistance();
    Serial.println(distance);

    bool send_data = false;

    if (distance < car_near_limit && state == 0){
        action_type = String("car_entered");
        state = 1;
        send_data = true;
    } else if(distance > car_away_limit && state == 1){
        action_type = String("car_left");
        state = 0;
        send_data = true;
    }

    if(send_data){
      String json = buildJson(distance);
      char jsonStr[200];
      json.toCharArray(jsonStr, 200);
      Serial.println(jsonStr);
  
      // Publish the data.
      client.publish("iot-2/evt/status/fmt/json", jsonStr);
    }else {
      String json = buildDistJson(distance);
      char jsonStr[200];
      json.toCharArray(jsonStr, 200);
      Serial.println(jsonStr);
  
      // Publish the data.
      client.publish("iot-2/evt/status/fmt/json", jsonStr);
    }
  }
}

// Builds the clientName
String buildClientName () {
  String data = "";
  data += "d:";
  data += orgName;
  data += ":";
  data += type;
  data += ":";
  data += macstr;
  return data;
}

// Builds the JSON
String buildDistJson(double distance) {
  char buffer[100];
  String data = "{\"Distance\":";
  dtostrf(distance, 1, 2, buffer);
  data += buffer;
  data += "}";
  return data;
}

// Builds the JSON
String buildJson(double distance) {
  char buffer[100];
  String data = "{\"Distance\":";
  dtostrf(distance, 1, 2, buffer);
  data += buffer;
  data += ",\"Plate\":";
  data += "\"" + current_car_plate + "\"";
  data += ",\"Type\":";
  data += "\"" + action_type + "\"";
  data += ",\"ID\":";
  data += "\"" + ID + "\"";
  data += "}";
  return data;
}

// Getting Temperature and Humidity Values
void callback(char* topic, byte* payload, unsigned int length) {}
float getDistance() {
  long duration;
  float distance;
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  return distance;
}