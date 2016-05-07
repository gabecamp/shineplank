void init_lcd()
{
  pxs.setSpiPins(LCD_SCLK, LCD_MOSI, LCD_CS, LCD_RSET, LCD_DC);
  pxs.init(); pxs.setOrientation(LANDSCAPE_FLIP); pinMode(LCD_BL, OUTPUT); 
}

void lcd_splash()
{
  /*---------------------
   INITIALIZE LCD
   ---------------------*/
  pxs.setBackground(0,0,0);
  pxs.setColor(255,255,255);
  pxs.clear();
  
  /*---------------------
   CREATE GLOWDECK GUI
   ---------------------*/
  //DRAW HEADER_ICON_DIVS
  pxs.setColor(130,130,130);
  pxs.drawLine(64,12,64,28);
  pxs.drawLine(128,12,128,28);
  pxs.drawLine(192,12,192,28);
  pxs.drawLine(256,12,256,28);
  
  //DRAW STREAMS_DIV
  pxs.drawLine(12,198,308,198);
  
  //DRAW HEADER_ICONS
  pxs.setFont(glowdeck_icons20a); pxs.setColor(255,255,255);
  
  pxs.print(13, 4, "a"); //DRAW HEADER_ICON_1 (BATTERY)
  pxs.setColor(0,0,0); pxs.fillRectangle(12,28,30,6); pxs.setColor(255,255,255);
  
  /*
   pxs.print(87, 4, "r"); //DRAW HEADER_ICON_2 (BLUETOOTH)
   pxs.setColor(0,0,0); pxs.fillRectangle(87,30,30,6); pxs.setColor(255,255,255);
   */
  
  /*
   pxs.print(148, 4, "v"); //DRAW HEADER_ICON_3 (WIFI)
   pxs.setColor(0,0,0); pxs.fillRectangle(148,28,30,6); pxs.setColor(255,255,255);
   */
  
  /*
   pxs.print(217, 4, "x"); //DRAW HEADER_ICON_4 (LIGHTS)
   pxs.setColor(0,0,0); pxs.fillRectangle(217,28,30,6); pxs.setColor(255,255,255);
   */
  

  
  //PRINT CLOCK
  //pxs.setFont(AvenirNextLTPro_UltLt63a); pxs.setColor(255,255,255);
  //pxs.print(49, 70, "11:35"); //TIME
  //pxs.setFont(AvenirNextLTPro_UltLt15a); pxs.setColor(255,255,255);
  //pxs.print(265, 77, "PM"); //XM
  
  //PRINT DATEWEATHER
  //pxs.setFont(AvenirNextLTPro_UltLt15a); pxs.setColor(255,255,255);
  //pxs.print(61, 148, "JULY 24 | 89√Ç¬º | SUNNY");
  
  //DRAW PRIVACY_MODE_ICON
  /*
   pxs.setFont(glowdeck_icons20a); pxs.setColor(255,255,255);
   pxs.print(298, 178, "g");
   */
   update_clock();
  
  //PRINT STREAMS
  //------------>
  //TWITTER STREAM
  
  //pxs.drawBitmap(11, 167, 27, 32, img_twtr); //STREAM_A
  
  pxs.setFont(OpenSansLight14ba); pxs.setColor(255,255,255); //STREAM_B
  pxs.print(12, 210, "Setup Glowdeck");
  int div = 12+pxs.getTextWidth("Setup Glowdeck")+9;
  
  pxs.setColor(125,125,125); pxs.drawLine(div, 209, div, 228); //STREAM_AB_DIV
  pxs.setColor(255,255,255);
		
  pxs.setFont(OpenSansLight14a);  //STREAM_C
  pxs.print(div+9, 210, "Launch Streams");
  
  bright = 100;
  digitalWrite(LCD_BL, bright);
}

void print_track()
{
  clear_stream();
  pxs.setFont(OpenSansLight14ba); pxs.setColor(255,255,255); pxs.print(12, 210, song); int div = 12+pxs.getTextWidth(song)+9; //SONG
  pxs.setColor(125,125,125); pxs.drawLine(div, 209, div, 228); pxs.setColor(255,255,255); //DIV
  pxs.setFont(OpenSansLight14a); pxs.print(div+9, 210, artist);  //ARTIST
}

void clear_stream()
{
  pxs.setColor(0,0,0); pxs.fillRectangle(0,170,320,70); pxs.setColor(255,255,255); //CLEAR STREAM
}

void ble_icon(int on)
{
  if (btc_conn == 1) return;
  
  pxs.setFont(glowdeck_icons20a); 
  if (on == 1)
  {
    pxs.setColor(255,255,255);
  }
  else
  {
    pxs.setColor(0,0,0);
  }
  pxs.print(85, 4, "s"); pxs.setColor(0,0,0); pxs.fillRectangle(85,30,30,6); pxs.setColor(255,255,255);
}

void btc_icon(int on)
{  
  pxs.setFont(glowdeck_icons20a); 
  if (on == 1)
  {
    if (ble_conn == 1)
    {
      pxs.setColor(0,0,0); pxs.print(85, 4, "s"); pxs.fillRectangle(85,30,30,6);
    }
    pxs.setColor(255,255,255);
  }
  else
  {
    if (ble_conn == 1)
    {
      pxs.setColor(0,0,0); pxs.print(87, 4, "r"); pxs.fillRectangle(85,30,30,6);
      pxs.setColor(255,255,255); pxs.print(85, 4, "s"); pxs.setColor(0,0,0); pxs.fillRectangle(85,30,30,6); pxs.setColor(255,255,255);
      return;
    }
    pxs.setColor(0,0,0);
  }
  pxs.print(87, 4, "r"); pxs.setColor(0,0,0); pxs.fillRectangle(87,30,30,6); pxs.setColor(255,255,255); 
}

void wifi_icon(int on)
{
  pxs.setFont(glowdeck_icons20a); 
  if (on == 1)
  {
    pxs.setColor(255,255,255);
  }
  else
  {
    pxs.setColor(0,0,0);
  }
  pxs.print(148, 4, "v"); pxs.setColor(0,0,0); pxs.fillRectangle(148,28,30,6); pxs.setColor(255,255,255);
}

void leds_icon(int on)
{
  pxs.setFont(glowdeck_icons20a); 
  if (on == 1)
  {
    pxs.setColor(255,255,255);
  }
  else
  {
    pxs.setColor(0,0,0);
  }
   pxs.print(217, 4, "z"); pxs.setColor(0,0,0); pxs.fillRectangle(217,28,30,6); pxs.setColor(255,255,255); 
}

void vol_icon()
{
  pxs.setFont(glowdeck_icons20a); pxs.setColor(255,255,255);
  if (level == 0)
  {
    pxs.print(280, 4, "b");
  }
  else if ((level > 0) && (level <= 4))
  {
    pxs.print(280, 4, "f");
  }
  else if ((level > 4) && (level <= 10))
  {
    pxs.print(280, 4, "e");
  }
  else
  {
    pxs.print(280, 4, "d");
  }
  
  //DRAW HEADER_ICON_5 (VOLUME)
  pxs.setColor(0,0,0); pxs.fillRectangle(280,28,30,6); pxs.setColor(255,255,255); 
}

void lcd_fade(int fadeTo)
{
  if (fadeTo < bright)
  {
    for (int i = bright; i >= fadeTo; i--)
    {
      digitalWrite(LCD_BL, i); delay(350);
    }
  }
  else
  {
    for (int i = bright; i <= fadeTo; i++)
    {
      digitalWrite(LCD_BL, i); delay(350);
    }
  }

  bright = fadeTo;
}

