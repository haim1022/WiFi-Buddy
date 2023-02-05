boolean buddyCheckConnection() //check if already connected to WiFi and ask user if to stay
{
  String answer;
  
  Serial.println(WiFi.status());
  if (WiFi.status() == WL_CONNECTED)
  {
    lcdBuffer[0] = WiFi.SSID();
    lcdBuffer[1] = "Stay connected";
    lcdBuffer[2] = "Disconnect";
    Serial.println(lcdBuffer[0]);
    answer = buddyDisplayMenu(3, lcdBuffer);
    lcd.clear();
    lcd.print("OK!");
    delay(500);
    if (answer == "Stay connected")
      return 0;
    else if (answer == "Disconnect")
    {
      WiFi.disconnect();
      return 1;
    }
  }
  else if (WiFi.status() == WL_DISCONNECTED)
    return 1;
  else if (WiFi.status() == WL_IDLE_STATUS)
  {
    if (WiFi.SSID() == 0)
      return 1;
    else
      return 0;
  }
  else
  {
    lcd.clear();
    lcd.print("Unknown Error: ");
    lcd.print(WiFi.status());
    delay(1000);
    return 1;
  }
}
