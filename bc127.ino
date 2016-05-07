void init_bc127()
{
  pinMode(PIO0, OUTPUT); pinMode(PIO1, OUTPUT); pinMode(PIO2, OUTPUT); pinMode(PIO4, OUTPUT); pinMode(PIO5, OUTPUT);
  digitalWrite(PIO0, LOW); digitalWrite(PIO1, LOW); digitalWrite(PIO2, LOW); digitalWrite(PIO4, LOW); digitalWrite(PIO5, LOW);
  Serial2.print("STATUS"); Serial2.print("\r"); Serial2.flush();
  
}

void bc127_handler(String cmd)
{
  cmd.trim(); 
  if ((cmd.startsWith("OK")) || (cmd.startsWith("ERROR")))
  {
    return;
  }
  
  debug(cmd, 0);
  
  if (cmd.indexOf("OPEN_OK BLE") != -1)  //BLE CONNECTED (IOS)
  {
    ble_conn = 1; 
    if (btc_conn == 0)
    {
      ble_icon(1);
      btc_discoverable();
    }
    if (btc_conn == 0)
    {
      ble_icon(1); btc_discoverable();
    }
    if (bt_name.length() >= 2)
    {
      Serial2.print("SEND LN:"); Serial2.print(bt_name); Serial2.print("\r"); Serial2.flush();
    }
    return;
  }
  else if (cmd.indexOf("OPEN_OK SPP") != -1)  //SPP CONNECTED (ANDROID/WP)
  {
    ble_conn = 1;
    if (btc_conn == 0)
    {
      ble_icon(1);
    }
    if (btc_conn == 0)
    {
      ble_icon(1);
    }
    if (bt_name.length() >= 2)
    {
      Serial2.print("SEND LN:"); Serial2.print(bt_name); Serial2.print("\r"); Serial2.flush();
    }
    return;
  }
  else if (cmd.indexOf("CLOSE_OK BLE") != -1) //BLE DISCONNECTED
  {
    ble_conn = 0;
    if (btc_conn == 0)
    {
      ble_icon(0); btc_discoverable();
    }
    ble_advertise(); delay(150); btc_discoverable();
    return;
  }
  else if (cmd.indexOf("CLOSE_OK SPP") != -1) //BLE DISCONNECTED
  {
    ble_conn = 0;
    if (btc_conn == 0)
    {
      ble_icon(0); ble_advertise(); delay(150); btc_discoverable();
    }
    return;
  }
  else if (cmd.indexOf("OPEN_OK A2DP") != -1) //BTC CONNECTED
  {
    btc_conn = 1;
    btc_icon(1);
    return;
  }
  else if (cmd.indexOf("CLOSE_OK A2DP") != -1) //BTC DISCONNECTED
  {
    btc_conn = 0;
    btc_icon(0);
    if (ble_conn == 1)
    {
      ble_icon(1);
    }
    btc_discoverable();
    return;
  }
  
  else if (cmd.indexOf("A2DP=") != -1) //VOLUME UPDATEs
  {
    volume = (cmd.substring(4,cmd.length())).toInt();
    vol_icon(/*volume*/);
    return;
  }
  
  else if (cmd.indexOf("ABS_VOL=") != -1)
  {
    volume = ((cmd.substring(((cmd.indexOf("=")) + 1), cmd.length())).toInt())/(9.08);
    vol_icon(/*volume*/);
    return;
  }
  
  else if (cmd.indexOf("AVRCP_MEDIA ") != -1) //SONG UPDATE
  {
    int a0; int a1;
    a0 = ((cmd.indexOf(":")) + 2);
    if (a0 == 1) return;
    a1 = cmd.length();
    
    if (cmd.indexOf("TITLE: ") != -1)
    {
      song = cmd.substring(a0, a1);
    }
    else if (cmd.indexOf("ARTIST: ") != -1)
    {
      artist = cmd.substring(a0, a1);
    }
    else if (cmd.indexOf("PLAYING_TIME(MS): ") != -1) //PLAYING NEW SONG, PRINT STREAM
    {
      print_track();
    }
  }
  else if (cmd.indexOf("AVRCP_PLAY") != -1) //PLAYING SONG, PRINT STREAM
  {
    print_track();
  }
  else if (cmd.indexOf("AVRCP_PAUSE") != -1) //PAUSED SONG, CLEAR STREAM
  {
    clear_stream(); 
  }
  
  //CUSTOM COMMANDS
  else if (cmd.startsWith("RECV "))  //COMMAND FROM BLUETOOTH DEVICE (BLE OR SPP)
  {
    if (cmd.indexOf("INIT") != -1) //INIT COMMAND SENDS GLOWDECK HARDWARE STATE TO CONNECTED DEVICE
    {
      Serial2.print("SEND "); 
      Serial2.print("IN:"); Serial2.print(udid[1]); Serial2.print("^");
      Serial2.print("ID:"); Serial2.print(udid[2]); Serial2.print(udid[3]); Serial2.print(udid[4]); Serial2.print(udid[5]); Serial2.print(udid[6]); Serial2.print("^");
      Serial2.print("CL:"); Serial2.print(coils); Serial2.print("^");
      Serial2.print("CS:"); Serial2.print(smart); Serial2.print("^");
      Serial2.print("LT:"); Serial2.print(led_sw); Serial2.print("^");
      Serial2.print("LC:"); Serial2.print("0"); Serial2.print(":"); Serial2.print("0"); Serial2.print(":"); Serial2.print("0"); Serial2.print("^");
      Serial2.print("MS:"); Serial2.print(msync); Serial2.print("^");
      Serial2.print("DB:"); Serial2.print(bright); Serial2.print("^");
      Serial2.print("DA:"); Serial2.print(autobright); Serial2.print("^");
      Serial2.print("WF:"); Serial2.print(wifi); Serial2.print("^");
      Serial2.print("WP:"); Serial2.print(code); Serial2.print("^");
      Serial2.print("FP:"); Serial2.print(fpress); Serial2.print("^");
      Serial2.print("FH:"); Serial2.print(fhold); Serial2.print("^");
      Serial2.print("LP:"); Serial2.print(lpress); Serial2.print("^");
      Serial2.print("LH:"); Serial2.print(lhold); Serial2.print("^");
      Serial2.print("RP:"); Serial2.print(rpress); Serial2.print("^");
      Serial2.print("RH:"); Serial2.print(rhold); Serial2.print("^");
      Serial2.print("ST:"); Serial2.print(streams); Serial2.print("^");
      Serial2.print("PV:"); Serial2.print(privacy); Serial2.print("^");
      Serial2.print("SM:"); Serial2.print(mstream); Serial2.print("^");
      Serial2.print("SX:"); Serial2.print(scrolls); Serial2.print("^");
      Serial2.print("NM:"); Serial2.print(night); Serial2.print("^");
      Serial2.print("\n\r"); Serial2.flush(); return;
    }
    else if (cmd.indexOf("TI:") != -1)                      //SET GLOWDECK TIME
    {
      String val = cmd.substring(12, cmd.indexOf("^"));
      unsigned long ble_time = ((val.toInt()) - 18000); //EST Conversion -14400; PST Conversion -25200;
      clockSet(ble_time); return;
    }
    else if ((cmd.indexOf("2015-") != -1) || (cmd.indexOf("2016-") != -1))
    {
      Serial.println("Received Android Time..."); return;
    }
    else if (cmd.indexOf("DL:") != -1) //DISPLAY BRIGHTNESS: SLIDER
    {
      int pwm_val = (cmd.substring(12, cmd.indexOf("^"))).toInt();
      bright = analog_convert(pwm_val); analogWrite(LCD_BL, bright); return;
    }
    else if (cmd.indexOf("PB:") != -1) //WIRELESS CHARGER: TOGGLE ON/OFF
    {
      int coils_val = (cmd.substring(12, cmd.indexOf("^"))).toInt();
      if (coils_val == 0)
      {
        digitalWrite(COIL_PW, LOW);
        coils = 0; return;
      }
      else 
      {
        digitalWrite(COIL_PW, HIGH);
        coils = 0; return;
      }
      return;
    }
    else if (cmd.indexOf("CV:") != -1) //LIGHT PAD CONTROLLER
    {
      int hsv_hue = hsv_convert((cmd.substring(12, 13)).toInt());
      int hsv_bright = hsv_convert((cmd.substring(13, cmd.indexOf("^"))).toInt());
      FastLED.showColor(CHSV(hsv_hue, 255, hsv_bright)); return;
    }
    else if (cmd.indexOf("UV:") != -1)  //LIGHTS OFF
    {
      FastLED.showColor(CHSV(0, 0, 0)); return;
    }
    else if (cmd.indexOf("RSD^") != -1)  //SCANS FOR SSIDS
    {
      Serial1.print("AT+WSCAN"); Serial1.print("\r"); Serial1.flush(); return;
    }
    else if (cmd.indexOf("WIC:") != -1)              //RECEIVED SSID:PASSWORD
    {
      //STORE SSID/CODE
      int a0; int a1; a0 = cmd.indexOf(':'); a1 = cmd.indexOf(':', a0+1);
      int ssid_num = ((cmd.substring(a0+1, a1)).toInt());
      ssid = ssid_scan[ssid_num][0]; code = cmd.substring(a1+1, cmd.length()); ssid_scan[ssid_num][4] = code;
      Serial.print("Wifi Source: "); Serial.println(ssid); 
      Serial.print("Wifi Password: "); Serial.println(code);
      
      //ATTEMPT TO CONNECT
      wifi_connect(ssid_num);  //USR_HANDLER WILL RESPOND WITH RESULT WHEN WIFI_STATUS PRODUCES RESULT
      return;
    }
    else if (cmd.indexOf("UID:") != -1)               //STORE STREAMS USER ID
    {
      streams_id = cmd.substring(12, cmd.indexOf("^"));
      Serial2.print("SEND UID:OK^"); Serial2.print("\n\r"); Serial2.flush();
      return;
    }
    else if (cmd.indexOf("TK:") != -1)
    {
      streams_tk = cmd.substring(12, cmd.indexOf("^"));
      Serial2.print("SEND TK:OK^"); Serial2.print("\n\r"); Serial2.flush();
    }
    else if (cmd.indexOf("LN:") != -1)
    {
      bt_name = cmd.substring(12, cmd.indexOf("^"));
      Serial2.print("SET NAME="); Serial2.print(bt_name); Serial2.print("\r"); Serial2.flush(); delay(100);
      Serial2.print("WRITE"); Serial2.print("\r"); Serial2.flush(); delay(100);
      Serial2.print("RESET"); Serial2.print("\r"); Serial2.flush(); delay(100);
    }
    else if (cmd.indexOf("```") != -1)                //ENTER DFU MODE
    {
      debug("ENTER DFU MODE", 1);
      Serial1.print("AT+Z"); Serial1.print("\r"); Serial1.flush(); delay(50);
      Serial2.print("RESET"); Serial2.print("\r"); Serial2.flush(); delay(50);
      dfu_mode(); return;
    }
    else if (cmd.indexOf("^|^") != -1)                //RESET TO PLSCO FACTORY SETTINGS
    {
      debug("FACTORY RESET", 1);
      //Serial1.print("AT+Z"); Serial1.print("\r"); Serial1.flush(); delay(50);
      //Serial2.print("RESET"); Serial2.print("\r"); Serial2.flush(); delay(50);
      /*dfu_mode();*/ return;
    }
    else                                              //NOT CUSTOM-HANDLED COMMAND
    {
      int len = cmd.length(); String val = cmd.substring(9, len);
      if ((val.substring(0,3) == "AT+") || (val.substring(0,3) == "AP+"))
      {
        Serial1.print(cmd); Serial1.print("\r"); Serial1.flush(); return;
      }
      else
      {
        if (val.indexOf("^") == -1)
        {
          Serial2.print(cmd); Serial2.print("\r"); Serial2.flush(); return;
        }
      }
    } 
  }
}

