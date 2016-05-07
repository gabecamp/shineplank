void init_eeprom()
{
  for (int i = 1; i < 7; i++)
  {
    char inChar;
    inChar = EEPROM.read(i);
    
    if ((inChar == NULL) || (inChar == '\0'))
    {
      delay(10); EEPROM.write(i, '0'); delay(10);
      char inChar; inChar = '0';
    }
    
    udid[i] = inChar;
    delay(50);
  } 
  /*
  for (int i = 1; i < 7; i++)
  {
    Serial.print("udid["); Serial.print(i); Serial.print("] = "); Serial.println(udid[i]);
  }
  */
}

void read_eeprom(int id)
{
  char inChar;
  inChar = EEPROM.read(id);
  debug(inChar, 1);
}

