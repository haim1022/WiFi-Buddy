boolean buddyPing()
{
  char remotePress = 0;

  if (WiFi.status() == WL_CONNECTED)
  {
    Start: String pingDes = buddyUserInput("IP or hostname:", "IP or hostname:"); //ping destination
    remote.enableReceive(D6);
    lcd.clear();
    lcd.print("Pinging ");
    lcd.setCursor(0, 1);
    lcd.print(pingDes);
    if (pingDes == "0") //exit to main menu
      return 0;
    if (Ping.ping(pingDes.c_str(), 4))
    {
      lcd.clear();
      lcd.print(pingDes);
      lcd.print(":");
      lcd.setCursor(0, 1);
      lcd.print(Ping.averageTime());
      lcd.print(" ms");
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
      lcd.clear();
      lcd.print("Unreachable...");
      delay(500);
      goto Start;
    }
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

