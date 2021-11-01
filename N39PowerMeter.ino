#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>

#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK "your-password"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);

volatile bool first_pulse = true;
volatile unsigned long pulse_time[2] = {};
volatile unsigned long delta = 0;
volatile unsigned long pulse_counter = 0;

ICACHE_RAM_ATTR void IntCallback() {
  pulse_time[1] = pulse_time[0];
  pulse_time[0] = millis();
  pulse_counter++;
  if (first_pulse) {
    first_pulse = false;
  } else {
    delta = pulse_time[0] - pulse_time[1];
  }
}

void handleRoot() {
  double current_power_usage_watts = 3600000.0 / delta;
  double kwh_counter = pulse_counter / 1000.0;

  char buffer[300] = {};

  snprintf(buffer, 300, "{\"current_watts\":\"%f\", \"kwh_counter\":\"%f\"}",
           current_power_usage_watts, kwh_counter);

  server.send(200, "application/json", buffer);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(13, INPUT);
  attachInterrupt(digitalPinToInterrupt(13), IntCallback, FALLING);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("power_meter_0")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

// the loop routine runs over and over again forever:
void loop() {
  server.handleClient();
  MDNS.update();
}
