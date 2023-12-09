#include "project.h"
#include "curl_pool.h"
#include <unistd.h>
//#include "socket.h"

#define SOCKET_IDX_MAIN     0
#define SOCKET_IDX_SESSION  1
#define SOCKET_IDX_EVENT    2

int main(int argc, char* argv[])
{
  CURLSession session;
  //session.OpenURL("https://www.yes24.com/Main/default.aspx");
  session.SetHeaderFile("data/linux_header");
  session.SetCookieFile("data/cookies.txt");
  session.OpenURL("https://www.yes24.com/Main/default.aspx");

  RETDATA* pdata = session.GetData();
  if (pdata)
  {
    //cout << pdata->response << endl;
  }

  pause();
  return 0;
}
