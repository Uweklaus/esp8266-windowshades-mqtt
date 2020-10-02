// HTTP requests are defined here

serverWS.on("/", handleRoot);
serverWS.on("/standard",loadStandardValuesAP);
serverWS.on("/root",handleAPRoot);
serverWS.on("/ssid",handleAPSSID);
serverWS.on("/favicon.ico", HTTP_GET, [](){Serial.println("get icon");});

serverWS.on("/setupTime", handleTiming);
serverWS.on("/setupPin", handlePin);
serverWS.on("/setCurrent", handleCurrent);
serverWS.on("/Tpost", handleTPost);
serverWS.on("/Ppost", handlePPost);
serverWS.on("/Cpost", handleCPost);
serverWS.on("/save", handleSave);

serverWS.on("/window/0/targetSetPosition", HTTP_POST, handleTargetPosition0); 
serverWS.on("/window/1/targetSetPosition", HTTP_POST, handleTargetPosition1);

serverWS.onNotFound(handleNotFound);
