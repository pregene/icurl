#if !defined(__CORETRUST_CA_CLIENT__)
#define __CORETRUST_CA_CLIENT__

#include "interpark.h"
#include <resolv.h>
#include <netdb.h>

typedef enum {
  SOCKET_UNKOWN = 0,
  SOCKET_TLS_1,
  SOCKET_TLS_12,
  SOCKET_TLS_13,
  SOCKET_CONNECT_MAX,
} SOCKET_CONNECT_TYPE;

class TLS_Socket
{
public:
    /* Constructors */
    TLS_Socket();
    TLS_Socket(int sockfd);
    /* Destructor */
    ~TLS_Socket();

    /* Get Properties */
    int         GetSocket();
    SSL*        GetSSLContext();
    struct sockaddr_in* GetSocketAddr();
    CASOCKETCB  GetCallback();
    void*       GetParam();

    /* socker I/O */
    int Send(string buffer);
    int Recv(string& buffer);
    // http protocol..
    int Send(string& header, string& body);
    int Recv(string& header, string& body);

    /* Set Properties */
    int SetSocket(int sockfd, SSL* ssl,
                          struct sockaddr_in* paddr = 0,
                          CASOCKETCB cb = 0,
                          void* param = 0);

    int Connect(string server, int port, int option);
    /* Client Event for Receiving Data */
    int OnReceive(SSL* ssl, char* buffer, int bufferlen);

    /* Receiving Thread */
#ifdef CA_CLIENT_THREAD
    static void* ClientThread(void* arg);
#endif

private:
    int Connect(const char *hostname, int port);
    int LoadCertificate(string pem);
#ifdef CA_CLIENT_THREAD
    /* Thread Informations and To start a client thread */
    void        RunThread();
    pthread_t   m_th;
#endif
    /* Properties */
    int         m_sock;
    SSL*        m_ssl;
    struct sockaddr_in m_addr;
    CASOCKETCB  m_cb;
    void*       m_param;

    /* SSL Context */
    SSL_CTX*    m_ctx;
};

#endif // __CORETRUST_CA_CLIENT__
