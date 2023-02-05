String buddyDisplayMenu(byte _bufferSize, String _lcdBuffer[32]) //displays menu and allow user control, returns chosen row, buffer size should include title
{
  byte doneFlag = 0;
  char remotePress = 0;
  int lastCursor = 0, rowCursor = 1;
  if(usingRemote)
    remote.enableReceive(D6);
  while (!doneFlag)
  {
    if (rowCursor != lastCursor) //dont change if selection at top/bottom
    {
      lcd.clear();
      if (rowCursor - 1 != 0) // no space on the left side if this is the title
        lcd.setCursor(1, 0);
      if(_lcdBuffer[0] == "Choose a WiFi:" && _lcdBuffer[rowCursor - 1].charAt(_lcdBuffer[rowCursor - 1].length() - 1) == ' ') //check last char of string
        lcd.write(6); //locked
      lcd.print(_lcdBuffer[rowCursor - 1]); //print first row
      lcd.setCursor(0, 1);
      if(_lcdBuffer[rowCursor] == "Go Back")
        lcd.write(127); //mark with left arrow
      else
        lcd.write(126); //mark current row
      if(_lcdBuffer[0] == "Choose a WiFi:" && _lcdBuffer[rowCursor].charAt(_lcdBuffer[rowCursor].length() - 1) == ' ') //check last char of string
        lcd.write(6); //locked
      lcd.print(_lcdBuffer[rowCursor]); //print second row
      Serial.println(_lcdBuffer[rowCursor - 1]);
      Serial.println(">" + _lcdBuffer[rowCursor]);
      lastCursor = rowCursor;
    }
    if(usingRemote)
      remotePress = buddyRemote();
    while (!remotePress) //wait for user press
    {
      yield();
      if (remotePress) //prevent wrong return checks
        break;
    }

    if (remotePress == UP && rowCursor > 1)
      rowCursor--; //going up means smaller row number
    else if (remotePress == DOWN && rowCursor < _bufferSize - 1)
      rowCursor++; //going down means bigger row number
    else if (remotePress == OK && rowCursor != 0)
    {
      doneFlag = 1;
      if(usingRemote)
        remote.disableReceive();
      return _lcdBuffer[rowCursor];
    }
  }
}
