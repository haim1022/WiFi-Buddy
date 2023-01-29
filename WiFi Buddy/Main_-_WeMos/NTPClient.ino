boolean buddyNTPClient() //NTP Client to recieve time stamp
{
  unsigned int localPort = 2390; //local port to listen for UDP packets
  const char* ntpServerName = "time.google.com";
  //const char* ntpServerName = "time.nist.gov";
  const int NTP_PACKET_SIZE = 48; //NTP time stamp is in the first 48 bytes of the message
  byte packetBuffer[NTP_PACKET_SIZE];
  
  IPAddress timeServerIP;
  WiFiUDP udp;

  char remotePress = 0;
  if (WiFi.status() == WL_CONNECTED)
  {
    if(usingRemote)
      remote.enableReceive(D6);
    udp.begin(localPort);
    lcd.clear();
    lcd.print("Clock App:");
    if(usingRemote)
      remotePress = buddyRemote();
    while (!remotePress && WiFi.status() == WL_CONNECTED)
    {
      WiFi.hostByName(ntpServerName, timeServerIP); //get a random server from the pool
      Serial.println("sending NTP packet...");
      memset(packetBuffer, 0, NTP_PACKET_SIZE); //reset buffer
                            //Initialize values needed to form NTP request
      packetBuffer[0] = 0b11100011;   // LI, Version, Mode
      packetBuffer[1] = 0;     // Stratum, or type of clock
      packetBuffer[2] = 6;     // Polling Interval
      packetBuffer[3] = 0xEC;  // Peer Clock Precision
                   // 8 bytes of zero for Root Delay & Root Dispersion
      packetBuffer[12] = 49;
      packetBuffer[13] = 0x4E;
      packetBuffer[14] = 49;
      packetBuffer[15] = 52;

      udp.beginPacket(timeServerIP, 123); //NTP requests are to port 123
      udp.write(packetBuffer, NTP_PACKET_SIZE); //request timestamp
      udp.endPacket();
      delay(500); // wait to see if a reply is available

      int cb = udp.parsePacket();
      if (!cb)
        Serial.println("no packet yet");
      else
      {
        Serial.print("packet received, length=");
        Serial.println(cb);
        udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
        unsigned long highWord = word(packetBuffer[40], packetBuffer[41]); //packet size is 4 bytes = 2 words = 1 long
        unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
        unsigned long secsSince1900 = highWord << 16 | lowWord;
        const unsigned long seventyYears = 2208988800UL;
        unsigned long epoch = secsSince1900 - seventyYears; //Unix time
        lcd.setCursor(0, 1);
        if ((((epoch % 86400L) / 3600) + 3) < 10)
          lcd.print("0"); //place '0' before one digit number
        lcd.print(((epoch % 86400L) / 3600) + 3);
        lcd.print(":");
        if (((epoch % 3600) / 60) < 10)
          lcd.print("0"); //place '0' before one digit number
        lcd.print((epoch % 3600) / 60); // print the minute (3600 equals secs per minute)
        lcd.print(":");
        if ((epoch % 60) < 10)
          lcd.print("0"); //place '0' before one digit number
        lcd.print(epoch % 60); // print the second
      }
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
    remote.disableReceive();
    return 1; //error
  }
}
