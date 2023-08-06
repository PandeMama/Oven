#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <MAX6675.h>

// Replace with your network credentials
const char *ssid = "your_SSID";
const char *password = "your_PASSWORD";

// Define MAX6675 pins
int thermDO = 13;
int thermCS = 12;
int thermCLK = 33;

// Initialize MAX6675
MAX6675 thermocouple(thermCLK, thermCS, thermDO);

// Create an instance of the server
AsyncWebServer server(80);

void setup()
{
  // Start the Serial communication to send messages to the computer
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println(WiFi.localIP());

  // Serve temperature on /temp/1 endpoint
  server.on("/temp/1", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    float temperature = thermocouple.readCelsius();
    char tempStr[10];
    snprintf(tempStr, sizeof(tempStr), "%.2f", temperature);
    request->send(200, "text/plain", tempStr); });

  // Serve index.html on root endpoint
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", "text/html"); });

  server.begin();
}

void loop()
{
  // No repeated functionality needed at this point
}
