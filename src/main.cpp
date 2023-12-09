#include "project.h"
#include "curl_pool.h"
#include <unistd.h>
//#include "socket.h"

int main(int argc, char* argv[])
{
  CURLSession session;
  //session.OpenURL("https://www.yes24.com/Main/default.aspx");
  session.SetDataFile("temp/1.res");
  session.SetHeaderFile("data/linux_header");
  session.SetCookieFile("data/cookies.txt");
  session.OpenURL("https://www.yes24.com/Main/default.aspx");

  pause();
  return 0;
}
