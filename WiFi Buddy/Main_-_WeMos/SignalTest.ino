boolean buddySignalTest() //test current WiFi signal
{
  char remotePress = 0;
  float signalStr = 0;
  float curPos = 0; //current cursor position in 0 - 80
  boolean modeF = 0; //display mode flag
  
  remote.enableReceive(D6);
  if (WiFi.status() == WL_CONNECTED)
  {
    lcd.clear();
    lcd.print("Signal strength:");
    remotePress = buddyRemote();
    while ((remotePress = buddyRemote()) != OK && WiFi.status() == WL_CONNECTED)
    {
      signalStr = WiFi.RSSI();
      if (!modeF)
      {
        lcd.setCursor(0, 1);
        lcd.print((int)signalStr);
        lcd.print(" db");
        curPos = 0;
      }
      if (modeF)
      {
        while (curPos != map(signalStr, -120, 0, 0, 80))
        {
          lcd.setCursor(curPos / 5, 1);
          lcd.write(map(((int)((curPos / 5) * 10) % 10), 0, 8, 0, 5));
          if (curPos < map(signalStr, -120, 0, 0, 80))
            curPos++;
          if (curPos > map(signalStr, -120, 0, 0, 80))
            curPos--;
        }
      }
      yield();
      if (remotePress == OK) //prevent wrong return checks
        break;
      else if (remotePress == UP || remotePress == DOWN)
      {
        modeF = ~modeF;
        lcd.setCursor(0, 1);
        lcd.print("                ");
      }
    }
    if (WiFi.status() != WL_CONNECTED)
      goto error;
    remote.disableReceive();
    return 0; //success
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
