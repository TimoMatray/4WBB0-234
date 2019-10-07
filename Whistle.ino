#include <ESP8266WiFi.h>
#include <Adafruit_BMP085.h>

IPAddress local_IP(192,168,4,21);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

Adafruit_BMP085 bmp;

bool readSensor() {
  int history[5];
start:
  delay(50);
  for(int i = 4; i > 0; i--){
    history[i] = history[i-1];
  }
  history[0] = bmp.readPressure();
  //Serial.print("First measurement: ");
  //Serial.println(history[4]);
  //Serial.print("Second measurement: ");
  //Serial.println(history[3]);
  //Serial.print("Third measurement: ");
  //Serial.println(history[2]);
  //Serial.print("Fourth measurement: ");
  //Serial.println(history[1]);
  //Serial.print("Fifth measurement: ");
  //Serial.println(history[0]);
  if (history[4] + 1000 < history[0]) {
    return true;
  }
  else goto start;
}

void setup()
{
  Serial.begin(115200);
  //Serial.println();

  //Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  //Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("4WWB0-234","passsword") ? "Ready" : "Failed!");

  //Serial.print("Soft-AP IP address = ");
  //Serial.println(WiFi.softAPIP());

  Wire.pins(0, 2);
  Wire.begin(0, 2);
  
  if (!bmp.begin()) {
    Serial.println("No BMP180 / BMP085");
    while (1) {}
  }
}

void loop() {
  if(readSensor())Serial.println("activated");
  delay(500);
}
