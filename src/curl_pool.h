#if !defined(__CURL_POOL__)
#define __CURL_POOL__

#include "project.h"
#include <curl/curl.h>
#include <map>
#include <fstream>
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

struct SEEKDATA {
  int our_fd;
};


class Cookie
{
public:
  Cookie() {m_partition = 0;}
  Cookie(string line)
  {
    m_partition = 0;
    ParseLine(line);
  }
  ~Cookie() {}

  void  ParseLine(string line)
  {
      string key[7];

      istringstream ss(line);
      string el;
      while (getline(ss, el, ';'))
      {
        vector<string> arrs = splitstring(el, '=');
        if (arrs.size() > 0)
        {
          if (arrs.at(0) == "path" ||
              arrs.at(0) == "Path")
          {
            key[1] = el;
          }
          else if (arrs.at(0) == "domain" ||
                   arrs.at(0) == "Domain")
          {
            key[2] = el;
          }
          else if (arrs.at(0) == "expires" ||
                   arrs.at(0) == "Expires")
          {
            key[3] = el;
          }
          else if (arrs.at(0) == "HttpOnly")
          {
            key[4] = el;
          }
          else if (arrs.at(0) == "SameSite")
          {
            key[5] = el;
          }
          else if (arrs.at(0) == "secure")
          {
            key[6] = el;
          }
          else
          {
            key[0] = el;
            m_key = arrs.at(0);
            if (arrs.size() > 1)
              m_value = arrs.at(1);
          }
        }
      }
      SetField( key[0],
                key[1],
                key[2],
                key[3],
                key[4],
                key[5],
                key[6],
                key[2].empty() ? 1 : 0);
  }

private:
  void SetField (   string key,
                    string path,
                    string domain,
                    string expire,
                    string httponly,
                    string secure,
                    string samesite,
                    int partition)
  {
    m_keyvalue = key;
    m_path = path;
    m_domain = domain;
    m_expire = expire;
    m_http_only = httponly;
    m_secure = secure;
    m_samsite = samesite;
    m_partition = partition;
  }

public:
  string GetKeyValue()  {return m_keyvalue;}
  string GetKey()       {return m_key;}
  string GetValue()     {return m_value;}
  string GetDomain()    {return m_domain;}
  string GetExpire()    {return m_expire;}
  string GetHttpOnly()  {return m_http_only;}
  string GetSecure()    {return m_secure;}
  int    GetPartition() {return m_partition;}

  void   SetKeyValue(string val)  {m_keyvalue=val;}
  void   SetKey(string val)       {m_key=val;}
  void   SetValue(string val)     {m_value=val;}
  void   SetDomain(string val)    {m_domain=val;}
  void   SetExpire(string val)    {m_expire=val;}
  void   SetHttpOnly(string val)  {m_http_only=val;}
  void   SetSecure(string val)    {m_secure=val;}
  void   SetPartition(int val)    {m_partition=val;}

  string DumpString()
  {
      string ret = m_keyvalue;
      ret += "\t";
      ret += m_domain;
      ret += "\t";
      ret += m_expire;
      ret += "\t";
      ret += m_http_only;
      ret += "\t";
      ret += m_secure;
      ret += "\t";
      ret += to_string(m_partition);
      return ret;
  }

  Cookie& operator=(Cookie& self)
  {
    this->SetKeyValue(self.GetKeyValue());
    this->SetKey(self.GetKey());
    this->SetValue(self.GetValue());
    this->SetDomain(self.GetDomain());
    this->SetExpire(self.GetExpire());
    this->SetHttpOnly(self.GetHttpOnly());
    this->SetSecure(self.GetSecure());
    this->SetPartition(self.GetPartition());
    return *this;
  }

private:
  string m_key;
  string m_value;
  string m_keyvalue;
  string m_path;
  string m_domain;
  string m_expire;
  string m_http_only;
  string m_secure;
  string m_samsite;
  int    m_partition;
};

class HTTPFormData {
public:
  HTTPFormData() {}
  ~HTTPFormData() {}

  void SetData(string key, string value)
  {
    map<string, string>::iterator search = m_arr_data.find(key);
    if (search == m_arr_data.end())
    {
      m_fields.push_back(key);
    }
    m_arr_data[key] = value;
  }

  string GetData(string key)
  {
    return m_arr_data[key];
  }

  string GetKeyName(unsigned int index)
  {
    if (index < m_fields.size())
      return m_fields.at(index);
    return "";
  }

  int GetFieldSize()
  {
    return m_fields.size();
  }

