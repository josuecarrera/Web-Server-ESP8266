#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "network name";
const char* password = "network password";

const int output = 14; //(D5)

const char* PARAM_INPUT = "state";

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML><html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>ESP Web Server</title>
    <style>
      html {font-family: Arial; display: inline-block; text-align: center;}
      h2 {font-size: 2.3rem;}
      p {font-size: 1.9rem;}
      body {max-width: 400px; margin: 0px auto; padding-bottom: 25px;}
      .button {display: inline-block; background-color: #4CAF50; border: none; color: white; padding: 16px 40px; text-align: center; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; border-radius: 4px;}
      .button2 {background-color: #555555;}
      </style>
     </head>
     <body>
        <h2>ESP Web Server</h2>
        <p><button class="button" onclick="sendRequest('on')">ON</button></p>
        <p><button class="button button2" onclick="sendRequest('off')">OFF</button></p>
    <script>
      function sendRequest(state) {
          var xhr = new XMLHttpRequest();
          xhr.open("GET", "/update?state=" + state, true);
          xhr.send();
        }
    </script>
    </body>
    </html>
)rawliteral";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
  }
  
  Serial.println(WiFi.localIP()); 

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/html", index_html);
  });

  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
      String inputMessage;
      if (request->hasParam(PARAM_INPUT)) {
          inputMessage = request->getParam(PARAM_INPUT)->value();
          if (inputMessage == "on") {
              digitalWrite(output, HIGH); 
          } else if (inputMessage == "off") {
              digitalWrite(output, LOW); 
          }
      } else {
          inputMessage = "No state sent";
      }
      Serial.println(inputMessage);
      request->send(200, "text/plain", "OK");
  });

  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}
