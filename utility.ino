int analog_convert(int inp)
{
  //if (lim == 10)
  //{
    /*
    if (inp == 10) return 1023;
    else if (inp == 0) return 0;
    else if (inp == 9) return 232;
    else if (inp == 8) return 209;
    else if (inp == 7) return 186;
    else if (inp == 6) return 163;
    else if (inp == 5) return 140;
    else if (inp == 4) return 117;
    else if (inp == 3) return 94;
    else if (inp == 2) return 71;
    else if (inp == 1) return 42;
    
  //}
    else return 1023;
    */
    
  if (inp == 10) return 100;
  else if (inp == 0) return 0;
  else if (inp == 9) return 90;
  else if (inp == 8) return 80;
  else if (inp == 7) return 70;
  else if (inp == 6) return 60;
  else if (inp == 5) return 50;
  else if (inp == 4) return 40;
  else if (inp == 3) return 30;
  else if (inp == 2) return 20;
  else if (inp == 1) return 10;
  else return 100;
}

int hsv_convert(int inp)
{
  if (inp == 0) return 0;
  else if (inp == 1) return 36;
  else if (inp == 2) return 72;
  else if (inp == 3) return 108;
  else if (inp == 4) return 144;
  else if (inp == 5) return 180;
  else if (inp == 6) return 216;
  else return 255;
}

