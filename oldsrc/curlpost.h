#if !defined(__CT_HTTP_POST__)
#define __CT_HTTP_POST__

#include <stdio.h>      // for fprintf()
#include <string.h>     // for malloc(), alloc(), free()
#include <curl/curl.h>  // libcurl 사용. 빌드시 -lcurl 포함되어야 함.
                        //   libcurl.a를 사용해서 HTTPS 프로토콜 사용시
                        //   libssl.a, libcrypto.a를 포함해야 함.
#include <iostream>     // for c++ class library

typedef enum {
  CT_HTTP_FAILED  = -1, /*!< 함수 실행 실패시 */
  CT_HTTP_SUCCESS = 0,  /*!< 함수 실행 성공시 */
  // Information
  HTTP_RESPONSE_CONTINUE    = 100, /*!< 같은 요청이 들어 왔을 때,
                                        서버가 진행 중이라고 통보함.
                                        클라이언트가 기다리라고 내려줌. */
  HTTP_RESPONSE_SWITCHING   = 101, /*!< 서버 프로토콜을 변경할 때 사용.
                                        Websocket 프로토콜 전환 시에 사용 */
  HTTP_RESPONSE_PROCESSING  = 102, /*!< 이 응답 코드는 서버가 요청을 수신하였으며
                                        이를 처리하고 있지만, 아직 제대로
                                        된 응답을 알려줄 수 없음 */
  // Successful Response
  HTTP_RESPONSE_OK          = 200, /*!< 요청이 성공적으로 되었습니다.
                                        정보는 요청에 따른 응답으로 반환 */
  HTTP_RESPONSE_CREATED     = 201, /*!< 요청이 성공적이었으며 그 결과로 새로운
                                        리소스가 생성되었습니다.
                                        이 응답은 일반적으로 POST 요청
                                        또는 일부 PUT 요청 이후에 따라옵니다. */
  HTTP_RESPONSE_ACCEPTED    = 202, /*!< 요청을 수신하였지만, 그에 대응 할 수 없음 */
  HTTP_RESPONSE_NONAUTH     = 203, /*!< 돌려받은 메타 정보 세트가 오리진
                                        서버의 것과 일치하지 않지만 로컬이나
                                        서드 파티 복사본에서 모아졌음을 의미 */
  HTTP_RESPONSE_NOCONTENT   = 204, /*!< 요청에 대해서 보내줄 수 있는 콘텐츠가 없지만,
                                        헤더는 의미 있음. (쿠키 정보 내림) */
  HTTP_RESPONSE_RESET       = 205, /*!< 이 응답 코드는 요청을 완수한 이후에
                                        사용자 에이전트에게 이 요청을 보낸
                                        문서 뷰를 리셋하라고 알려 줌 */
  HTTP_RESPONSE_PARTIAL     = 206, /*!< 클라이언트가 이어받기를 시도하면
                                        웹서버가 이에 대한 응답코드로
                                        '206 Partial Content'와
                                        함께 Range 헤더에 명시된 데이터의
                                        부분(byte)부터 전송을 시작 */
  HTTP_RESPONSE_MULTISTATUS = 207, /*!< 여러 리소스가 여러 상태 코드인
                                        상황이 적절한 경우에 해당되는 정보를 전달 */
  HTTP_RESPONSE_PROSTAT     = 208, /*!< Prostat(property와 status의 합성어)
                                        응답 속성으로 동일 컬렉션으로 바인드된 복수의
                                        내부 멤버를 반복적으로 열거하는 것을 피하기 위해 사용 */
  HTTP_RESPONSE_IMUSED      = 209, /*!< 인스턴스 조작이 현재 인스턴스에
                                        적용이 되었음을 알려 줌 */
  // Redirection messages
  HTTP_RESPONSE_MULTIPLE    = 300, /*!< 요청에 대해서 하나 이상의 응답이 가능 */
  HTTP_RESPONSE_MOVED       = 301, /*!< 요청한 리소스의 URI가 변경되었음을 의미 */
  HTTP_RESPONSE_FOUND       = 302, /*!< 요청한 리소스의 URI가 일시적으로 변경되었음을 의미 */
  HTTP_RESPONSE_SEEOTHER    = 303, /*!< 요청한 리소스를 다른 URI에서 GET 요청을 통해
                                        얻어야 할 때, 서버가 클라이언트로 직접 보내는 응답 */
  HTTP_RESPONSE_NOTMODIFIED = 304, /*!< 캐시를 목적으로 사용. 응답이 수정되지
                                        않았음을 알려 줌. 클라이언트는 계속해서
                                        응답의 캐시된 버전을 사용할 수 있음. */
  HTTP_RESPONSE_USEPROXY    = 305, /*!< 요청한 응답은 반드시 프록시를 통해서 접속해야 함 */
  HTTP_RESPONSE_UNUSED      = 306, /*!< 더이상 사용되지 않으며, 현재는 추후 사용을 위해 예약 */
  HTTP_RESPONSE_TEMPREDIR   = 307, /*!< 요청한 리소스가 다른 URI에 있으며,
                                        이전 요청과 동일한 메소드를 사용하여
                                        요청해야 할 때, 서버가 클라이언트에
                                        이 응답을 직접 보냄 */
  HTTP_RESPONSE_REDIRECT    = 308, /*!< 301 Moved Permanently
                                        HTTP 응답 코드와 동일한 의미 */
  // Client error responses
  HTTP_RESPONSE_BADREQUEST  = 400, /*!< 잘못된 문법 */
  HTTP_RESPONSE_UNAUTH      = 401, /*!< '비인증(unauthenticated)'를 의미 */
  HTTP_RESPONSE_NEEDPAYMENT = 402, /*!< 디지털 결제 시스템에 사용하기 위하여
                                        만들어졌지만 지금 사용되고 있지는 않음 */
  HTTP_RESPONSE_FORBIDEN    = 403, /*!< 콘텐츠에 접근할 권리를 가지고 있지 않음 */
  HTTP_RESPONSE_NOTFOUND    = 404, /*!< 리소스를 찾을 수 없음 */
  HTTP_RESPONSE_NOTALLOWED  = 405, /*!< 요청한 메소드는 서버에서 알고 있지만,
                                        제거되었고 사용할 수 없음 */
  HTTP_RESPONSE_NOTACCEPTABLE = 406, /*!< 정해준 규격에 따른 어떠한 콘텐츠도
                                        찾지 않았을 때, 웹서버가 보냄 */
  HTTP_RESPONSE_NEEDPROXYAUTH = 407, /*!< 401과 비슷하지만 프록시에 의해
                                        완료된 인증이 필요 */
  HTTP_RESPONSE_TIMEOUT     = 408, /*!< 응답은 요청을 한 지 시간이 오래된 연결에 일부 서버가 전송 */
  HTTP_RESPONSE_CONFLICT    = 409, /*!< 요청이 현재 서버의 상태와 충돌 */
  HTTP_RESPONSE_GONE        = 410, /*!< 콘텐츠가 서버에서 영구적으로 삭제 */
  HTTP_RESPONSE_NEEDLENGTH  = 411, /*!< 서버에서 필요로 하는 Content-Length 헤더 필드가
                                        정의되지 않은 요청이 들어왔기 때문에 서버가 요청 */
  HTTP_RESPONSE_FAILEDCONDS = 412, /*!< 클라이언트의 헤더에 있는 전제조건은 서버의 전제조건에 적절하지 않음 */
  HTTP_RESPONSE_LARGEPAYLOAD = 413, /*!< 요청 엔티티는 서버에서 정의한 한계보다 큼 */
  HTTP_RESPONSE_LONGURI     = 414, /*!< 클라이언트가 요청한 URI는 서버에서 처리하지 않기로 한 길이보다 김 */
  HTTP_RESPONSE_ERRMEDIATYPE = 415, /*!< 요청한 미디어 포맷은 서버에서 지원하지 않음 */
  HTTP_RESPONSE_RANGEERROR  = 416, /*!< Range 헤더 필드에 요청한 지정 범위를 만족시킬 수 없음 */
  HTTP_RESPONSE_EXPECTERROR = 417, /*!< Expect 요청 헤더 필드로 요청한 예상이 서버에서는 적당하지 않음 */
  HTTP_RESPONSE_TEAPOT      = 418, /*!< 찻 주전자 */
  HTTP_RESPONSE_MISDIRECTED = 421, /*!< 서버로 유도된 요청은 응답을 생성할 수 없음 */
  HTTP_RESPONSE_UNPROCESS   = 422, /*!< 요청은 잘 만들어졌지만, 문법 오류 */
  HTTP_RESPONSE_LOCKED      = 423, /*!< 리소스는 접근하는 것이 잠겨있음 */
  HTTP_RESPONSE_FAILDEPEND  = 424, /*!< 이전 요청이 실패하였기 때문에 지금의 요청도 실패 */
  HTTP_RESPONSE_NEEDUPGRADE = 426, /*!< 클라이언트가 다른 프로토콜로 업그레이드 */
  HTTP_RESPONSE_NEEDPRECONDITION = 428, /*!< 요청이 조건적이어야 함 */
  HTTP_RESPONSE_TOOMANYREQ  = 429, /*!< 사용자가 지정된 시간에 너무 많은 요청을 보냄 */
  HTTP_RESPONSE_LARGEHEADER = 431, /*!< 요청한 헤더 필드가 너무 큼 */
  HTTP_RESPONSE_IRREGALACCESS = 451, /*!< 요청한 것은 정부에 의해 검열된 웹페이지 */
  // Server error reponses
  HTTP_RESPONSE_INTERNALERROR = 500, /*!< 웹 사이트 서버에 문제가 있음 */
  HTTP_RESPONSE_NOTIMPL     = 501, /*!< 서버가 요청을 이행하는 데 필요한 기능을 지원하지 않음 */
  HTTP_RESPONSE_BADGATEWAY  = 502, /*!< 서버가 게이트웨이로부터 잘못된 응답을 수신 */
  HTTP_RESPONSE_UNAVAILABLE = 503, /*!< 서버가 요청을 처리할 준비가 되지 않았음 */
  HTTP_RESPONSE_GATEWAYTIMEOUT = 504, /*!< 인터넷상의 서버 간의 네트워크 오류이거나 실제 서버의 문제 */
  HTTP_RESPONSE_VERSIONERROR = 505, /*!< 서버에서 지원되지 않는 HTTP 버전을 클라이언트가 요청 */
  HTTP_RESPONSE_VARIANTERROR = 506, /*!< 서버에 내부 구성 오류가 있는 경우 발생 */
  HTTP_RESPONSE_INSUFFICIENT = 507, /*!< 선택한 가변 리소스는 투명한 서버에 내부 구성 요류가 있는 경우 발생 */
  HTTP_RESPONSE_LOOPDETECTED = 508, /*!< 서버가 요청을 처리하는 동안 무한 루프를 감지 */
  HTTP_RESPONSE_NOTEXTEND    = 510, /*!< 서버가 요청을 이행하려면 요청에 대한 추가 확장이 필요 */
  HTTP_RESPONSE_NEEDNETWORKAUTH = 511, /*!< 클라이언트가 네트워크 액세스를 얻기 위해 인증할 필요가 있음 */
  CT_HTTP_STATUS_MAX,   /*!< 최대 값 */
} HTTP_STATUS;

typedef struct
{
    char* response; /*!< HTTP 프로토콜 POST, GET로 서버로 부터 받은 데이터 */
    size_t size;    /*!< HTTP 프로토콜 POST, GET로 서버로 부터 받은 데이터의 크기 */
} HTTPDATA, *PHTTPDATA;


class CTHttpPost
{
public:
  CTHttpPost();
  ~CTHttpPost();


  int       OpenPostURL(const char* szURL,
                        const char* postData = NULL,
                        int postDataLen = 0);
  int       GetReturnCode() {return m_nRetCode;}
  HTTPDATA* GetData()   {return &m_Data;}
  HTTPDATA* GetHeader() {return &m_Header;}
  int       m_nRetCode;
private:
  HTTPDATA  m_Data;
  HTTPDATA  m_Header;
};
#endif // __CT_HTTP_POST__
