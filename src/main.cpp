#include "project.h"
#include "curl_pool.h"
#include <unistd.h>
//#include "socket.h"

int main(int argc, char* argv[])
{
  CURLSession session;
  //session.OpenURL("https://www.yes24.com/Main/default.aspx");

  // curl -v option :
  session.SetVerbose(1);

  // curl -L option..
  session.SetRedirect(1);

  // curl -o option..
  session.SetWriteFile("temp/1.res");

  // curl -H filename option..
  session.SetHeaderFile("data/linux_header");

  // curl -b -c option..
  session.SetCookieFile("data/cookies.txt");

  // curl GET mothod --url option..
  session.OpenURL("https://www.yes24.com");

  // get http return code..
  printf("[%s:%d]\n", session.GetURL().c_str(), session.GetReturnCode());

  // print header..
  printf("%s\n", session.GetHTTPHeader("Content-Type").c_str());
  printf("[HEADER]\n%s", session.GetHeaderString().c_str());

  // print body..
  printf("%s\n", session.GetBody().c_str());

  // debug Cookies..
  session.DebugJARCookies();  // in curl session..
  session.DebugCookies();     // cookie list are parsed from HTTP header

  // get cookies in curl session..
  printf("%s\n", session.GetJARCookie("HTTP_REFERER").c_str());
  printf("%s\n", session.GetCookies().c_str());

  // get cookies in parsed cookie list..
  printf("%s\n", session.GetCookie("HTTP_REFERER").c_str());
  printf("%s\n", session.GetCookies().c_str());

  // new query..
  session.RefreshHTTPHeader(); // reload from header file..

  // set custom header..
  // curl -H option..
  session.SetHTTPHeader("Referer", "http://localhost:8080/index.html");
  // curl POST method --url option..
  session.PostURLEncode("http://localhost:8080/", "a=Hello#!&b=1", 13);

  // print status..
  printf("[%s:%d]\n", session.GetURL().c_str(), session.GetReturnCode());

  pause();
  return 0;
}
