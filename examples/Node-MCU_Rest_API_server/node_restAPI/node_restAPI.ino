
#include <stdio.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

#define HTTP_REST_PORT 80
#define WIFI_RETRY_DELAY 500
#define MAX_WIFI_INIT_RETRY 50

const char* wifi_ssid = "wifi_ssid_name";
const char* wifi_passwd = "wifi_password";

ESP8266WebServer http_rest_server(HTTP_REST_PORT);

/**
    Function To Initialize WiFi
*/
int init_wifi() {
  int retries = 0;
  Serial.println("Connecting to WiFi ...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_passwd);
  // check the status of WiFi connection to be WL_CONNECTED
  while ((WiFi.status() != WL_CONNECTED) && (retries < MAX_WIFI_INIT_RETRY)) {
    retries++;
    delay(WIFI_RETRY_DELAY);
    Serial.print(".");
    // Dots will print to show still connecting...
  }
  return WiFi.status(); // return the WiFi connection status
}

/**
    Function Of Rest API - /tempdata
    This function will execute when a HTTP GET is called
*/
void get_temp_data() {
  Serial.println("HTTP GET called");

  // Create JSON object to send data
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& jsonObj = jsonBuffer.createObject();
  char JSONmessageBuffer[200];
  // Add Data To Json Object to send data
  jsonObj["StringData"] = "your_string_data";
  jsonObj["IntData"] = 100;
  jsonObj.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  http_rest_server.send(200, "application/json", JSONmessageBuffer);
}



/**
    config REST API function
*/
void config_rest_server_routing() {
  http_rest_server.on("/", HTTP_GET, []() {
    http_rest_server.send(200, "text/html",
                          "Welcome to the ESP8266 REST Web Server");
  });

  // Add your REST APIs & the functions to be run when call those APIs
  // http://192.168.1.7/tempdata
  http_rest_server.on("/tempdata", HTTP_GET, get_temp_data);
}

void setup(void) {
  Serial.begin(115200);
  Serial.println("MCU Program Started...");
  if (init_wifi() == WL_CONNECTED) {
    Serial.println("WIFI Connetted");
    Serial.print(wifi_ssid);
    Serial.print("--- IP: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.print("Error connecting to: ");
    Serial.println(wifi_ssid);
  }

  config_rest_server_routing();

  http_rest_server.begin();
  Serial.println("HTTP REST Server Started");
}

void loop(void) {
  http_rest_server.handleClient();
}
