String buddyUserInput(String _title, String _webKeyboardTitle) //returns user typed string
{
  char remotePress = 0;
  char input[16] = { 0 };
  byte doneFlag = 0;
  int charNum = 0;
  boolean webKeyF = 0; //web keyboard flag
  boolean apF = 0; //access point flag
  
  remote.enableReceive(D6);
  lcd.clear();
  lcd.print(_title);
  while(!doneFlag)
  {
    lcd.setCursor(0, 1);
    for (int counter = 0; counter <= charNum; counter++) //print typed password
      lcd.print(input[counter]);
    remotePress = buddyRemote();
    while (~(remotePress = buddyRemote())) //wait for user press
    {
      if(WiFi.status() == WL_CONNECTED)
      {
        webKeyboardTitle = _webKeyboardTitle;
        keyBuffer = "***";
        server.handleClient();
        if(keyBuffer != "***")
        {
          keyBuffer.toCharArray(input, 16);
          charNum = keyBuffer.length() - 1; //"charNum"'s numbering starts from zero
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print(keyBuffer);
        }
      }
      yield();
      if (remotePress) //prevent wrong return checks
        break;
    }
    if(input[charNum] == 0) //textbox starts with null, first input change to '0'
      input[charNum] = '0';
    if (remotePress == UP && input[charNum] < 126)
      input[charNum]++;
    else if (remotePress == DOWN && input[charNum] > 31)
      input[charNum]--;
    else if (remotePress == OK) //if "OK" pressed - open options menu
    {
      char remotePress1 = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Up-Delete Ok-");
      lcd.setCursor(0, 1);
      lcd.print("Down-Done  Enter");
      remotePress1 = buddyRemote();
      while (~(remotePress1 = buddyRemote())) //wait for user press
      {
        yield();
        if (remotePress1) //prevent wrong return checks
          break;
      }
      if (remotePress1 == UP) //backspace
      {
        if (charNum == 0)
          input[charNum] = '0';
        else
          charNum--;
      }
      else if (remotePress1 == OK) //enter
      {
        charNum++;
        input[charNum] = '0';
      }
      else if (remotePress1 == DOWN) //done
      {
        doneFlag = 1;
        String inputString = String(input);
        remote.disableReceive();
        /*if(apF == 1)
          WiFi.softAPdisconnect(1);*/
        return inputString;
      }
      lcd.clear();
      lcd.print(_title);
    }
  }
}

void handleKeyboard() //Webapp
{
  File f = SPIFFS.open("/Keyboard.html", "r"); //open file for reading
  if (!f)
  {
    Serial.println("file open failed");
    handleNotFound();
    return;
  }
  if(server.hasArg("Keyboard_text"))
    keyBuffer = server.arg("Keyboard_text");
  String page = f.readString();
  if(webKeyboardTitle != "***")
    page.replace("Type something:", webKeyboardTitle);
  server.send(200, "text/html", page);
  f.close();
}
