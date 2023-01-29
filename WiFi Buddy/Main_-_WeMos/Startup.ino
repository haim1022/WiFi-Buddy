void buddyStartup() //displays logo, configures loading animation frames
{
  lcd.clear();
  lcd.print("   WiFiBuddy    ");
  delay(500);
  for (int counter = 0; counter < 6; counter++) //run through array rows
  {
    for (int counter1 = 0; counter1 < 8; counter1++) //run through array cols
      loadAnim[counter][counter1] = loadPixel; //set the BMP char array
    loadPixel = loadPixel >> 1;
    loadPixel = loadPixel | 0b10000;
  }
  for (int counter = 0; counter < 6; counter++)
    lcd.createChar(counter, loadAnim[counter]);
  byte lockChar[8] = { 0b00000, 0b01110, 0b10001, 0b10001, 0b11111, 0b11011, 0b11111, 0b00000 };
  lcd.createChar(6, lockChar);
  for (int counter = 16; counter>0; counter--)
  {
    lcd.setCursor(counter, 0);
    lcd.print(" ");
    delay(50);
  }
}
