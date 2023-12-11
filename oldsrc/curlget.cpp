#include "curlget.h"

size_t post_write_callback(void *data, size_t size, size_t nmemb, void *userp);
size_t post_header_callback(char *data, size_t size, size_t nmemb, void *userp);

CTHttpGet::CTHttpGet()
{
  m_url       = 0;
  m_object    = 0;
#ifdef _AP4_H_
  m_pFile     = NULL;
#endif
}

CTHttpGet::~CTHttpGet()
{
  if (m_url)
    free(m_url);
  m_url = 0;
  if (m_object)
    free(m_object);
  m_object = 0;
#ifdef _AP4_H_
  if (m_pFile)
    m_pFile->Release();
#endif
}

void    CTHttpGet::SetObject(const char* szURL)
{
  char* pos = 0;
  if (m_url)
    free(m_url);
  m_url = (char*) malloc(strlen(szURL) + 1);
  strcpy(m_url, szURL);

  if (m_object)
    free(m_object);
  m_object = (char*) malloc(strlen(szURL) + 1);
  pos = strrchr(m_url, '/');
  if (pos)
  {
    strcpy(m_object, pos + 1);
  }
  else
  {
    strcpy(m_object, m_url);
  }
}

int CTHttpGet::OpenURL(const char* szURL)
{
  CURL *curl;
  CURLcode res;

  if (!szURL)
    return -1; // URL Error..

  SetObject(szURL);

  curl = curl_easy_init();

  if (!curl)
    return -1; // curl handler error..
               // should call curl_global_init(CURL_GLOBAL_DEFAULT) before this.
  curl_easy_setopt(curl, CURLOPT_URL, szURL);

  /* return */
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, post_write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) this);

  /* header */
  curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, post_header_callback);
  curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*) this);

  /* https */
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

  res = curl_easy_perform(curl);
  if(res != CURLE_OK)
    fprintf(stderr, "HTTP:GET:FAIL: %s\n",
            curl_easy_strerror(res));

  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &m_nRetCode);
  curl_easy_cleanup(curl);

  // you shoud call curl_global_cleanup() if not use anymore..
  return 0;
}

int CTHttpGet::OpenURL(const char* szURL, HTTPDATA* data)
{
  if (!data && OpenURL(szURL) == 0 && GetReturnCode() == 200)
  {
    HTTPDATA* p        = GetData();
    data->size        = p->size;
    data->response    = p->response;
    p->size           = 0;
    p->response       = 0;
    return 0;
  }
  return -1;
}

#ifdef USED_BENTO4
size_t media_download_callback(void *data, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  CHttpGET* p = (CHttpGET*) userp;
  AP4_MemoryByteStream* pFile = NULL;
  AP4_Size writesize = 0;

  if (!p)
    return 0;

  pFile = (AP4_MemoryByteStream*) p->GetFile();
  if (!pFile)
    return 0;

  pFile->WritePartial(data, realsize, writesize);
  return realsize;
}

int CTHttpGet::DownloadMedia(const char* szURL, AP4_MemoryByteStream* pFile)
{
  CURL *curl;
  CURLcode res;
  int reference = 0;

  if (!szURL)
    return -1; // URL Error..

  curl = curl_easy_init();

  if (!curl)
    return -1; // curl handler error..
               // should call curl_global_init(CURL_GLOBAL_DEFAULT) before this.
  SetObject(szURL);
  if (!pFile)
  {
    if (m_pFile)
      m_pFile->Release();

    m_pFile = new AP4_MemoryByteStream;
  }
  else
  {
    reference = 1;
    m_pFile = pFile;
  }
  curl_easy_setopt(curl, CURLOPT_URL, szURL);

  /* return */
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, media_download_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) this);

  /* header */
  curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, post_header_callback);
  curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*) this);

  /* https */
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

  res = curl_easy_perform(curl);
  if(res != CURLE_OK)
    fprintf(stderr, "HTTP:GET:FAIL: %s\n",
            curl_easy_strerror(res));

  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &m_nRetCode);
  curl_easy_cleanup(curl);

  // you shoud call curl_global_cleanup() if not use anymore..
  if (reference == 1)
    m_pFile = NULL;
  return 0;
}
#endif
