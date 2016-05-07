void init_audio()
{
  pinMode(AMP, OUTPUT); digitalWrite(AMP, HIGH); 
}

void update_audio()
{
  if (btc_conn == 1)
  {
    Serial2.print(F("VOLUME")); Serial2.print(F("\r")); Serial2.flush();
  }
}

