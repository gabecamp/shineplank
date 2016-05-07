void init_usr()
{
  if (Serial1.available() > 0)
  {
    while (Serial1.available())
    {
      Serial1.read();
    }
  }
  Serial1.write("+++");
  unsigned long timeout = millis() + (4*1000);
  while (!Serial1.available())
  {
    delay(1);
    if (millis() > timeout) 
    {
      debug("USR AT TIMEOUT", 0); return;
    }
  }
  
  String inUSR = "";
  while (Serial1.available() > 0)
  {
    inUSR += char(Serial1.read());
  }
  
  Serial1.print("a"); delay(100);
  Serial1.print("AT+E"); Serial1.print("\r"); Serial1.flush(); delay(100);
  //debug("USR AT SUCCESS", 0);
  
  if (Serial1.getCommandCount())
  {
    while (Serial1.getCommandCount())
    {
      Serial1.pullCommand(buffer1); memset(buffer1, 0, sizeof(buffer1));
    }
  }
}

void usr_handler(String cmd)
{
  cmd.trim(); 
  if ((cmd == "+ok") || (cmd == "+ok=") || (cmd == "+ERROR"))
  {
    return;
  }
  if (cmd.startsWith("+ok="))
  {
    cmd = cmd.substring(4, cmd.length());
  }

  debug(cmd, 0);

  if ((cmd.indexOf(F("Ch,SSID,BSSID,Security,Indicator")) != -1) && (ssid_count != -1))  //SCANNING FOR SSIDs, RESET SSID STORAGE ARRAY
  {
    for (int i = 0; i < ssid_count; i++)
    {
      for (int j = 0; j < 3; j++)
      {
        ssid_scan[i][j] = "";
      }
    }
    ssid_count = -1; return;
  }
  else if ((cmd.indexOf("WPA2PSK/AES") != -1) || (cmd.indexOf("OPEN/NONE") != -1) || (cmd.indexOf("WPAPSK/TKIPAES") != -1) || (cmd.indexOf("WPAPSKWPA2WPSK/TKIPAES") != -1)) //SSID IN-RANGE LINE
  {
    ssid_count++;
    if (ssid_count > 24)
    {
      ssid_count = 24; return;
    }
    else
    {
      int a0; int a1; int b0; int b1; int c0; int c1; int d0; int d1; int e0; int e1;
      a0 = cmd.indexOf(','); a1 = cmd.indexOf(',', a0+1);
      if ((a0 == -1) || ((a1 - a0) <= 1)) //EITHER NO COMMA DELIMITERS PRESENT OR SSID NAME IS 0 CHARS LONG SO SKIP
      {
        ssid_count--; return;
      }
      b0 = cmd.indexOf(',', a1+1); b1 = cmd.indexOf(',', b0+1); c0 = b1+1; c1 = cmd.length();
      
      //[0] SSID
      ssid_scan[ssid_count][0] = cmd.substring(a0+1, a1);
      
      //[1] AUTHENTICATION TYPE | [2] ENCRYPTION MODE
      String auth_tmp = cmd.substring(b0+1, b1); int auth_len = auth_tmp.length(); int auth_div = auth_tmp.indexOf("/");
      if (auth_div != -1) //FOUND DIV
      {
        ssid_scan[ssid_count][1] = auth_tmp.substring(0, auth_div); //AUTHENTICATION TYPE
        ssid_scan[ssid_count][2] = auth_tmp.substring(auth_div+1, auth_len); //ENCRYPTION MODE
      }
      else //ERROR...MARK AS OPEN FOR NOW
      {
        ssid_scan[ssid_count][1] = "OPEN";
        ssid_scan[ssid_count][2] = "NONE";
      }
      
      //[3] STORE SIGNAL STRENGTH
      ssid_scan[ssid_count][3] = cmd.substring(c0, c1);
      
      //[4] CLEAR PASSWORD ENTRY IF ONE EXISTS
      ssid_scan[ssid_count][4] = "";
      
      //SEND SSID LIST TO CONNECTED CLIENT DEVICE
      if (ble_conn == 1)
      { 
        Serial2.print("SEND SSID:"); Serial2.print(ssid_count); Serial2.print(":");
        Serial2.print(ssid_scan[ssid_count][0]); Serial2.print(":");
        if (ssid_scan[ssid_count][1] == "OPEN")
        {
          Serial2.print('0');
        }
        else
        {
          Serial2.print('1');
        }
        Serial2.print("^"); Serial2.print("\n\r"); Serial2.flush();
        
        delay(90);
        //IF LAST SSID, SEND SCAN COMPLETE INDICATOR TO DEVICE
        if (!Serial1.getCommandCount())
        {
          Serial.println("SSID:_DONE_^"); Serial.flush();
          Serial2.print("SEND SSID:_DONE_^"); Serial2.print("\n\r"); Serial2.flush(); await_response(2);
        }
      }
    }
  }
  else if ((cmd.indexOf("Disconnected") != -1) || (cmd.indexOf("RF Off") != -1))  //NO STA LINK
  {
    wiconn = 0;
    wifi_icon(0);
    if (ble_conn == 1)  //REPORT RESULT TO DEVICE
    {
      Serial2.print("SEND WIC:ERR^"); Serial2.print("\n\r"); Serial2.flush(); return;
    }
  }
  else if (cmd.indexOf(ssid + "(") != -1)   //STA LINK SUCCESSFULLY ESTABLISHED
  {
    wiconn = 1;
    wifi_icon(1);    
    if (ble_conn == 1)  //REPORT RESULT TO DEVICE
    {
      Serial2.print("SEND WIC:OK^"); Serial2.print("\n\r"); Serial2.flush(); return;
    }
  }
}

void wifi_connect(int router_id)
{
  Serial.print("AT+WSSSID="); Serial.println(ssid_scan[router_id][0]);
  Serial.print("AT+WSKEY="); Serial.print(ssid_scan[router_id][1]); Serial.print(","); Serial.print(ssid_scan[router_id][2]); Serial.print(","); Serial.println(ssid_scan[router_id][4]); Serial.flush();
  
  Serial1.print("AT+WSSSID="); Serial1.print(ssid_scan[router_id][0]); Serial1.print("\r"); await_response(1);
  Serial1.print("AT+WSKEY="); Serial1.print(ssid_scan[router_id][1]); Serial1.print(","); Serial1.print(ssid_scan[router_id][2]); Serial1.print(","); Serial1.print(ssid_scan[router_id][4]); Serial1.print("\r"); await_response(1);
  delay(1000); wifi_status();
}

void wifi_status()
{
  Serial.println("AT+WSLK");
  Serial1.print("AT+WSLK"); Serial1.print("\r"); Serial1.flush();
}

void wifi_reset()
{
  Serial.println("AT+Z");
  Serial1.print("AT+Z"); Serial1.print("\r"); Serial1.flush(); 
}
