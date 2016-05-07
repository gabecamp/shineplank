void init_serial()
{  
  Serial.begin(9600); delay(100); 
  Serial1.begin(9600); delay(100);
  Serial2.begin(9600); delay(100);
}

void serial_stream()
{
  if (Serial.available() > 0)
  {
    String process = "";
    while (Serial.available() > 0)
    {
      char inChar = char(Serial.read());
      process += inChar;
    }
    serial_handler(process);
    process = "";
  }
    
  if (Serial1.getCommandCount())
  {
    memset(buffer1, 0, sizeof(buffer1));
    while (Serial1.getCommandCount())
    {
      Serial1.pullCommand(buffer1);
      String process = (char*)buffer1;
      memset(buffer1, 0, sizeof(buffer1));
      usr_handler(process);
      process = "";
    }
    if (Serial1.checkListOverflow()) Serial1.resetCommandList();
  }
  
  if (Serial2.getCommandCount())
  {  
    memset(buffer2, 0, sizeof(buffer2));
    while (Serial2.getCommandCount())
    {
      Serial2.pullCommand(buffer2);
      String process = (char*)buffer2;
      memset(buffer2, 0, sizeof(buffer2));
      bc127_handler(process);
      process = "";
    }
    if (Serial2.checkListOverflow()) Serial2.resetCommandList();
  }
  
  if (Serial1.checkListOverflow()) Serial1.resetCommandList();
  if (Serial2.checkListOverflow()) Serial2.resetCommandList();
}

void serial_handler(String cmd)
{
  if (cmd.indexOf("```") != -1)
  {
    debug("ENTER DFU MODE", 1); dfu_mode(); return;
  }
  else
  {
    if (cmd.indexOf("AT+") != -1)
    {
      Serial1.print(cmd); Serial1.flush(); return;
    }
    else
    {
      Serial2.print(cmd); Serial2.flush(); return;
    }
  } 
}

void debug(String msg, int bt_echo)
{
  Serial.println(msg);
  
  if ((ble_conn == 1) && (bt_echo == 1))
  {
    Serial2.print("SEND "); Serial2.print(msg); Serial2.print("\n\r"); Serial2.flush();

    if (Serial2.getCommandCount())
    {    
      Serial2.pullCommand(buffer2); memset(buffer2, 0, sizeof(buffer2));
    }
  }
}

void await_response(int serial_id)  //AWAITS RESPONSE FROM SERIAL PORT AND THEN CLEARS IT FROM BUFFER
{
  delay(100);
  
  if (serial_id == 1)
  {
    if (Serial1.getCommandCount())
    {
      while (Serial1.getCommandCount())
      {
        Serial1.pullCommand(buffer1); memset(buffer1, 0, sizeof(buffer1));
      }
    }
    else
    {
      delay(500);
      while (Serial1.getCommandCount())
      {
        Serial1.pullCommand(buffer1); memset(buffer1, 0, sizeof(buffer1));
      }      
    }
    return;
  }
  else
  {
    if (Serial2.getCommandCount())
    {
      while (Serial2.getCommandCount())
      {
        Serial2.pullCommand(buffer2); memset(buffer2, 0, sizeof(buffer2));
      }
    }
    else
    {
      delay(500);
      while (Serial2.getCommandCount())
      {
        Serial2.pullCommand(buffer2); memset(buffer2, 0, sizeof(buffer2));
      }      
    }
    return;
  }
}


