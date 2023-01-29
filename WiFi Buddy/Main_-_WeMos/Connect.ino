boolean buddyConnect(String _ssid, String _password) //connect to specified SSID, with animation and stuff
{
  String info[8];
  unsigned long startTime;
  
  WiFi.disconnect();
  if(_password == "0")
    WiFi.begin(_ssid.c_str());
  else
    WiFi.begin(_ssid.c_str(), _password.c_str());
  startTime = millis();
  lcd.clear();
  lcd.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED)
  {
    yield();
    lcd.setCursor(0, 1);
    for (int counter = 0; counter < 16 && WiFi.status() != WL_CONNECTED; counter++)
    {
      for (int counter1 = 0; counter1 < 6; counter1++)
      {
        lcd.setCursor(counter, 1);
        lcd.write(counter1);
        delay(10);
      }
    }
    for (int counter = 16; counter >= 0 && WiFi.status() != WL_CONNECTED; counter--)
    {
      for (int counter1 = 5; counter1 >= 0; counter1--)
      {
        lcd.setCursor(counter, 1);
        lcd.write(counter1);
        delay(10);
      }
    }
    if ((WiFi.status() == WL_CONNECT_FAILED)  || ((millis() - startTime) / 1000) >= 15)
      return 1;
  }
  info[0] = "Connected to: ";
  info[1] = WiFi.SSID();
  info[2] = "Local IP: ";
  info[3] = WiFi.localIP().toString();
  info[4] = "MAC Address: ";
  info[5] = WiFi.macAddress();
  info[6] = "Signal strength is: ";
  info[7] = String(WiFi.RSSI()) + " db";
  for (int counter = 0; counter < 8; counter = counter + 2)
  {
    lcd.clear();
    lcd.print(info[counter]);
    lcd.setCursor(0, 1);
    lcd.print(info[counter + 1]);
    delay(1500);
  }
  return 0;
}
