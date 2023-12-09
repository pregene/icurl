#include "curl_pool.h"
#include <fstream>

static size_t pool_read_callback(char *dest, size_t size, size_t nmemb, void *userp)
{
  struct POSTDATA *wt = (struct POSTDATA *)userp;
  size_t buffer_size = size*nmemb;

  if(wt->sizeleft) {
    /* copy as much as possible from the source to the destination */
    size_t copy_this_much = wt->sizeleft;
    if(copy_this_much > buffer_size)
      copy_this_much = buffer_size;
    memcpy(dest, wt->readptr, copy_this_much);

    wt->readptr += copy_this_much;
    wt->sizeleft -= copy_this_much;
    return copy_this_much; /* we copied this many bytes */
  }

  return 0; /* no more data left to deliver */
}

size_t pool_write_callback(void *data, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  CURLSession* p = (CURLSession*) userp;
  RETDATA *mem = (RETDATA *)p->GetData();

  char *ptr = (char*)realloc(mem->response, mem->size + realsize + 1);
  if(ptr == NULL)
   return 0;  /* out of memory! */

  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realsize);

  mem->size += realsize;
  mem->response[mem->size] = 0; // last pading is filled with zero value..
  return realsize;
}

size_t pool_header_callback(void *data, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  CURLSession* p = (CURLSession*) userp;
  RETDATA *mem = (RETDATA *)p->GetHeader();

  char *ptr = (char*)realloc(mem->response, mem->size + realsize + 1);
  if(ptr == NULL)
   return 0;  /* out of memory! */

  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realsize);

  mem->size += realsize;
  mem->response[mem->size] = 0; // last pading is filled with zero value..
  return realsize;
}

CURLSession::CURLSession() {
  m_curl = 0;m_port = 0;
  m_nRetCode=0;
  m_verifier = 0;
  memset(&m_Data, 0, sizeof(RETDATA));
  memset(&m_Header, 0, sizeof(RETDATA));
  Open();
}

CURLSession::~CURLSession() {Close();}

CURL*   CURLSession::Open()
{
  if (!m_curl)
  {
    m_curl = curl_easy_init();
  }
  return m_curl;
}
void    CURLSession::Close()
{
  if (m_curl)
    curl_easy_cleanup(m_curl);
  m_port = 0;
  m_curl = 0;
  if (m_Data.response)
    free(m_Data.response);
  if (m_Header.response)
    free(m_Header.response);
  m_nRetCode=0;
  m_verifier = 0;
}

CURL*   CURLSession::GetConnection() {return m_curl;}
int     CURLSession::SetURL(const char* szURL)
{
  if (!m_curl)
    return -1;

  m_url = szURL;
  curl_easy_setopt(m_curl, CURLOPT_URL, szURL);
  return 0;
}
string  CURLSession::GetURL()  {return m_url;}
int     CURLSession::GetPort() {return m_port;}
string  CURLSession::GetHost() {return m_host;}

// option method..
int       CURLSession::SetCookieFile(string filename)
{
  m_cookie = filename;
  return 0;
}
int       CURLSession::SetHeaderFile(string filename)
{
  m_header = filename;
  return 0;
}
int       CURLSession::SetWriteFile(string filename)
{
  m_body_file = filename;
  return 0;
}
int       CURLSession::SetDataFile(string filename)
{
  m_data_file = filename;
  return 0;
}
int       CURLSession::SetHeaderOut(string filename)
{
  m_header_file = filename;
  return 0;
}

int       CURLSession::SetVerifier(int option)
{
  return 0;
}

// url method..
int      CURLSession::OpenURL(const char* szURL)
{
  return OpenURL(m_curl, szURL);
}

void      CURLSession::Release()
{
  if (m_Data.response)
    free(m_Data.response);
  if (m_Header.response)
    free(m_Header.response);
  m_nRetCode=0;
  memset(&m_Data, 0, sizeof(RETDATA));
  memset(&m_Header, 0, sizeof(RETDATA));
}

int CURLSession::OpenURL(CURL* curl, const char* szURL)
{
  CURLcode res;

  if (!curl || !szURL)
    return -1; // curl handler error..
               // should call curl_global_init(CURL_GLOBAL_DEFAULT) before this.

  Release();
  //curl_easy_setopt(curl, CURLOPT_URL, szURL);
  SetURL(szURL);

  /* return */
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, pool_write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) this);

  /* user-agne */
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.71.1");
  /* header callback */
  curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, pool_header_callback);
  curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*) this);

  /* https */
  if (m_verifier == 0)
  {
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
  }

  /* header set */

  res = curl_easy_perform(curl);
  if(res != CURLE_OK)
    fprintf(stderr, "HTTP:GET:FAIL: %s\n",
            curl_easy_strerror(res));

  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &m_nRetCode);
  return 0;
}

int       CURLSession::LoadHeader(string filename)
{
  ifstream file;
  file.open(filename);
  string el;
  while (getline(file, el))
  {
    cout << el << endl;
  }
  return 0;
}
