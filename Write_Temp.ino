void writeTemp (){
  
  //Write the calculated temp out to the serial port for transmission in JSON format
  Serial.print("{\"Sensor_ID\":7,\"Sensor_Value\":");
  Serial.print(sensorVal);
  Serial.print(",\"Voltage\":\"");
  
  Serial.print(voltage);
  Serial.print("\",\"Temperature\":\"");
 
  Serial.print(temperature);
  
  Serial.println("\"}");
}

