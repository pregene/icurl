#if !defined(__CT_HTTP_GET__)
#define __CT_HTTP_GET__

#include "curlpost.h"

class CTHttpGet : public CTHttpPost
{
public:
  CTHttpGet();
  ~CTHttpGet();

  int         OpenURL(const char* szURL);
  int         OpenURL(const char* szURL, HTTPDATA* data);
  const char* GetObject() {return m_object;}
  const char* GetRequestURL()    {return m_url;}
  const char* GetDataPtr() {return GetData()->response;}
  size_t      GetDataSize() {return GetData()->size;}

protected:
  void        SetObject(const char* szURL);
private:
  char*     m_url;
  char*     m_object;
};

#endif // __CT_HTTP_GET__
