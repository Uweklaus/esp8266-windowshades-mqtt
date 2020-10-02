
/*
 *  esp8266 shades MQTT
 *  
 *  http and mqtt implemented
 */
#define FIRMWARE "Firmware V 0.3.0"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

/* Here define the constants, different for each shade
 * ___________________________________________________________________________________ 
 */
IPAddress ip;
#define HOST_NAME "RolloD"
#define MQTT_PATH "RolloD/shadestatus"
byte SwitchPin [4] = {0,2,5,4}; // UP_Window 1, DOWN, UP_Window 2, DOWN D1,D2,D3,D4
byte RelayPin [4] = {12,13,14,16}; //UP_Window 1, DOWN, UP_Window 2, DOWN D6,D7,D5,D0
long windowTime [4] = {18000,18000,18000,18000}; //Window 1Up, 2Up, 1Down, 2Down 

#define IPMQTT "192.168.2.90"  // YOUR NODE-RED SERVER IP
#define AIO_SERVERPORT  1883
#define AIO_KEY         ""
/* 
 *  __________________________________________________________________________________
 */
#include "settings.h"

// WiFiServer server(1337);  // 80
ESP8266WebServer serverWS(80);

#include <Task.h> // für  Taskmanager Schaltertasks

// include sub files
#include "ButtonTask.h" // this implements the button task



// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, IPMQTT, AIO_SERVERPORT, "", AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'shadestatus' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>

Adafruit_MQTT_Publish shadestatus = Adafruit_MQTT_Publish(&mqtt, MQTT_PATH);

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).

void MQTT_connect();

//Definition der Variablen und Konstanten

enum PositionState {
  PS_DECREASING = 0,
  PS_INCREASING = 1,
  PS_STOPPED = 2
};

enum Direction {
  UP = 0,
  DOWN = 1
};

enum HttpMethod {
  HM_NONE,
  HM_GET,
  HM_POST
};


String msg_err = "";
String old_msg = "";

// für initialisierung - Rollo(s) soll komplett hochgefahren werden bei Start
int currentPosition [2] = {100,100};
int targetPosition [2] = {100,100};
PositionState positionState [2] = {PS_STOPPED, PS_STOPPED};

int8_t MQTT_Status;

TaskManager taskManager;
#include "ShadesTask.h"  // muss hier stehen, da taskManager verwendet wird

// Deklaration der Handles

void Handle_SWITCH1_UP(ButtonState state);
void Handle_SWITCH1_DOWN(ButtonState state);
void Handle_SWITCH2_UP(ButtonState state);
void Handle_SWITCH2_DOWN(ButtonState state);

void Handle_Window1_Move(bool x, int state){};
void Handle_Window2_Move(bool x, int state){};

void Handle_Check_Client(uint32_t deltaTime);
void Handle_Connect_Client(uint32_t deltaTime);
void Handle_MQTT_Connect(uint32_t deltaTime);
void Handle_MQTT_Publish(uint32_t deltaTime);
void Handle_Move_Publish(uint32_t deltaTime);
void Handle_Check_Publish(uint32_t deltaTime);

// foreward delcare functions passed to task constructors now required
ButtonTask SWITCH1_UP_Task(Handle_SWITCH1_UP, SwitchPin[0]);
ButtonTask SWITCH1_DOWN_Task(Handle_SWITCH1_DOWN, SwitchPin[1]);
ButtonTask SWITCH2_UP_Task(Handle_SWITCH2_UP, SwitchPin[2]);
ButtonTask SWITCH2_DOWN_Task(Handle_SWITCH2_DOWN, SwitchPin[3]);
MoveTask Windows1_Move_Up_Task (Handle_Window1_Move,false,1);
MoveTask Windows1_Move_Down_Task (Handle_Window1_Move,false,0);
MoveTask Windows2_Move_Up_Task (Handle_Window2_Move,true,1);
MoveTask Windows2_Move_Down_Task (Handle_Window2_Move,true,0);
FunctionTask Check_Client_Task(Handle_Check_Client, MsToTaskTime(8));
FunctionTask Connect_Client_Task(Handle_Connect_Client, MsToTaskTime(500));
FunctionTask Connect_MQTT_Task(Handle_MQTT_Connect, MsToTaskTime(2000));
FunctionTask Publish_MQTT_Task(Handle_MQTT_Publish, MsToTaskTime(120000));
FunctionTask Publish_Move_Task(Handle_Move_Publish, MsToTaskTime(1000));
FunctionTask Check_Publish_Task(Handle_Check_Publish, MsToTaskTime(50));

