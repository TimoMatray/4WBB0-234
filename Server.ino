#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_BMP085.h>

char * ssid_ap = "4WWB0-234";
char * password_ap = "passsword";

IPAddress ip(192, 168, 11, 4);
IPAddress gateway(192, 168, 11, 1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server;

Adafruit_BMP085 bmp;
const int threshold = 1000;

String whistle = "0";

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

  Wire.pins(0, 2);
  Wire.begin(0, 2);
  if (!bmp.begin()) {
    Serial.println("No BMP180 / BMP085");
  }
}

void loop(){
  whistle = readSensor();
  server.handleClient();
}

void handleIndex(){
  server.send(200,"text/plain",whistle);
  whistle = "0";
}

String readSensor() {
  int history[5];
  for(int i = 0; i > 0; i++){
      history[i] = bmp.readPressure();
      delay(50);
  }
  if (history[0] + threshold < history[4]) {
    return "1";
  }
  else return "0";
}