void play_tone(int tone_id)
{
  if (tone_id == 1)
  {
    Serial2.print(F("TONE V 128 TI 0 N C4 L 8 N D4 L 8 N E4 L 8 N F4 L 8 N G4 L 8 N A4 L 8 N B4 L 8 N C5 L 8")); Serial2.print("\r"); Serial2.flush();
  }
  else if (tone_id == 2)
  {
    Serial2.print(F("TONE TE 400 V 64 TI 0 N C5 L 8 N R0 L 32 N E5 L 8 N R0 L 32 N G5 L 8 N R0 L 32 N B5 L 4 N R0 L 1 N C6 L 2 TN C6 L 8")); Serial2.print("\r"); Serial2.flush();
  }
  else if (tone_id == 3)
  {
    Serial2.print(F("TONE V 64 N C6 L 4 V 128 N C6 L 4 V 255 N C6 L 4 V 128 N C6 L 4 V 64 N C6 L 4")); Serial2.print("\r"); Serial2.flush();
  }
}

void ble_advertise()
{
  Serial2.print("ADVERTISING ON"); Serial2.print("\r"); Serial2.flush(); 
}

void btc_discoverable()
{
  Serial2.print("DISCOVERABLE ON"); Serial2.print("\r"); Serial2.flush(); 
}

void bc127_factory_reset()
{
  digitalWrite(PIO0, HIGH); Serial2.print("RESET"); delay(10); Serial2.print("\r"); Serial2.flush(); delay(500); digitalWrite(PIO0, LOW); 
}

void gpio_press(int pio_id, int hold = 99)
{
  digitalWrite(pio_id, HIGH); delay(hold); digitalWrite(pio_id, LOW);
}