int wCounter = 0;
bool noWiFi = false;

// functions to handle http requests
#include "webpage_root.h"
#include "webpage_pin.h"
#include "webpage_timing.h";
#include "webpage_current.h";
#include "handleWiFi.h"
#include "handleWiFiAP.h"
/*
 * Setup
 */
void setup() {
  Serial.begin(115200);
  delay(10);  // überspielt Zeichen beim Reset
  Serial.println();
  Serial.println("Starting...");

  EEPROM.begin(512);
  delay(10);
  //EEPROM.write(0,0);
  if (!loadConfig()) {
    Serial.println("Standard values will be loaded");
    loadStandardValues();
  } else {
    Serial.println("Config loaded");
  }

  // Definition Eingangs und Ausgangs Pins, HIGH = nicht aktiv
  for (int i=0;i<4;i++) {
    pinMode (SwitchPin[i], INPUT_PULLUP);
    pinMode (RelayPin[i], OUTPUT);
    digitalWrite(RelayPin[i], HIGH);
  }
  
  Serial.println("");  
  Serial.println(FIRMWARE);
  Serial.println("Pin I/O state defined");
  Serial.println("");
  Serial.println("Shades should be initially open...");
  Serial.println("");
  
  #include "HTTP_Server_Requests.h"; 
  
  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.print(WiFi.SSID());
  WiFi.mode(WIFI_STA);
  WiFi.hostname(HOST_NAME);
  
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    wCounter += 1;
    delay(500);
    Serial.print(".");
    if (wCounter > 30) {
      wCounter=0;
      noWiFi=true;
      break;
    }
  }
  Serial.println("done");
  if (noWiFi) { 
    Serial.println("WiFi not connected - Standalone"); 
    Serial.println("Configuring access point...");
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ssidAP, passwordAP);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.print("AP SSID: ");
  Serial.println(ssidAP);
  Serial.print("AP password: ");
  Serial.println(passwordAP);
  
  serverWS.on("/", handleAPRoot);
  serverWS.on("/ssid",handleAPSSID);
  serverWS.on("/standard",loadStandardValuesAP);
  
  serverWS.begin();
  Serial.println("HTTP AP server started"); 
  }
  else { 
    Serial.println("WiFi connected");
    if (MDNS.begin("esp8266")) {
      Serial.println("MDNS responder started");
    }


// Start the server
    //    server.begin();
    serverWS.begin();
    Serial.println("Server started");
    ip = WiFi.localIP();
    Serial.println(ip);
  }

// Tasks definieren und starten
  taskManager.StartTask(&SWITCH1_UP_Task);
  taskManager.StartTask(&SWITCH1_DOWN_Task);
  taskManager.StartTask(&SWITCH2_UP_Task);
  taskManager.StartTask(&SWITCH2_DOWN_Task);

  taskManager.StopTask(&Windows1_Move_Up_Task);
  taskManager.StopTask(&Windows1_Move_Down_Task);  
  taskManager.StopTask(&Windows2_Move_Up_Task);
  taskManager.StopTask(&Windows2_Move_Down_Task);
  taskManager.StopTask(&Connect_Client_Task);  //reconnecting to WiFi
  taskManager.StartTask(&Check_Client_Task);  //listening to WiFi  
  taskManager.StartTask(&Connect_MQTT_Task); //connect to mqtt server
  taskManager.StopTask(&Publish_MQTT_Task); //MQTT publizieren
  taskManager.StartTask(&Publish_Move_Task); //MQTT publizieren wenn Bewegung
  taskManager.StartTask(&Check_Publish_Task);  
  Serial.println("Running...");
}

/*
 * Loop
 */
void loop() {

  taskManager.Loop();


}





#include "HTTP_Task.h";
#include "MQTT_Task.h";
#include "shadeswitches.h"; // includes switch_handling
#include "window_function.h"; // includes windowshade_handling
