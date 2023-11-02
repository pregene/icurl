#if !defined(__CT_HTTP_GET__)
#define __CT_HTTP_GET__

#include "curlpost.h"

/**
  @file
  @brief   HTTP 프로토콜 GET 사용하기 위한 클래스 (HTTP, HTTPS 모두 사용 가능)
  @class   CTHttpGet
            + 함수: OpenURL()
            + 함수: GetObject()
            + 함수: GetRequestURL()
            + 함수: DownloadMedia(const char* szURL, AP4_MemoryByteStream* pFile)
            + 함수: GetFile()
            + 함수: GetDataPtr()
            + 함수: GetDataSize()
  @author  paul@coretrust.com
  @date    2022-07-11

  \code{.cpp}
    ...
    int ret = 0;
    CTHttpGet httpget;
    ret = httpget.OpenURL("https://www.google.co.kr");
    if (ret == CT_HTTP_SUCCESS && httpget.GetReturnCode() == HTTP_RESPONSE_OK)
    {
      // success..
      printf("SIZE=%d\n", httpget.GetData()->size);
      printf("DATA=%s\n", httpget.GetData()->response);
    }
    else
    {
      // failed
      printf("HTTP:GET:Failed\n");
    }
    ...
  \endcode

  @note
*/
class CTHttpGet : public CTHttpPost
{
public:
  /**
    @brief CTHttpGet 클래스의 생성자
  */
  CTHttpGet();
  /**
    @brief CTHttpGet 클래스의 소멸자
  */
  ~CTHttpGet();

