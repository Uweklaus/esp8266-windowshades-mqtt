// HTTP Client
void Handle_Connect_Client(uint32_t deltaTime) {
  // Connect to WiFi network
  Serial.print("Connecting to WIFI");
  WiFi.mode(WIFI_STA);
  WiFi.hostname(HOST_NAME);
  WiFi.begin(ssid,password);

  if (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  } else {
    taskManager.StopTask(&Connect_Client_Task);
    taskManager.StartTask(&Check_Client_Task); 
    Serial.println("connected"); 
  }  
}

void Handle_Check_Client(uint32_t deltaTime) { 
  if (WiFi.status() != WL_CONNECTED) {
    taskManager.StopTask(&Check_Client_Task);
    taskManager.StartTask(&Connect_Client_Task);
  } else {
    serverWS.handleClient();
  }
}
