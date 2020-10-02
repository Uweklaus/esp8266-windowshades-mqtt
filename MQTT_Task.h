// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.

void Handle_MQTT_Connect(uint32_t deltaTime) {

  // Stop if already connected.
    if (mqtt.connected()) {
      taskManager.StopTask(&Connect_MQTT_Task);
      taskManager.StartTask(&Publish_MQTT_Task);
      Serial.println("MQTT connection already established");
    return;
  }
  taskManager.StopTask(&Publish_MQTT_Task);
  Serial.print("Connecting to MQTT... ");

  if ((MQTT_Status = mqtt.connect()) != 0) { // connect will return 0 for connected

       Serial.println(mqtt.connectErrorString(MQTT_Status));
       Serial.println("Retrying MQTT connection later");
       mqtt.disconnect();
  }
  else {
       Serial.println("MQTT Connected! - Start publishing");
       taskManager.StopTask(&Connect_MQTT_Task);
       taskManager.StartTask(&Publish_MQTT_Task);
       Handle_MQTT_Publish(MsToTaskTime(120000));
  }
}

void Handle_MQTT_Publish(uint32_t deltaTime) {
  String msg_MQTT;
  String az="\"";
  if (mqtt.connected()) {
  msg_MQTT = "{"+az+"window0"+az+":{"+az+"CurrentPosition"+az+":"+String(currentPosition[0])+","+az+"TargetPosition"+az+":"+String(targetPosition[0])+","+az+"PositionState"+az+":"+String(positionState[0])+"}";
  msg_MQTT = msg_MQTT + ","+az+"window1"+az+":{"+az+"CurrentPosition"+az+":"+String(currentPosition[1])+","+az+"TargetPosition"+az+":"+String(targetPosition[1])+","+az+"PositionState"+az+":"+String(positionState[1])+"}}";
  if (! shadestatus.publish(msg_MQTT.c_str())) {
    Serial.println("Failed");
  } else {
    Serial.println("Published!");
  }
  } else {
    taskManager.StopTask(&Publish_MQTT_Task);
    taskManager.StartTask(&Connect_MQTT_Task);
    Serial.println("MQTT trying to reconnect");
  }
}

void Handle_Move_Publish(uint32_t deltaTime) {
  String msg_MQTT;
  String az="\"";
  if (mqtt.connected()) {
  msg_MQTT = "{"+az+"window0"+az+":{"+az+"CurrentPosition"+az+":"+String(currentPosition[0])+","+az+"TargetPosition"+az+":"+String(targetPosition[0])+","+az+"PositionState"+az+":"+String(positionState[0])+"}";
  msg_MQTT = msg_MQTT + ","+az+"window1"+az+":{"+az+"CurrentPosition"+az+":"+String(currentPosition[1])+","+az+"TargetPosition"+az+":"+String(targetPosition[1])+","+az+"PositionState"+az+":"+String(positionState[1])+"}}";
  if (! shadestatus.publish(msg_MQTT.c_str())) {
    Serial.println("Failed");
  } else {
    Serial.println("Published while Moving!");
  }
  } else {
    taskManager.StopTask(&Publish_Move_Task);
    taskManager.StartTask(&Connect_MQTT_Task);
  }
  if (positionState[0] == 2 and positionState[1] == 2) {
    taskManager.StopTask(&Publish_Move_Task);
  } 
 }

 void Handle_Check_Publish(uint32_t deltaTime) {
    if (!(positionState[0] == 2 and positionState[1] == 2)) {
      taskManager.StartTask(&Publish_Move_Task);
    }
 }
 
