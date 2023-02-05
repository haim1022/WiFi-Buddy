String buddySaveSSID(String _ssid)
{
  if(_ssid.charAt(_ssid.length() - 1) != ' ')
    return "0"; //unecrypted
  else
  {
    _ssid.remove(_ssid.length() - 1);
    ssid.remove(ssid.length() - 1);
  }
  if(!SPIFFS.exists("/Login.txt"))
  {
    lcd.clear();
    lcd.print("Couldn't find");
    lcd.setCursor(0, 1);
    lcd.print("login file!");
    delay(1000);
    lcd.clear();
    lcd.print("Creating...");
    delay(1000);
    File f = SPIFFS.open("/Login.txt", "w");
    lcd.clear();
    if (!f)
    {
      lcd.print("Failed creating");
      lcd.setCursor(0, 1);
      lcd.print("login file!");
      return "1";
    }
    f.close();
  }
  File f = SPIFFS.open("/Login.txt", "r+");
  if (!f)
  {
    lcd.print("Couldn't open");
    lcd.setCursor(0, 1);
    lcd.print("login file!");
    delay(1000);
    return "1";
  }
  while(f.available())
  {
    String line = f.readStringUntil(0xA); //read until LF (not included)
    line.remove(line.length() - 1); //remove CR
    if(_ssid == line)
    {
      //ask if to use existing password
      lcdBuffer[0] = "Use saved pass?";
      lcdBuffer[1] = "Yes";
      lcdBuffer[2] = "No";
      String answer = buddyDisplayMenu(3, lcdBuffer);
      if(answer == "Yes")
      {
        String _pass = f.readStringUntil(0xA);
        _pass.remove(_pass.length() - 1);
        return _pass;
      }
      else
      {
        String _pass = buddyUserInput("New password:", "New password:");
        f.println(_pass);
        return _pass;
      }
    }
  }
  String _pass = buddyUserInput("Enter password: ", "Enter password: ");
  f.println(_ssid);
  f.println(_pass);
  lcd.clear();
  lcd.print("Password saved!");
  delay(1000);
  return _pass;
}
