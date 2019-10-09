/*
 * This is the wifi-STA code for esp to esp connection
 */
#include <ESP8266WiFi.h>

/* wifi config */
const char *ssid = "4WWB0-234";
const char *pass = "passsword";

/* network config */
IPAddress ip(192, 168, 4, 2);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

/* client config */
IPAddress host(192, 168, 4, 1); // should be an array of hosts in case of multiple armbands
const int port = 80;

/* vibration code */
void vibrate()
{
  analogWrite(LED,1023);
  delay(250);
  analogWrite(LED,0);
  delay(250);
  analogWrite(LED,1023);
  delay(250);
  analogWrite(LED,0);
  delay(250);
  analogWrite(LED,1023);
  delay(500);
  analogWrite(LED,0);
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.print("Set ESP STA mode ... ");
  WiFi.mode(WIFI_STA);

  Serial.print("Setting up Wifi config ... ");
  WiFi.config(ip, gateway, subnet);
  Serial.println("Done!"); 

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
  WiFiClient client;

  Serial.println("\nConnecting to host ... ");
  if (client.connect(host, port)) {
    Serial.println("Established TCP connection!");

    // send message to host
    client.println("hello from client!");

    //read back one line from server
    Serial.println("receiving from remote server");
    String line = client.readStringUntil('\r');
    Serial.println(line);

    Serial.println("closing connection");
    client.stop();
  } else {
    Serial.println("Could not establish TCP connection ... ");
  }
  
  Serial.println("wait 5 sec...");
  delay(5000);
}
