#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <FS.h>
#include <ESP8266WebServer.h>
#include <AccelStepper.h>


#define motorPin1  14                // IN1 pin on the ULN2003A driver to pin D1 on NodeMCU board
#define motorPin2  12                // IN2 pin on the ULN2003A driver to pin D2 on NodeMCU board
#define motorPin3  13                // IN3 pin on the ULN2003A driver to pin D3  on NodeMCU board
#define motorPin4  15                // IN4 pin on the ULN2003A driver to pin D4 on NodeMCU board

// Define a stepper and the pins it will use
AccelStepper stepper(AccelStepper::FULL4WIRE, motorPin1, motorPin3, motorPin2, motorPin4);

//Strings para inserir a página HTML
String webPageDesligado = "";
String webPageLigado = "";


//SSID e Senha da rede wifi
const char* ssid = "ALEX DANTAS 2.4G";
const char* password = "americadenatal";

//Declare a global object variable from the ESP8266WebServer class.
ESP8266WebServer server(80); //Server on port 80

//---------------------------------------------------------------------------------------------------------

//Função que abre a página inicial
void handleRoot() {
  server.send(200, "text/html", webPageDesligado);
}

void handleMotorOn() {
  stepper.setMaxSpeed(200);    
  server.send(200, "text/html", webPageLigado);
}

void handleMotorOff() {
  stepper.setMaxSpeed(0);    
  server.send(200, "text/html", webPageDesligado);
}

void handleChangeVel() {
  
  String argumento = server.argName(0);
  String valorstr = server.arg(0);
  Serial.println(argumento + valorstr);  
  stepper.setMaxSpeed(valorstr.toInt());
  server.send(200, "text/html", webPageLigado);
}

//Faz a leitura das páginas HTML
void readFile(void) 
{
  //Faz a leitura do arquivo HTML
  File rFile1 = SPIFFS.open("/desligado.html","r");
  Serial.println("Lendo arquivo HTML...");
  webPageDesligado = rFile1.readString();
  File rFile2 = SPIFFS.open("/ligado.html","r");
  Serial.println("Lendo arquivo HTML...");
  webPageLigado = rFile2.readString();
  rFile1.close();  
  rFile2.close();  
}

void setup() {  
  // Change these to suit your stepper if you want
  stepper.setMaxSpeed(0);
  stepper.setAcceleration(100);
  stepper.moveTo(25000);

  Serial.begin(9600);
  SPIFFS.begin();
 
  if(SPIFFS.exists("/index.html"))
  {
    Serial.println("\n\nfile exists!");
  }
  else Serial.println("\n\nNo File :(");
  readFile();

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
  server.on("/motorOn", handleMotorOn);
  server.on("/motorOff", handleMotorOff);
  server.on("/changeVel", handleChangeVel);
  server.begin();                  //Start server
  Serial.println("HTTP server started");
  
}

//==============================================================
//                     LOOP
//==============================================================

void loop() {  
    server.handleClient(); //Handle client requests
    
    //Inicia o motor no modo pêndulo
    if (stepper.distanceToGo() == 0)
      stepper.moveTo(-stepper.currentPosition());
    stepper.run();
}
