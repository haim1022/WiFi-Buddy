char buddyRemote() //returns chosen action or '0' if none, use remote.enableReceive(D6) before calling function
{
  if (remote.available())
  {
    int value = remote.getReceivedValue();
    remote.resetAvailable();
    delay(100); //prevent double press
    if ((value & 0xFFFF00) == 0x2DD900)
      value = value & 0x0000FF;
    if ((value & 0xF0) == 0x00)
      return value;
  }
  else if (WiFi.status() == WL_CONNECTED)
  {
    ftpSrv.handleFTP();
    server.handleClient();
    if(server.hasArg("name"))
    {
      Serial.println(server.arg("name"));
      delay(10);
      if(server.arg("name") == "UP")
        return 1;
      else if(server.arg("name") == "ENTER")
        return 2;
      else if(server.arg("name") == "DOWN")
        return 4;
      else
        return 0;
    }
  }
  return 0;
}

void handleRemote() //Webapp
{
  File f = SPIFFS.open("/Remote.html", "r"); //open file for reading
  if (!f)
  {
    Serial.println("file open failed");
    handleNotFound();
    return;
  }
  String page = f.readString();
  server.send(200, "text/html", page);
  f.close();
}
