void dfu_mode()
{
  EEPROM.write(0, 13); //Flag BYTE 1 with 13 (i.e. the magic bootloader entry number)
  #define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
  #define CPU_RESTART_VAL 0x5FA0004
  #define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);
  CPU_RESTART;
}

void glowdeck_factory_reset()
{
  //RESET EEPROM
  Serial.println(F("Restoring to Glowdeck initial factory configuration..."));
  EEPROM.write(1, 1); delay(200);
  EEPROM.write(2, 0); delay(200);
  EEPROM.write(3, 0); delay(200);
  EEPROM.write(4, 0); delay(200);
  EEPROM.write(5, 0); delay(200);
  EEPROM.write(6, 0); delay(200);
  
  //RESET BC127
  Serial2.print("RESTORE"); delay(20); Serial2.print("\r"); Serial2.flush(); delay(10);
  Serial2.print("WRITE"); delay(20); Serial2.print("\r"); Serial2.flush(); delay(10);
  Serial2.print("RESET"); delay(20); Serial2.print("\r"); Serial2.flush(); delay(10);
  
  //clearStream();
  pxs.setFont(OpenSansLight14ba); pxs.setColor(255,255,255); //STREAM_B
  pxs.print(12, 210, "Factory Reset");
  int div = 12 + pxs.getTextWidth("Factory Reset") + 9;
  
  pxs.setColor(125,125,125); pxs.drawLine(div, 209, div, 228); //STREAM_AB_DIV
  pxs.setColor(255,255,255);
  
  pxs.setFont(OpenSansLight14a);  //STREAM_C
  pxs.print(div+9, 210, "Restart Now");
  
  Serial.println(F("Success. Power Cycle Glowdeck.")); Serial.flush();
  
  /*
   #define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
   #define CPU_RESTART_VAL 0x5FA0004
   #define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);
   CPU_RESTART;
   */
}

