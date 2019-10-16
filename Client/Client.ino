/*
 * This is the wifi-STA code for esp to esp connection
 */
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "4WWB0-234";
const char *pass = "jankinator9000";

IPAddress ip(192, 168, 11, 5);
IPAddress gateway(192, 168, 11, 1);
IPAddress subnet(255, 255, 255, 0);

const char *host = "192.168.11.4";
const int port = 80;

// vibration parameters with default values
int iter;
int delay_a;
int delay_b;
int delay_loop;

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.print("Set ESP STA mode ... ");
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, pass);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  // get parameters from server
  HTTPClient http;
  String url;
  
  Serial.println("Retrieving parameters from server ...");
  
  url = "http://";
  url += host;
  url += "/getDelayA";
  http.begin(url);
  if (http.GET()) {
    delay_a = http.getString().toInt();
  } else {
    Serial.println("Failed to get delay_a ...");
    delay_a = 1000; // fallback value
  }
  Serial.print("delay_a: ");
  Serial.println(delay_a);
  http.end();

  url = "http://";
  url += host;
  url += "/getDelayB";
  http.begin(url);
  if (http.GET()) {
    delay_b = http.getString().toInt();
  } else {
    Serial.println("Failed to get delay_b ...");
    delay_b = 0; // fallback value
  }
  Serial.print("delay_b: ");
  Serial.println(delay_b);
  http.end();

  url = "http://";
  url += host;
  url += "/getIter";
  http.begin(url);
  if (http.GET()) {
    iter = http.getString().toInt();
  } else {
    Serial.println("Failed to get iter ...");
    iter = 1; // fallback value
  }
  Serial.print("iter: ");
  Serial.println(iter);
  http.end();

  url = "http://";
  url += host;
  url += "/getLoopDelay";
  http.begin(url);
  if (http.GET()) {
    delay_loop = http.getString().toInt();
  } else {
    Serial.println("Failed to get delay_loop ...");
    delay_loop = 150; // fallback value
  }
  Serial.print("delay_loop: ");
  Serial.println(delay_loop);
  http.end();
}

void loop() {
  HTTPClient http;
  String url = "http://";
  url += host;
  url += "/access";
  
  http.begin(url);  // Specify request destination
  String value = "0";
  if (http.GET()) {
    value = http.getString();
    Serial.println(value);
  } else {
    Serial.println("Failed to retrieve flag ...");
  } 
  http.end();
  if(value[1] == '1') {
    ESP.restart(); // serverside restart flag was raised
  }
  if(value[0] == '1') {
    Serial.println("start vibration mode ...");
    vibrate();
  }
  
  delay(delay_loop);
}

/**
 * send signal to vibration motors
 */
void vibrate() {
  pinMode(3, OUTPUT);
  for (int i = 1; i <= iter; i++) {
    analogWrite(3,1023);
    Serial.println("vibrating!");
    delay(delay_a);
    analogWrite(3,0);
    Serial.println("stop!");
    delay(delay_b);
  }
}
