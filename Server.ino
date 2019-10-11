#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

char * ssid_ap = "4WWB0-234";
char * password_ap = "passsword";

IPAddress ip(192, 168, 11, 4);
IPAddress gateway(192, 168, 11, 1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server;

float sensor_value = 0.0;

void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(ssid_ap,password_ap);

  Serial.begin(115200);
  Serial.println(); Serial.print("IP adress: "); Serial.println(ip);

  server.on("/",handleIndex);
  server.on("/test",handleIndex);
  server.onNotFound(handleIndex);
  server.begin();
}

void loop(){
  server.handleClient();
}

void handleIndex(){
  server.send(200,"text/plain",String(sensor_value));
}
