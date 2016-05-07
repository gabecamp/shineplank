void init_clock()
{
  setSyncProvider(getTeensy3Time);
}

time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}

unsigned long processSyncMessage() 
{
  unsigned long pctime = 0L;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013 

  if (Serial.find(TIME_HEADER)) 
  {
     pctime = Serial.parseInt();
     return pctime;
     if (pctime < DEFAULT_TIME) //Check the value is a valid time (i.e. greater than 1/1/13)
     { 
       pctime = 0L; //Return 0 to indicate that the time is not valid
     }
  }
  return pctime;
}

void clockSet(unsigned long timeCode)
{
  time_t t = timeCode;
  if (t != 0)
  {
    Teensy3Clock.set(t); // Set the RTC
    setTime(t);
    update_clock();
  }
}

void update_clock()
{
  time_t t = Teensy3Clock.get();
  
  hours = hour(); minutes = minute(); seconds = second(); /*days = dow;*/ date = day(); months = month(); years = year();
  
 //UPDATE TIME
  if ((minutes != minutesLast) || (hours != hoursLast))
  {
    String ampm;
    if ((hours >= 0) && (hours <= 11))
    {
      ampm = F("AM");
    }
    else
    {
      ampm = F("PM");
    }
    		
    //UPDATE TIME
    String addZ = F("");
    if (minutes < 10) addZ = F("0");
    
    //MEASURE WIDTH OF NEW TIME TEXT
    pxs.setFont(AvenirNextUltraLight65a); pxs.setColor(255,255,255);
    int timeWidth = pxs.getTextWidth(hrConvert(hours) + F(":") + addZ + minutes); //TIME
    pxs.setFont(AvenirLTStd35Light16a); pxs.setColor(255,255,255);
    int ampmWidth = pxs.getTextWidth(ampm); int totalWidth = timeWidth + ampmWidth; int tBegin = ((320 - totalWidth)/2);
    
    //PRINT NEW TIME TEXT (e.g. XX:XX XM)
    pxs.setColor(0, 0, 0); pxs.fillRectangle(0, 65, 320, 79); //CLEAR CLOCK
    pxs.setBackground(0, 0, 0); pxs.setColor(255, 255, 255); pxs.setFont(AvenirNextUltraLight65a); //TEXT SETUP
    pxs.print(tBegin, 63, hrConvert(hours) + F(":") + addZ + minutes); //PRINT TIME
    pxs.setFont(AvenirLTStd35Light16a); pxs.print(tBegin+timeWidth+(tBegin/6), 71, ampm); //PRINT XM
    
    //UPDATE STATE VARS
    minutesLast = minutes; hoursLast = hours;
  }
  
  if ((months != monthsLast) || (date != dateLast))
  {
    //MEASURE AND PRINT NEW DATE TEXT
    String dateString = monthConvert(months) + F(" ") + date + F(" | 61√É∆í√¢‚Ç¨≈°√É‚Äö√Ç¬∫ | RAIN");
    pxs.setBackground(0, 0, 0); pxs.setColor(255,255,255); pxs.setFont(AvenirLTStd35Light16a);     
    int dateWidth = pxs.getTextWidth(dateString); int dBegin = ((320 - dateWidth)/2);
    pxs.setColor(0, 0, 0); pxs.fillRectangle(0, 145, 320, 29); pxs.setColor(255,255,255); //CLEAR OLD DATE
    pxs.print(dBegin, 150, dateString); //PRINT DATE
    
    //UPDATE STATE VARS
    monthsLast = months; dateLast = date;
  }
}

String hrConvert(int inp)
{
  String result = F("12");

  if (inp == 0) 
  {
    result = F("12");
    return result;
  }
  else if (inp == 1) 
  {
    result = F("1");
    return result;
  }
  else if (inp == 2) 
  {
    result = F("2");
    return result;
  }
  else if (inp == 3) 
  {
    result = F("3");
    return result;
  }
  else if (inp == 4) 
  {
    result = F("4");
    return result;
  }
  else if (inp == 5) 
  {
    result = F("5");
    return result;
  }
  else if (inp == 6) 
  {
    result = F("6");
    return result;
  }
  else if (inp == 7) 
  {
    result = F("7");
    return result;
  }
  else if (inp == 8) 
  {
    result = F("8");
    return result;
  }
  else if (inp == 9) 
  {
    result = F("9");
    return result;
  }
  else if (inp == 10) 
  {
    result = F("10");
    return result;
  }
  else if (inp == 11) 
  {
    result = F("11");
    return result;
  }
  else if (inp == 12) 
  {
    result = F("12");
    return result;
  }
  else if (inp == 13) 
  {
    result = F("1");
    return result;
  }
  else if (inp == 14) 
  {
    result = F("2");
    return result;
  }
  else if (inp == 15) 
  {
    result = F("3");
    return result;
  }
  else if (inp == 16) 
  {
    result = F("4");
    return result;
  }
  else if (inp == 17) 
  {
    result = F("5");
    return result;
  }
  else if (inp == 18) 
  {
    result = F("6");
    return result;
  }
  else if (inp == 19) 
  {
    result = F("7");
    return result;
  }
  else if (inp == 20) 
  {
    result = F("8");
    return result;
  }
  else if (inp == 21) 
  {
    result = F("9");
    return result;
  }
  else if (inp == 22) 
  {
    result = F("10");
    return result;
  }
  else if (inp == 23) 
  {
    result = F("11");
    return result;
  }
  return result;
}

String monthConvert(int inp)
{
  String result;

  if (inp == 1) 
  {
    result = F("JANUARY");
    return result;
  }
  else if (inp == 2) 
  {
    result = F("FEBRUARY");
    return result;
  }
  else if (inp == 3) 
  {
    result = F("MARCH");
    return result;
  }
  else if (inp == 4) 
  {
    result = F("APRIL");
    return result;
  }
  else if (inp == 5) 
  {
    result = F("MAY");
    return result;
  }
  else if (inp == 6) 
  {
    result = F("JUNE");
    return result;
  }
  else if (inp == 7) 
  {
    result = F("JULY");
    return result;
  }
  else if (inp == 8) 
  {
    result = F("AUGUST");
    return result;
  }
  else if (inp == 9) 
  {
    result = F("SEPTEMBER");
    return result;
  }
  else if (inp == 10) 
  {
    result = F("OCTOBER");
    return result;
  }
  else if (inp == 11) 
  {
    result = F("NOVEMBER");
    return result;
  }
  else
  {
    result = F("DECEMBER");
    return result;
  }
  
  return result;
}


