/*
 * This is the wifi-STA code for esp to esp connection
 */
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

/* wifi config */
const char *ssid = "4WWB0-234";
const char *pass = "passsword";

/* network config */
//IPAddress ip(192, 168, 4, 2);
//IPAddress gateway(192, 168, 4, 1);
//IPAddress subnet(255, 255, 255, 0);

/* client config */
const char *host = "192.168.11.4";
const int port = 80;

/* vibration code */
void vibrate()
{
  pinMode(0, OUTPUT);
  analogWrite(0,1023);
  delay(250);
  analogWrite(0,0);
  delay(250);
  analogWrite(0,1023);
  delay(250);
  analogWrite(0,0);
  delay(250);
  analogWrite(0,1023);
  delay(500);
  analogWrite(0,0);
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.print("Set ESP STA mode ... ");
  WiFi.mode(WIFI_STA);

//  Serial.print("Setting up Wifi config ... ");
//  WiFi.config(ip, gateway, subnet);
//  Serial.println("Done!"); 

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
}

void loop() {
  //WiFiClient client;
  HTTPClient http;  //Declare an object of class HTTPClient
  String url = "http://";
  url += host;
  http.begin(url);  //Specify request destination
  String value = "0";
  if (http.GET()) { //Check the returning code
  value = http.getString();
  Serial.println(value);                     //Print the response payload
  }else{
    Serial.println("Failed");
  } 
  http.end();
  if(value == "1"){
    Serial.println("vibrating");
    vibrate();
  }
  delay(100);
}
