void init_press()
{
  pinMode(PRESS_F, INPUT_PULLUP); pinMode(PRESS_L, INPUT_PULLUP); pinMode(PRESS_R, INPUT_PULLUP);
  digitalWrite(PRESS_F, HIGH); digitalWrite(PRESS_L, HIGH); digitalWrite(PRESS_R, HIGH); 
}

void ISR_F()
{
  if ((long)(micros() - last_micros_f) >= (150*1000)) 
  {
    if (digitalRead(PRESS_F) == LOW) //STILL PRESSED
    {
      long downTime; long downStart = millis();
      while (1)
      {
        downTime = (millis() - downStart);
        if (digitalRead(PRESS_F) == HIGH) //UNPRESSED
        {
          if (downTime >= 1125) //POST-HOLD THRESHOLD (HELD)
          {
            debug("FRONT HOLD", 0);
            if (btc_conn == 1)
            {
              Serial2.print("TOGGLE_VR"); Serial2.print("\r"); Serial2.flush();
            }
            last_micros_f = micros(); return;
          }
          else                 //PRE-HOLD THRESHOLD (PRESSED)
          {
            debug("FRONT PRESS", 0);
            if (btc_conn == 1)
            {
              Serial2.print("MUSIC PAUSE"); Serial2.print("\r"); Serial2.flush();
            }
            last_micros_f = micros(); return;            
          }
        }
        if (downTime >= 1125) //REACHED TIME LIMIT
        {
          debug("FRONT HOLD", 0);
          if (btc_conn == 1)
          {
            Serial2.print("TOGGLE_VR"); Serial2.print("\r"); Serial2.flush();
          }
          last_micros_f = micros(); return;
        }            
      }
    }
    else                      //NOT STILL PRESSED
    {
      debug("FRONT PRESS", 0);
      if (btc_conn == 1)
      {
        Serial2.print("MUSIC PAUSE"); Serial2.print("\r"); Serial2.flush();
      }
      last_micros_f = micros(); return;             
    }
  }
}

void ISR_L()
{
  if ((long)(micros() - last_micros_l) >= (150*1000)) 
  {
    if (digitalRead(PRESS_L) == LOW) //STILL PRESSED
    {
      long downTime; long downStart = millis();
      while (1)
      {
        downTime = (millis() - downStart);
        if (digitalRead(PRESS_L) == HIGH) //UNPRESSED
        {
          if (downTime >= 1125) //AFTER HOLD THRESHOLD (HELD)
          {
            debug("LEFT HOLD", 0);
            if (btc_conn == 1)
            {
              Serial2.print("MUSIC BACKWARD"); Serial2.print("\r"); Serial2.flush();
            }
            last_micros_l = micros(); return;
          }
          else                 //BEFORE HOLD THRESHOLD (PRESSED)
          {
            debug("LEFT PRESS", 0);
            if (btc_conn == 1)
            {
              Serial2.print("VOLUME DOWN"); Serial2.print("\r"); Serial2.flush();
            }
            last_micros_l = micros(); return;            
          }
        }
        if (downTime >= 1125) //REACHED TIME LIMIT
        {
          debug("LEFT HOLD", 0);
          if (btc_conn == 1)
          {
            Serial2.print("MUSIC BACKWARD"); Serial2.print("\r"); Serial2.flush();
          }
          last_micros_l = micros(); return;
        }            
      }
    }
    else                      //NOT STILL PRESSED
    {
      debug("LEFT PRESS", 0);
      if (btc_conn == 1)
      {
        Serial2.print("VOLUME DOWN"); Serial2.print("\r"); Serial2.flush();
      }
      last_micros_l = micros(); return;             
    }
  }
}

void ISR_R()
{
  if ((long)(micros() - last_micros_r) >= (150*1000)) 
  {
    if (digitalRead(PRESS_R) == LOW) //STILL PRESSED
    {
      long downTime; long downStart = millis();
      while (1)
      {
        downTime = (millis() - downStart);
        if (digitalRead(PRESS_R) == HIGH) //UNPRESSED
        {
          if (downTime >= 1125) //POST-HOLD THRESHOLD (HELD)
          {
            debug("RIGHT HOLD", 0);
            if (btc_conn == 1)
            {
              Serial2.print("MUSIC FORWARD"); Serial2.print("\r"); Serial2.flush();
            }
            last_micros_r = micros(); return;
          }
          else                 //PRE-HOLD THRESHOLD (PRESSED)
          {
            debug("RIGHT PRESS", 0);
            if (btc_conn == 1)
            {
              Serial2.print("VOLUME UP"); Serial2.print("\r"); Serial2.flush();
            }
            last_micros_r = micros(); return;            
          }
        }
        if (downTime >= 1125) //REACHED TIME LIMIT
        {
          debug("RIGHT HOLD", 0);
          if (btc_conn == 1)
          {
            Serial2.print("MUSIC FORWARD"); Serial2.print("\r"); Serial2.flush();
          }
          last_micros_r = micros(); return;
        }            
      }
    }
    else                      //NOT STILL PRESSED
    {
      debug("RIGHT PRESS", 0);
      if (btc_conn == 1)
      {
        Serial2.print("VOLUME UP"); Serial2.print("\r"); Serial2.flush();
      }
      last_micros_r = micros(); return;             
    }
  }
}


