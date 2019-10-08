/*
 * This is the wifi-AP code for esp to esp connection
 */
#include <ESP8266WiFi.h>
//#include <Adafruit_BMP085.h>

/* wifi config */
const char *ssid = "4WWB0-234";
const char *pass = "passsword";

/* network config */
IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

/* host config */
const int port = 80;
WiFiServer server(port);

//Adafruit_BMP085 bmp;

const int threshold = 1000;

void setup()
{
  Serial.begin(115200);
  Serial.println();

  WiFi.mode(WIFI_AP);
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready!" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid, pass) ? "Ready!" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

//  Wire.pins(0, 2);
//  Wire.begin(0, 2);
//  
//  if (!bmp.begin()) {
//    Serial.println("No BMP180 / BMP085");
//  }
}

void loop() {
  WiFiClient client = server.available();
  
  // wait for a client (web browser) to connect
  if (client) {
    Serial.println("\nClient connected!");
    while (client.connected()) {
      // read line by line what the client is requesting
      if (client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        // wait for end of client's request, that is marked with an empty line
        if (line.length() == 1 && line[0] == '\n') {
          client.println("hello back from the server!");
          break;
        }
      }
    }
    delay(1); // give the web browser time to receive the data

    // close the connection:
    client.stop();
    Serial.println("Client disonnected!");
  }
  
//  if(readSensor())Serial.println("activated");
//  delay(500);
}

/*
 * returns true, if notable difference in airpressure
 */
//bool readSensor() {
//  int history[5];
//start:
//  delay(50);
//  for(int i = 4; i > 0; i--){
//    history[i] = history[i-1];
//  }
//  history[0] = bmp.readPressure();
//  //Serial.print("First measurement: ");
//  //Serial.println(history[4]);
//  //Serial.print("Second measurement: ");
//  //Serial.println(history[3]);
//  //Serial.print("Third measurement: ");
//  //Serial.println(history[2]);
//  //Serial.print("Fourth measurement: ");
//  //Serial.println(history[1]);
//  //Serial.print("Fifth measurement: ");
//  //Serial.println(history[0]);
//  if (history[4] + threshold < history[0]) {
//    return true;
//  }
//  else goto start;
//}
