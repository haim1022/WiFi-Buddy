boolean buddyFTP()
{
  char remotePress = 0;
  if(usingRemote)
    remote.enableReceive(D6);
  if (WiFi.status() == WL_CONNECTED)
  {
    lcd.clear();
    lcd.print("Connect to:");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP().toString());
    lcd.print(":21"); //port
    if(usingRemote)
      remotePress = buddyRemote();
    while (!remotePress && WiFi.status() == WL_CONNECTED)
    {
      yield();
      if (remotePress) //prevent wrong return checks
        break;
    }
    if (WiFi.status() != WL_CONNECTED)
      goto error;
    if(usingRemote)
      remote.disableReceive();
    return 0;
  }
  else
  {
    error: lcd.clear();
    lcd.print("No connection!");
    lcd.setCursor(0, 1);
    lcd.print("Press any key...");
    if(usingRemote)
      remotePress = buddyRemote();
    while (!remotePress) //wait for user press
    {
      yield();
      if (remotePress) //prevent wrong return checks
        break;
    }
    if(usingRemote)
      remote.disableReceive();
    return 1; //error
  }
}