  /**
    @brief OpenURL(const char* szURL)함수는 HTTP 또는 HTTPS의 GET 메서드를 호출하여 URL의 내용을 메모리에 저장한다.
    @param const char* szURL HTTP 또는 HTTPS 프로토콜의 Uniform Resource Locator(URL)
    @return 성공시, CT_HTTP_SUCCESS 반환. 실패시, CT_HTTP_FAILED 반환.
    @author paul@coretrust.com
    @date 2022-07-11

    \code{.cpp}
      ...
      int ret = 0;
      CTHttpGet httpget;
      ret = httpget.OpenURL("https://www.google.co.kr");
      if (ret == CT_HTTP_SUCCESS && httpget.GetReturnCode() == HTTP_RESPONSE_OK)
      {
        // success..
        ...
        printf("URI: %s\n", httpget.GetObject());
        ...

      }
      else
      {
        // failed
        printf("HTTP:GET:Failed\n");
      }
      ...
    \endcode
  */
  int         OpenURL(const char* szURL);
  /**
    @brief OpenURL(const char* szURL, RETDATA* data)함수는 HTTP 또는 HTTPS의 GET 메서드를 호출하여 URL의 내용을 HTTPDATA* 구조체에 반환
    @param const char* szURL HTTP 또는 HTTPS 프로토콜의 Uniform Resource Locator(URL)
    @param HTTPDATA* data  다운로드 된 내용을 반환하기 위한 구조체의 포인터
    @return 성공시, CT_HTTP_SUCCESS 반환. 실패시, CT_HTTP_FAILED 반환.
    @author paul@coretrust.com
    @date 2022-07-11
    @note HTTPDATA 구조체로 반환 받은 메모리 주소는 자동으로 해지되지 않으므로 직접 해지 해주어야 한다.

    \code{.cpp}
      ...
      int ret = 0;
      HTTPDATA data;
      CTHttpGet httpget;
      memeset(&data, 0, sizeof(HTTPDATA));
      ret = httpget.OpenURL("https://www.google.co.kr", &data);
      if (ret == CT_HTTP_SUCCESS && httpget.GetReturnCode() == HTTP_RESPONSE_OK)
      {
        // success..
        ...
        printf("URI: %s\n", httpget.GetObject());
        printf("SIZE: %d\n", data.size);
        printf("DATA: %s\n", data.response);
        ...

        // 반환 받은 메모리를 직접 해지..
        if (data.response)
          free(data.response);
      }
      else
      {
        // failed
        printf("HTTP:GET:Failed\n");
      }
      ...
    \endcode
  */
  int         OpenURL(const char* szURL, HTTPDATA* data);
  /**
    @brief OpenURL()함수를 호출할 때, 입력 받게되는 URL에서 저장해 놓은 Object URI 문자열
    @return Object URI의 주소를 반환
    @author paul@coretrust.com
    @date 2022-07-11

    \code{.cpp}
      ...
      int ret = 0;
      CTHttpGet httpget;
      ret = httpget.OpenURL("https://www.google.co.kr");
      if (ret == CT_HTTP_SUCCESS && httpget.GetReturnCode() == HTTP_RESPONSE_OK)
      {
        // success..
        ...
        printf("URI: %s\n", httpget.GetObject());
        ...

      }
      else
      {
        // failed
        printf("HTTP:GET:Failed\n");
      }
      ...
    \endcode
  */
  const char* GetObject() {return m_object;}
  /**
    @brief OpenURL()함수를 호출할 때, 입력 받게되는 URL을 반환
    @return 요청 URL 주소를 반환
    @author paul@coretrust.com
    @date 2022-07-11

    \code{.cpp}
      ...
      int ret = 0;
      CTHttpGet httpget;
      ret = httpget.OpenURL("https://www.google.co.kr");
      if (ret == CT_HTTP_SUCCESS && httpget.GetReturnCode() == HTTP_RESPONSE_OK)
      {
        // success..
        printf("URL: %s\n", httpget.GetRequestURL());
        ...

      }
      else
      {
        // failed
        printf("HTTP:GET:Failed\n");
      }
      ...
    \endcode
  */
  const char* GetRequestURL()    {return m_url;}
  /**
    @brief DownloadMedia()함수를 호출하여 다운로드된 메모리의 주소를 반환
    @return const char* 형의 메모리 주소
    @author paul@coretrust.com
    @date 2022-07-11

    \code{.cpp}
      ...
      int ret = 0;
      CTHttpGet httpget;
      ret = httpget.OpenURL("https://www.google.co.kr");
      if (ret == CT_HTTP_SUCCESS && httpget.GetReturnCode() == HTTP_RESPONSE_OK)
      {
        // success..
        ...
        printf("DATA=%s\n", httpget.GetDataPtr());

      }
      else
      {
        // failed
        printf("HTTP:GET:Failed\n");
      }
      ...
    \endcode
  */
  const char* GetDataPtr() {return GetData()->response;}
  /**
    @brief DownloadMedia()함수를 호출하여 다운로드된 메모리의 주소의 할당 크기를 반환
    @return const char* 형의 메모리 주소의 할당 size_t형의 크기
    @author paul@coretrust.com
    @date 2022-07-11

    \code{.cpp}
      ...
      int ret = 0;
      CTHttpGet httpget;
      ret = httpget.OpenURL("https://www.google.co.kr");
      if (ret == CT_HTTP_SUCCESS && httpget.GetReturnCode() == HTTP_RESPONSE_OK)
      {
        // success..
        ...
        printf("SIZE=%d\n", httpget.GetDataSize());
        ...

      }
      else
      {
        // failed
        printf("HTTP:GET:Failed\n");
      }
      ...
    \endcode
  */
  size_t      GetDataSize() {return GetData()->size;}

#ifdef USED_BENTO4
  /**
    @brief DownloadMedia()함수를 호출하여 다운로드된 메모리 파일의 인스턴스의 포인터를 반환
    @return AP4_MemoryByteStream 클래스 포인터를 반환
    @author paul@coretrust.com
    @date 2022-07-11
  */
  AP4_MemoryByteStream* GetFile() {return m_pFile;}
  /**
    @brief DownloadMedia()함수는 HTTP URL로 부터, AP4_MemoryByteStream 클래스로 미디어 파일을 다운로드 한다.
    @param const char* szURL HTTP 또는 HTTPS 프로토콜의 Uniform Resource Locator(URL)
    @param AP4_MemoryByteStream* pFile 반환 받을 클래스 인스턴스의 포인터.
    @return 성공시, CT_HTTP_SUCCESS 반환. 실패시, CT_HTTP_FAILED 반환.
    @author paul@coretrust.com
    @date 2022-07-11
    @note AP4_MemoryByteStream* pFile 파라미터가 NULL인 경우는 CTHttpGet 클래스의 멤버 변수를 활용해서
          다운로드를 실행한다. 파일은 클래스가 해지 될 때, 자동으로 해지 된다.

    \code{.cpp}
      ...
      int ret = 0;
      CTHttpGet httpget;
      ret = httpget.DownloadMedia("https://server:port/media/01.mp4");
      if (ret == CT_HTTP_SUCCESS && httpget.GetReturnCode() == HTTP_RESPONSE_OK)
      {
        // success..
        ...
        AP4_MemoryByteStream* pFile = GetFile();
        printf("SIZE: %d\n", pFile->GetDataSize());
        ...
      }
      else
      {
        // failed
        printf("HTTP:GET:Failed\n");
      }
      ...
    \endcode

    \code{.cpp}
      ...
      int ret = 0;
      AP4_MemoryByteStream* pFile = new AP4_MemoryByteStream;
      CTHttpGet httpget;
      ret = httpget.DownloadMedia("https://server:port/media/01.mp4", pFile);
      if (ret == CT_HTTP_SUCCESS && httpget.GetReturnCode() == HTTP_RESPONSE_OK)
      {
        // success..
        ...
        printf("SIZE: %d\n", pFile->GetDataSize());
        ...
      }
      else
      {
        // failed
        printf("HTTP:GET:Failed\n");
      }
      ...
      if (pFile)
        pFile->Release();
      ...
    \endcode
  */
  int      DownloadMedia(const char* szURL, AP4_MemoryByteStream* pFile = NULL);
#endif

protected:
  /**
    @brief OpenURL()함수를 호출하면, Object URI를 얻어서 저장하기 위해 내부에서 실행
    @param const char* szURL HTTP 또는 HTTPS 프로토콜의 Uniform Resource Locator(URL)
    @return void
    @author paul@coretrust.com
    @date 2022-07-11
  */
  void        SetObject(const char* szURL);
private:
  char*     m_url;
  char*     m_object;
};

#endif // __CT_HTTP_GET__
