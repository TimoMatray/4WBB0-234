#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_BMP085.h>

const char *ssid = "4WWB0-234";
const char *pass = "jankinator9000";

IPAddress ip(192, 168, 11, 4);
IPAddress gateway(192, 168, 11, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server;
Adafruit_BMP085 bmp;
bool bmp_connected;

// serverside parameters
int threshold = 1000;
int flag = 0;
int ref_pressure = 0;
int restart_client = 0;

// clientside parameters
int iter = 3;
int delay_a = 250;
int delay_b = 250;
int delay_loop = 150;

void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(ssid, pass, 1, false, 4); // max 4 connections

  Serial.begin(115200);
  Serial.println();
  Serial.print("IP adress: ");
  Serial.println(ip);

  server.on("/", handleIndex);
  server.on("/access", handleAccess);
  server.on("/changeFlag", changeFlag);
  server.on("/changeThreshold", changeThreshold);
  server.on("/getDelayA", getDelayA); // possible to pass on args to change value
  server.on("/getDelayB", getDelayB);
  server.on("/getLoopDelay", getLoopDelay);
  server.on("/getIter", getIter);
  server.on("/restartServer", restartServer);
  server.on("/restartClient", restartClient);
  server.onNotFound(handleIndex);
  server.begin();

  Wire.pins(1, 2); // configure input BMP
  Wire.begin(1, 2);
  
  if (!bmp.begin()) {
    Serial.println("No BMP180 / BMP085");
    ref_pressure = 0; // fallback
    bmp_connected = false;
  } else {
    Serial.println("Read initial pressure ...");
    ref_pressure = bmp.readPressure();
    bmp_connected = true;
  }
}

void loop(){
  if (bmp_connected) {
    flag = readSensor();
  }
  server.handleClient();
}

/**
 * responds with information about server parameters
 */
void handleIndex() {
  server.send(200, "text/plain", 
    "flag: " + String(flag) + "\nthreshold: " + String(threshold) + "\nref_pressure: " + ref_pressure
    + "\n\ndelay_a: " + String(delay_a) + "\ndelay_b: " + String(delay_b) + "\niter: " + iter);
}

/**
 * called when client makes http request, resets flagy
 */
void handleAccess() {
  server.send(200, "text/plain", "" + String(flag) + String(restart_client));
  flag = 0; // reset
  restart_client = 0;
}


/**
 * changes the flag via webaccess
 */
void changeFlag() {
  flag = server.arg("value").toInt();
  server.send(200, "text/plain", String(flag));
}

/**
 * changes the threshold via webaccess
 */
void changeThreshold() {
  threshold = server.arg("value").toInt();
  server.send(200, "text/plain", String(threshold));
}

/**
 * get client delay A
 * Also possible to change the value via arg
 */
void getDelayA() {
  String newDelay = server.arg("value");
  if (newDelay != "") {
    delay_a = newDelay.toInt();
  }
  server.send(200, "text/plain", String(delay_a));
}

/**
 * get client delay B
 * Also possible to change the value via arg
 */
void getDelayB() {
  String newDelay = server.arg("value");
  if (newDelay != "") {
    delay_b = newDelay.toInt();
  }
  server.send(200, "text/plain", String(delay_b));
}

/**
 * get client delay B
 * Also possible to change the value via arg
 */
void getLoopDelay() {
  String newDelay = server.arg("value");
  if (newDelay != "") {
    delay_loop = newDelay.toInt();
  }
  server.send(200, "text/plain", String(delay_loop));
}

/**
 * get client delay A (delay, iterations for vibration)
 * Also possible to change the value via arg
 */
void getIter() {
  String newIter = server.arg("value");
  if (newIter != "") {
    iter = newIter.toInt();
  }
  server.send(200, "text/plain", String(iter));
}

/**
 * restarts the CPU
 */
void restartServer() {
  ESP.restart();
}

/**
 * restarts the CPU
 */
void restartClient() {
  restart_client = 1;
}

/**
 * reads BMP pressure and compares to reference pressure.
 * returns 1 if there has been physical input, and 0 if there hasn't.
 */
int readSensor() {
  if (bmp.readPressure() - threshold > ref_pressure) {
    Serial.println("High");
    return 1;
  }
  else return 0;
}
