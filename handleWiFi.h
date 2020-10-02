// Handling WiFi and HTTP for window shades

// functions to handle http requests

void handleSave (){
  char temp[256];  
   Serial.println("Save");

    saveConfig();
    Serial.println("Config saved");
    snprintf ( temp, 256,
"<html><head><title>Save Config</title>\
<meta http-equiv='refresh' content='2; URL=/'></head>\
<body>Config saved to EEPROM</body></html>");
  serverWS.send ( 200, "text/html", temp );
}

void handleTPost() {
  char temp[256];
  Serial.println("TPost");
  
  for ( uint8_t i = 0; i < serverWS.args(); i++ ) {
    windowTime[i] = serverWS.arg(i).toInt();
  }
  snprintf ( temp, 256,
"<html><head><title>Timing Setup</title>\
<meta http-equiv='refresh' content='1; URL=/'></head>\
<body><b>Do NOT forget to SAVE the values!</b></body></html>");
  serverWS.send ( 200, "text/html", temp );
}

void handleTiming() {
  char temp[TIMING_PAGE_LEN]; 
  Serial.println("Timing");

  snprintf ( temp, TIMING_PAGE_LEN,TIMING_PAGE,

    windowTime[0], windowTime[1],windowTime[2], windowTime[3]
  );
  serverWS.send ( 200, "text/html", temp );
}

void handlePPost() {
  char temp[500];
  bool flag=true;
  int help=0,checksum1=0,checksum2=0; 
  Serial.println("PPost");

/*  for ( uint8_t i = 0; i < serverWS.args(); i++ ) {
    help = serverWS.arg(i).toInt();
    if (i==0|i==1|i==4|i==5){
      if (help==0|help==2|help==4|help==5){checksum1+=help;}
    }
    else {
      if (help==12|help==13|help==14|help==16){checksum2+=help;}      
    }
  }
  if (checksum1!=11|checksum2!=55) {flag=false;} */
  flag=true;                                                  // Keine Prüfung, da out=5,4
  if (flag) {
   SwitchPin[0]=byte(serverWS.arg(0).toInt());
   SwitchPin[1]=byte(serverWS.arg(1).toInt());
   RelayPin[0]=byte(serverWS.arg(2).toInt());
   RelayPin[1]=byte(serverWS.arg(3).toInt());   
   SwitchPin[2]=byte(serverWS.arg(4).toInt());
   SwitchPin[3]=byte(serverWS.arg(5).toInt());
   RelayPin[2]=byte(serverWS.arg(6).toInt());
   RelayPin[3]=byte(serverWS.arg(7).toInt());

  snprintf ( temp, 500,
  "<html><head><title>Pin Setup</title>\
  <meta http-equiv='refresh' content='1; URL=/'></head>\
  <body><b>Do NOT forget to SAVE the values!</b></body></html>");
  serverWS.send ( 200, "text/html", temp );
  }
  else {
  snprintf ( temp, 500,
  "<html><head><title>Pin Setup</title>\
  <meta http-equiv='refresh' content='3; URL=/setupPin'></head>\
  <body>Wrong Pin Assingment - Please correct.</body></html>");
  serverWS.send ( 200, "text/html", temp );
  }
}

void handlePin() {
  char temp[PIN_PAGE_LEN]; 
 Serial.println("pin");
   
  snprintf ( temp, PIN_PAGE_LEN, PIN_PAGE,
    SwitchPin[0], SwitchPin[1], RelayPin[0], RelayPin[1],
    SwitchPin[2], SwitchPin[3], RelayPin[2], RelayPin[3]
  );
  serverWS.send ( 200, "text/html", temp );

}
 

void handleRoot() {

  char temp[HOME_PAGE_LEN];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

 Serial.println("root");
  snprintf( temp, HOME_PAGE_LEN, HOME_PAGE,
    hr, min % 60, sec % 60,
    HOST_NAME,
    ssid,ip[0],ip[1],ip[2],ip[3],
    IPMQTT,AIO_SERVERPORT,MQTT_PATH,
    currentPosition[0],targetPosition[0],positionState[0],
    currentPosition[1],targetPosition[1],positionState[1],
    SwitchPin[0], SwitchPin[1], RelayPin[0], RelayPin[1],
    windowTime[0], windowTime[1],
    SwitchPin[2], SwitchPin[3], RelayPin[2], RelayPin[3],
    windowTime[2], windowTime[3] 
    );

  serverWS.send ( 200, "text/html", temp );
}

void handleCurrent() {

  char temp[CURRENT_PAGE_LEN];

 Serial.println("Current");
  snprintf( temp, CURRENT_PAGE_LEN, CURRENT_PAGE,   
    currentPosition[0], currentPosition[1]
    );

  serverWS.send ( 200, "text/html", temp );
}

void handleCPost() {
  char temp[256];
  Serial.println("CPost");
  
  for ( uint8_t i = 0; i < serverWS.args(); i++ ) {
    currentPosition[i] = serverWS.arg(i).toInt();}
  targetPosition[0] = currentPosition[0];
  targetPosition[1] = currentPosition[1];
  Handle_MQTT_Publish(MsToTaskTime(120000));
  snprintf ( temp, 256,
"<html><head><title>Set Current Position</title>\
<meta http-equiv='refresh' content='1; URL=/'></head>\
<body><b>Current and Target Position set!</b></body></html>");
  serverWS.send ( 200, "text/html", temp );
  
}

  void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += serverWS.uri();
  message += "\nMethod: ";
  message += (serverWS.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += serverWS.args();
  message += "\n";
  for (uint8_t i=0; i<serverWS.args(); i++){
    message += " " + serverWS.argName(i) + ": " + serverWS.arg(i) + "\n";
  }
  serverWS.send(404, "text/plain", message);
  Serial.println(message);
  }


void handleTargetPosition(bool _window) {
 
  Serial.println("set TP window " + String(_window)+"."+serverWS.args());
    serverWS.send ( 200, "text/html", "" );

  Serial.println(serverWS.uri());
  Serial.println(serverWS.arg(0));
  Serial.println(serverWS.argName(0));
  Serial.println();
  if (serverWS.args()!=0 ){ 
    int newTargetPosition = serverWS.arg(0).toInt();
    if (newTargetPosition == 0 && serverWS.arg(0) != "0") {
    Serial.println("hTP wrong argument"+String(serverWS.args())+"/"+String(serverWS.arg("value")));   
      handleNotFound();
      return;
    }

    if (newTargetPosition < 0 || newTargetPosition > 100
      || (targetPosition[_window] == -1 && newTargetPosition == currentPosition[_window])) {
      Serial.println("hTP out of range");         
      handleNotFound();
      return;
    }

    targetPosition[_window] = newTargetPosition;

    void closeWindow(bool _window);
    void openWindow(bool _window);
    
    Serial.println("Target position Window "+String(_window)+" :" + targetPosition[_window]);
    if (newTargetPosition < currentPosition[_window]) {
      closeWindow(_window);
    }
    if (newTargetPosition > currentPosition[_window]) {
      openWindow(_window);
    }
    serverWS.send(200, "text/plain", "OK");
  } 
  else {
    Serial.println("no arguments");
    handleNotFound();
  }
}

void handleTargetPosition0 () {handleTargetPosition(0);}
void handleTargetPosition1 () {handleTargetPosition(1);}
