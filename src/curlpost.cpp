#include "curlpost.h"

struct POSTDATA {
  const char *readptr;
  size_t sizeleft;
};

size_t post_header_callback(char *data, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  CTHttpPost* p = (CTHttpPost*) userp;
  HTTPDATA *mem = (HTTPDATA *)p->GetHeader();

  char *ptr = (char*)realloc(mem->response, mem->size + realsize + 1);
  if(ptr == NULL)
   return 0;  /* out of memory! */

  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realsize);

  mem->size += realsize;
  mem->response[mem->size] = 0; // last pading is filled with zero value..
  return realsize;
}

static size_t post_read_callback(char *dest, size_t size, size_t nmemb, void *userp)
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

size_t post_write_callback(void *data, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  CTHttpPost* p = (CTHttpPost*) userp;
  HTTPDATA *mem = (HTTPDATA *)p->GetData();

  char *ptr = (char*)realloc(mem->response, mem->size + realsize + 1);
  if(ptr == NULL)
   return 0;  /* out of memory! */

  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realsize);

  mem->size += realsize;
  mem->response[mem->size] = 0; // last pading is filled with zero value..
  return realsize;
}

CTHttpPost::CTHttpPost()
{
  m_nRetCode=0;
  memset(&m_Data, 0, sizeof(HTTPDATA));
  memset(&m_Header, 0, sizeof(HTTPDATA));
}

CTHttpPost::~CTHttpPost()
{
  if (m_Data.response)
    free(m_Data.response);
  if (m_Header.response)
    free(m_Header.response);
}

int CTHttpPost::OpenPostURL(const char* szURL,
                           const char* postData,
                           int postDataLen)
{
  CURL *curl;
  CURLcode res;
  POSTDATA data;

  if (!szURL)
    return CT_HTTP_FAILED; // URL Error..

  curl = curl_easy_init();

  if (!curl)
    return CT_HTTP_FAILED; // curl handler error..
               // should call curl_global_init(CURL_GLOBAL_DEFAULT) before this.
  curl_easy_setopt(curl, CURLOPT_URL, szURL);

  /* Now specify we want to POST data */
  curl_easy_setopt(curl, CURLOPT_POST, 1L);

  /* we want to use our own read function */
  curl_easy_setopt(curl, CURLOPT_READFUNCTION, post_read_callback);

  if (postData && postDataLen > 0)
  {
    data.readptr = postData;
    data.sizeleft = postDataLen;
  }
  /* pointer to pass to our read function */
  curl_easy_setopt(curl, CURLOPT_READDATA, &data);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)data.sizeleft);

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
    fprintf(stderr, "HTTP:POST:FAIL: %s\n",
            curl_easy_strerror(res));

  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &m_nRetCode);
  curl_easy_cleanup(curl);

  // you shoud call curl_global_cleanup() if not use anymore..
  return CT_HTTP_SUCCESS;
}
