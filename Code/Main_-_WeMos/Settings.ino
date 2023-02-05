boolean buddySettings() //settings...
{
  char remotePress = 0;
  
  Settings: lcdBuffer[0] = "Settings: ";
  lcdBuffer[1] = "1.On Device";
  lcdBuffer[2] = "2.Web Server";
  lcdBuffer[3] = "3.FTP Server";
  lcdBuffer[4] = "Go Back";
  String menuSelection = buddyDisplayMenu(5, lcdBuffer);
  if (menuSelection == "1.On Device")
  {
    OnDevice: lcdBuffer[0] = "Settings: ";
    lcdBuffer[1] = "1.WiFi";
    lcdBuffer[2] = "2.Contrast";
    lcdBuffer[3] = "Go Back";
    menuSelection = buddyDisplayMenu(4, lcdBuffer);
    if (menuSelection == "1.WiFi")
    {
      if (buddyCheckConnection())
      {
        reconnect: ssid = buddyDisplayMenu(buddyFindWifi(), lcdBuffer);
        if (ssid == "Reload list...")
          goto reconnect;
        pass = buddySaveSSID(ssid); //"buddySaveSSID" returns password
        if (buddyConnect(ssid, pass))
        {
          lcd.clear();
          lcd.print("Unsuccessful!");
          lcd.setCursor(0, 1);
          lcd.print("Try again...");
          delay(2000);
          goto reconnect;
        }
      }
      goto OnDevice;
    }
    else if (menuSelection == "2.Contrast")
    {
      remote.enableReceive(D6);
      lcd.clear();
      lcd.print("Contrast: ");
      start: remotePress = buddyRemote();
      while (!(remotePress = buddyRemote())) //wait for user press
      {
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor((contrastVal/25), 1);
        lcd.write(5);
        delay(10); //stable refresh rate
        yield();
        if (remotePress) //prevent wrong return checks
          break;
      }
      if (remotePress == OK)
        goto OnDevice;
      else if(remotePress == UP && contrastVal <= 375)
        contrastVal += 25;
      else if (remotePress == DOWN && contrastVal >= 25)
        contrastVal -= 25;
      analogWrite(contrastPin, contrastVal);
      goto start;
    }
    else if (menuSelection == "Go Back")
      goto Settings;
  }
  else if (menuSelection == "2.Web Server")
  {
    buddyWebServer();
    goto Settings;
  }
  else if (menuSelection == "3.FTP Server")
  {
    buddyFTP();
    goto Settings;
  }
  else if (menuSelection == "Go Back")
    return 0;
}

void handleSettings()
{
  File f = SPIFFS.open("/Settings.html", "r"); //open file for reading
  if (!f)
  {
    Serial.println("file open failed");
    handleNotFound();
    return;
  }
  server.send(200, "text/html", f.readString());
  f.close();
}

void handleContrast()
{
  File f = SPIFFS.open("/Contrast.html", "r"); //open file for reading
  if (!f)
  {
    Serial.println("file open failed");
    handleNotFound();
    return;
  }
  if(server.hasArg("Contrast_data"))
  {
    contrastVal = server.arg("Contrast_data").toInt();
    analogWrite(contrastPin, contrastVal);
  }
  String page = f.readString();
  page.replace("value=\"0\"", "value=\"" + String(contrastVal) + "\"");
  server.send(200, "text/html", page);
  f.close();
}

