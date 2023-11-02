#include "project.h"
#include "curlget.h"
//#include "socket.h"

#define SOCKET_IDX_MAIN     0
#define SOCKET_IDX_SESSION  1
#define SOCKET_IDX_EVENT    2

int main(int argc, char* argv[])
{
  CTHttpGet main_session;
  main_session.OpenURL("www.naver.com");
  return 0;
}
