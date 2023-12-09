#include "project.h"
#include "curl_pool.h"
#include <unistd.h>
//#include "socket.h"

int main(int argc, char* argv[])
{
  CURLSession session;
  //session.OpenURL("https://www.yes24.com/Main/default.aspx");
  //session.SetVerbose(1);
  session.SetRedirect(1);
  session.SetWriteFile("temp/1.res");
  session.SetHeaderFile("data/linux_header");
  session.SetCookieFile("data/cookies.txt");
  session.OpenURL("https://www.yes24.com");

  printf("[%s:%d]\n", session.GetURL().c_str(), session.GetReturnCode());

  printf("%s\n", session.GetHTTPHeader("Content-Type").c_str());
  //printf("%s\n", session.GetBody().c_str());

  pause();
  return 0;
}