  int LoadFile(string filename)
  {
    ifstream file;
    string el;

    if (file_exist(filename) == false)
      return -1;

    m_arr_data.clear();
    m_fields.clear();

    file.open(filename);
    while (getline(file, el, '&'))
    {
      vector<string> arrs = splitstring(el, '=');
      if (arrs.size() == 2)
      {
        m_arr_data[arrs.at(0)] = arrs.at(1);
      }
      else
      {
        m_arr_data[arrs.at(0)] = "";
      }
      m_fields.push_back(arrs.at(0));
    }
    file.close();
    return 0;
  }

  int SaveFile(string filename)
  {
    ofstream file;

    file.open(filename);
    string data = GetDataString();
    file.write((char*)data.c_str(), data.size());
    file.close();
    return 0;
  }

  string GetDataString()
  {
    string ret;
    for (vector<string>::iterator it = m_fields.begin(); it != m_fields.end(); ++it)
    {
      if (!ret.empty())
      {
        ret += "&";
      }
      ret += *it;
      ret += "=";
      ret += m_arr_data[*it];
    }
    return ret;
  }

private:
  map<string, string> m_arr_data;
  vector<string>      m_fields;
};

class CURLSession
{
public:
  CURLSession();
  ~CURLSession();
  CURL*     Open();
  void      Close();
  CURL*     GetConnection();
  string    GetURL();
  //int       GetPort();  // deplicated..
  //string    GetHost();  // deplicated..
  int       GetReturnCode() {return m_nRetCode;}
  RETDATA*  GetData() {return &m_Data;}
  RETDATA*  GetHeader() {return &m_Header;}
  string    GetBody() {return m_body;}
  string    GetHeaderString() {string ret = m_Header.response; return ret;}

  // set curl option..
  int       SetCookieFile(string filename);
  int       SetHeaderFile(string filename);
  int       SetWriteFile(string filename);
  int       SetDataFile(string filename);
  int       SetHeaderOut(string filename);
  int       SetVerbose(int option);
  int       SetVerifier(int option);
  int       SetRedirect(int option);

  // get reponse header..
  string    GetHTTPHeader(string key);

  // HTTP protocol methods..
  int       SetHTTPHeader(string key, string value);
  void      RefreshHTTPHeader();  // load from header file(m_header) again..

  // get cookie from the cookie list of curl.
  string    GetJARCookie(string key);
  string    GetJARCookieValue(string key);
  string    GetJARCookies();

  // get cookie from own cookie stuff..
  string    GetCookie(string key);
  string    GetCookieValue(string key);
  string    GetCookies();
  void      SetCookie(string key, string value);

  // method..
  int       OpenURL(const char* szURL);
  // curl --data "abc=abc"
  int       PostURL(const char* szURL, const char* postData, int postDataLen);
  // curl --data @filename
  int       PostURL(const char* szURL, string filename);
  // curl --data-urlencode "abc=abc#!"
  int       PostURLEncode(const char* szURL, const char* postData, int postDataLen);
  // curl --data-urlencode @filename
  int       PostURLEncode(const char* szURL, string filename);

  // internal cookie list dump..
  void      DebugCookies();
  // curl cookie list dump..
  void      DebugJARCookies();

  // URLDecode..
  string    URLDecode(string str);

private:
  void      Release();
  int       PrepareOption(CURL* curl);
  int       ParseHeader();
  //int       PostOption(CURL* curl);
  int       SetURL(const char* szURL);
  int       LoadHeader(string filename);
  int       OpenURL(CURL* curl, const char* szURL);
  int       PostURL(CURL* curl, const char* szURL, const char* postData, int postDataLen);
  //int       PostURL(CURL* curl, const char* szURL, string filename); // deplicated..
  int       QueryURL(CURL* curl, FILE* pfile = NULL);
  int       LoadBodyFile();
  int       CheckCookie(string key);
  void      PrepareCookies(CURL* curl);


  RETDATA   m_Data;
  RETDATA   m_Header;
  //string    m_host;           // host name string(deplicated)
  //int       m_port;           // port number(deplicated)
  string    m_url;            // url string

  int       m_nRetCode;
  // query option..
  int       m_redirect;
  int       m_verbose;
  int       m_verifier;
  string    m_cookie;         // input/output filename of cookie
  string    m_header;         // input filename of header
  string    m_data_file;      // input filename of post data
  string    m_header_file;    // output filename of header
  string    m_body_file;      // output filename of body
  // internal header stuff
  map<string, string> m_arr_headers;
  map<string, string> m_res_headers;

  // cookie error fix..
  list<Cookie> m_arr_cookies;

  // output file pointer of body
  FILE*     m_pfile;

  // a pointer of http header list
  struct curl_slist *m_phl;

  // response
  string    m_body;           // body string
  // connection.
  CURL*     m_curl;           // a connection of libcurl
};

#endif // __CURL_POOL__
