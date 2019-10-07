/*
 * This is the client code for esp to esp connection
 */

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

/* wifi config */
const char *ssid = "4WWBO-234";
const char *pass = "passsword";

/* network config */
IPAddress ip(192, 168, 4, 88);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

/* host config */
const int port = 4000;
const char *host = "192.168.4.99";

WiFiClient client;

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.print("Set ESP STA mode ... ");
  Serial.println(WiFi.mode(WIFI_STA) == WIFI_STA ? "Ready!" : "Failed!");

  Serial.print("Setting up Wifi config ... ");
  WiFi.config(ip, gateway, subnet);
  Serial.print("Done!"); 

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
  Serial.printf("\n[Connecting to %s ... ", host);
  
  if (client.connect(host, port)) {
    Serial.println("connected]");

    Serial.println("[Sending a request]");
    client.print(String("GET /") + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n" +
                 "\r\n"
                );

    Serial.println("[Response:]");
    while (client.connected() || client.available()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    }
    client.stop();
    Serial.println("\n[Disconnected]");
  }
  else {
    Serial.println("connection failed!]");
    client.stop();
  }
  delay(5000);
}

void connect() {
  if (client.connect(host, port)) {
    Serial.println("connected!");
  } else {
    Serial.print(".");
  }
}

bool queryServer() {
  
}
