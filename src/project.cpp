#include "project.h"

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}

std::vector<std::string> splitstring(std::string line, char ch)
{
  std::vector<std::string> arrs;
  if (line.empty())
    return arrs;
  std::istringstream ss(line);
  std::string buff;
  while (getline(ss, buff, ch))
  {
    arrs.push_back(trim(buff));
  }
  return arrs;
}

bool file_exist (const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

std::string strformat(const char* format, ...)
{
  std::string ret;
  va_list arg_ptr;
  va_start(arg_ptr, format);
  int bytes = vsnprintf(0 , 0, format, arg_ptr);
  ret.resize(bytes + 1);
  va_start(arg_ptr, format);
  vsnprintf((char*) ret.c_str(), bytes + 1, format, arg_ptr);
  va_end(arg_ptr);
  return ret.c_str();
}

std::string getcurtime()
{
  std::string ret;
  struct tm* t = NULL;
  time_t now;
  time(&now);
  t = localtime(&now);
  ret = strformat("%04d-%02d-%02d %02d:%02d:%02d",
                      t->tm_year + 1900,
                      t->tm_mon + 1,
                      t->tm_mday,
                      t->tm_hour,
                      t->tm_min,
                      t->tm_sec);
  return ret;
}

std::string getseconds()
{
  std::string ret;
  time_t now;
  time(&now);

  ret = strformat("%u", now);
  return ret;
}
