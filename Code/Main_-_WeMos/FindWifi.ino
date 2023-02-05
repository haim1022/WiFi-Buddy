byte buddyFindWifi() //returns amount of rows in array and set "lcdBuffer" as an array of available networks
{
  //memset(locked, 0, 32);
  lcd.clear();
  lcd.print("Scanning...");
  lcd.setCursor(0, 1);
  lcd.print("Please wait");
  lcdBuffer[0] = "Choose a WiFi:";
  int networksAmount = WiFi.scanNetworks();
  for (int counter = 0; counter < networksAmount; counter++)
  {
    lcdBuffer[counter + 1] = WiFi.SSID(counter);
    if(WiFi.encryptionType(counter) != 7)
      lcdBuffer[counter + 1] += ' ';
  }
  lcdBuffer[networksAmount + 1] = "Reload list...";
  return networksAmount + 2; //added title and reload option
}
