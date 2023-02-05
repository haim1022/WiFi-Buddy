boolean buddyWebServer() //allows easier device configuration
{ 
  char remotePress = 0;

  remote.enableReceive(D6);
  if (WiFi.status() == WL_CONNECTED)
  {
    lcd.clear();
    if (mdnsF == 1)
      lcd.print("WiFiBuddy.local/");
    else
      lcd.print(WiFi.localIP().toString());
    remotePress = buddyRemote();
    while (!(remotePress = buddyRemote()) && WiFi.status() == WL_CONNECTED)
    {
      yield();
      if (remotePress) //prevent wrong return checks
        break;
    }
    if (WiFi.status() != WL_CONNECTED)
      goto error;
    remote.disableReceive();
    return 0;
  }
  else
  {
    error: lcd.clear();
    lcd.print("No connection!");
    lcd.setCursor(0, 1);
    lcd.print("Press any key...");
    remotePress = buddyRemote();
    while (!(remotePress = buddyRemote())) //wait for user press
    {
      yield();
      if (remotePress) //prevent wrong return checks
        break;
    }
    remote.disableReceive();
    return 1; //error
  }
}

void handleRoot()
{
  File f = SPIFFS.open("/index.html", "r"); //open file for reading
  if (!f)
  {
    Serial.println("file open failed");
    handleNotFound();
    return;
  }
  server.send(200, "text/html", f.readString());
  f.close();
}

void handleNotFound()
{
  String message = "Error 404\n\n";
  message += "Page Not Found...";
  server.send(404, "text/plain", message);
}

boolean startServer() //returns mdns server status
{
  if (WiFi.status() == WL_CONNECTED)
  {
    if (MDNS.begin("WiFiBuddy"))
    {
      mdnsF = 1;
      Serial.println("MDNS responder started");
    }
    server.on("/", handleRoot);
    server.on("/Keyboard", handleKeyboard);
    server.on("/Remote", handleRemote);
    server.on("/Settings", handleSettings);
    server.on("/Settings/Contrast", handleContrast);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
    ftpSrv.begin("wifibuddy","Aa123456");
    Serial.println("FTP server started");
  }
}

