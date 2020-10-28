/*
 * ESP8266 NodeMCU LED Control over WiFi Demo
 *
 * https://circuits4you.com
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

//ESP Web Server Library to host a web page
#include <ESP8266WebServer.h>

#include <AccelStepper.h>
 
#define motorPin1  14                // IN1 pin on the ULN2003A driver to pin D1 on NodeMCU board
#define motorPin2  12                // IN2 pin on the ULN2003A driver to pin D2 on NodeMCU board
#define motorPin3  13                // IN3 pin on the ULN2003A driver to pin D3 on NodeMCU board
#define motorPin4  15                // IN4 pin on the ULN2003A driver to pin D4 on NodeMCU board

AccelStepper stepper(AccelStepper::FULL4WIRE, motorPin1, motorPin3, motorPin2, motorPin4);

//---------------------------------------------------------------
//Our HTML webpage contents in program memory
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <style>
      .buttonOn {
        background-color: #4CAF50;
        border: none;
        color: white;
        padding: 15px 32px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 16px;
        margin: 4px 2px;
        cursor: pointer;
      }
      .buttonOff {
        background-color: #e45353;
        border: none;
        color: white;
        padding: 15px 32px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 16px;
        margin: 4px 2px;
        cursor: pointer;
      }
    </style>
  </head>
  <body>
    <center>
      <h2>Motor de passo</h2>
      <label for="vol">Volume (between 0 and 50):</label>
      <br><input type="range" id="vol" name="vol" min="0" max="600">
      <br><a href="motorOn" target="myIframe" class="buttonOn">Liga</button>
      <a href="motorOff" target="myIframe" class="buttonOff">Desliga</button>

      Motor State:<iframe name="myIframe" width="100" height="25" frameBorder="0"><br>
      <hr>
      <a href="https://circuits4you.com">circuits4you.com</a>
    </center>
  </body>
</html>
)=====";

//SSID and Password of your WiFi router
const char* ssid = "nomedowifi";
const char* password = "senhadowifi";

//Declare a global object variable from the ESP8266WebServer class.
ESP8266WebServer server(80); //Server on port 80

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 Serial.println("You called root page");
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleMotorOn() {
  //Aqui vai o código pra fazer quando clicar em ligar
}

void handleMotorOff() { 
 //Aqui vai o código pra fazer quando clicar em desligar
}

//==============================================================
//                  SETUP
//==============================================================
void setup(){
  // Change these to suit your stepper if you want
  stepper.setMaxSpeed(600);
  stepper.setAcceleration(60);
  stepper.moveTo(5000);

  Serial.begin(115200);
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
 
  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/motorOn", handleLEDon); //as Per  <a href="ledOn">, Subroutine to be called
  server.on("/motorOff", handleLEDoff);

  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests
}