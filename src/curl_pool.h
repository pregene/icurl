#if !defined(__CURL_POOL__)
#define __CURL_POOL__

#include "project.h"
#include <curl/curl.h>
#include <map>
/*
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <string>

using namespace std;
*/


typedef struct
{
    char* response;
    size_t size;
} RETDATA;

struct POSTDATA {
  const char *readptr;
  size_t sizeleft;
};

class CURLSession
{
public:
  CURLSession();
  ~CURLSession();
  CURL*     Open();
  void      Close();
  CURL*     GetConnection();
  int       SetURL(const char* szURL);
  int       SetCookieFile(string filename);
  int       SetHeaderFile(string filename);
  int       SetWriteFile(string filename);
  int       SetDataFile(string filename);
  int       SetHeaderOut(string filename);
  int       SetVerbose(int option);
  string    GetURL();
  int       GetPort();
  string    GetHost();
  int       GetReturnCode() {return m_nRetCode;}
  RETDATA*  GetData() {return &m_Data;}
  RETDATA*  GetHeader() {return &m_Header;}
  int       SetVerifier(int option);

  // HTTP protocol methods..
  int       SetHTTPHeader(string key, string value);

  // test codes..


  // method..
  int       OpenURL(const char* szURL);
  int       PostURL(const char* szURL, const char* postData = NULL, int postDataLen = 0);
  int       PostURL(const char* szURL, string filename);
  int       m_nRetCode;
private:
  void      Release();
  int       PrepareOption(CURL* curl);
  //int       PostOption(CURL* curl);
  int       LoadHeader(string filename);
  int       OpenURL(CURL* curl, const char* szURL);
  int       PostURL(CURL* curl, const char* szURL, const char* postData = NULL, int postDataLen = 0);
  int       PostURL(CURL* curl, const char* szURL, string filename);
  int       QueryURL(CURL* curl, FILE* pfile = NULL);

  RETDATA   m_Data;
  RETDATA   m_Header;
  string    m_host;           // host name string
  int       m_port;           // port number
  string    m_url;            // url string
  // query option..
  int       m_verbose;
  int       m_verifier;
  string    m_cookie;         // input/output filename of cookie
  string    m_header;         // input filename of header
  string    m_data_file;      // input filename of post data
  string    m_header_file;    // output filename of header
  string    m_body_file;      // output filename of body
  // internal header stuff
  map<string, string> m_arr_headers;
  FILE*     m_pfile;
  struct curl_slist *m_phl;

  // response
  string    m_body;           // body string
  // connection.
  CURL*     m_curl;           // a connection of libcurl
};

#endif // __CURL_POOL__
