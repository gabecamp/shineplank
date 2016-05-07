String string_split(String parse, String div, int id)
{
  String result = ""; int p_len; int x1; int x2;
  
  p_len = parse.length();
  
  if (p_len < 1)
  {
    return result;
  }
  if (parse.indexOf(div) == -1)
  {
    return result;
  }
  
  
}

int char_count(const char *str, char div)
{
  const char *p = str; int count = 0;
  do
  {
    if (*p == div)
    {
      count++;
    }
  } while (*(p++));

  return count;
}

