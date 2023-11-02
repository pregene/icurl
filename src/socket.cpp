#include "socket.h"

#define SUCCESS 0
#define FAIL    -1

TLS_Socket::TLS_Socket()
{
    m_sock = 0;
    m_ssl = 0;
    m_cb = 0;
    m_param = 0;
    //m_pcert = 0;
    m_ctx = 0;
    memset(&m_addr, 0, sizeof(struct sockaddr_in));
#ifdef CA_CLIENT_THREAD
    m_th = 0;
#endif
}

TLS_Socket::TLS_Socket(int sockfd)
{
    m_sock = sockfd;
    m_ssl = 0;
    m_cb = 0;
    m_param = 0;
    //m_pcert = 0;
    m_ctx = 0;
    memset(&m_addr, 0, sizeof(struct sockaddr_in));
#ifdef CA_CLIENT_THREAD
    m_th = 0;
#endif
}

TLS_Socket::~TLS_Socket()
{
#ifdef CA_CLIENT_THREAD
    if (m_th)
    {
        void* status = 0;
        close(m_sock);
        pthread_join(m_th, (void**)&status);
    }
#endif
    if (m_ssl)
    {
      SSL_free(m_ssl);
      m_ssl = 0;
    }
    if (m_ctx)
    {
      SSL_CTX_free(m_ctx);
      m_ctx = 0;
    }
}

int TLS_Socket::SetSocket(int sockfd, SSL* ssl, struct sockaddr_in* paddr, CASOCKETCB cb, void* param)
{
    m_sock = sockfd;
    m_ssl = ssl;
    if (paddr)
      memcpy(&m_addr, paddr, sizeof(struct sockaddr_in));
    m_cb = cb;
    m_param = param;
#ifdef CA_CLIENT_THREAD
    m_th = 0;
    RunThread();
#endif
    return 0;
}

int TLS_Socket::GetSocket()
{
  return m_sock;
}

SSL* TLS_Socket::GetSSLContext()
{
    return m_ssl;
}

/*
string& TLS_Socket::GetIssuer()
{
  return m_issuer.m_certline;
}

string& TLS_Socket::GetCertificate()
{
  return m_subject.m_certline;
}
*/

//CAStatus* CAClient::GetStatus()
//{
//  return &m_status;
//}

/*
void    TLS_Socket::SetIssuer(string issuer)
{
  m_issuer.ParseString(issuer);
}

void    TLS_Socket::SetCertificate(string certificate)
{
  m_subject.ParseString(certificate);
}
*/
struct sockaddr_in* TLS_Socket::GetSocketAddr()
{
  return &m_addr;
}

CASOCKETCB TLS_Socket::GetCallback()
{
  return m_cb;
}

void*   TLS_Socket::GetParam()
{
  return m_param;
}

int TLS_Socket::OnReceive(SSL* ssl, char* buffer, int bufferlen)
{
    if (m_cb)
      return m_cb((void*)this, m_sock, 1, buffer, bufferlen);
    return 0;
}

int TLS_Socket::Send(string buffer)
{
  if (m_ssl)
    return SSL_write(m_ssl, buffer.c_str(), buffer.length());
  else if (m_sock)
    return write(m_sock, buffer.c_str(), buffer.length());
  return CA_FAIL;
}

int TLS_Socket::Recv(string& buffer)
{
  char buf[1024];
  buffer.clear();
  int bytes = 0;
  if (m_ssl)
  {
    while ((bytes = SSL_read(m_ssl, buf, sizeof(buf) - 1)) == 1023)
    {
        buf[bytes] = 0;
        buffer += buf;
    }
    if (bytes > 0)
    {
      buf[bytes] = 0;
      buffer += buf;
    }
  }
  else if (m_sock)
  {
    while ((bytes = read(m_sock, buf, sizeof(buf) - 1)) == 1023)
    {
        buf[bytes] = 0;
        buffer += buf;
    }
    if (bytes > 0)
    {
      buf[bytes] = 0;
      buffer += buf;
    }
  }
  return buffer.length();
}

// http protocol..
int TLS_Socket::Send(string& header, string& body)
{
  return 0;
}
int TLS_Socket::Recv(string& header, string& body)
{
  string buffer;
  int bytes = Recv(buffer);
  if (bytes > 0)
  {
    header = buffer;
    size_t pos = buffer.find("\n\n");
    size_t pos2 = string::npos;
    if (pos != string::npos)
    {
      header  = trim(buffer.substr(0, pos));
      pos2    = buffer.find("\n\n", pos + 2);
    }
    if (pos2 != string::npos)
    {
      body    = trim(buffer.substr(pos + 2));
    }
  }
  return bytes;
}

#ifdef CA_CLIENT_THREAD
void* TLS_Socket::ClientThread(void* arg)
{
    CAClient* sock = (CAClient*) arg;
    if (sock)
    {
        int sockfd      = sock->GetSocket();
        SSL* ssl        = sock->GetSSLContext();
        struct sockaddr_in* paddr = sock->GetSocketAddr();
        CASOCKETCB cb   = sock->GetCallback();
        int status      = 0;
        CA_printf("IMS:CLIENT=%d, [%s:%d]\n", sockfd, inet_ntoa(paddr->sin_addr), ntohs(paddr->sin_port));
        //CA_printf("IMS:CLIENT:SUBJECT=%s\n", sock->GetCertificate().c_str());
        //CA_printf("IMS:CLIENT:ISSUER =%s\n", sock->GetIssuer().c_str());
        if (sockfd != -1 && sockfd != 0)
        {
            // 인증서 정보를 얻는다..
            do {
                string buffer;
                status = sock->Recv(buffer);
                //printf("IMS:CLIENT:READ=%d\n", status);
                if (status == 0 || status < 0)
                  break;

                // do something..
                if (status > 0)
                status = sock->OnReceive(ssl, (char*)buffer.c_str(), buffer.length());
            } while (status >= CA_SUCCESS);

        }
        if (cb)
          cb((void*)sock, sockfd, -1, 0, 0);

        // socket을 명시적으로 닫는다.
        close(sockfd);
        if (ssl) SSL_free(ssl);
    }
    return arg;
}
#endif

#ifdef CA_CLIENT_THREAD
void        TLS_Socket::RunThread()
{
    if (!m_th)
    {
        pthread_create(&m_th, 0, CAClient::ClientThread, (void*) this);
    }
}
#endif

int TLS_Socket::Connect(const char *hostname, int port)
{   int sd;
    struct hostent *host;
    struct sockaddr_in addr;
    printf("\t1\n");

    if ( (host = gethostbyname(hostname)) == NULL )
    {
        perror(hostname);
        return -1;
    }
    printf("\t2\n");
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    printf("\t3\n");
    if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        close(sd);
        perror(hostname);
        return -1;
    }
    printf("\t4\n");
    return sd;
}

X509* read_certificate(string filename)
{
  X509* x509 = 0;
  FILE* file = fopen(filename.c_str(), "r");
  if (file)
  {
    x509 = PEM_read_X509(file, 0, 0, 0);
    fclose(file);
  }
  return x509;
}

EVP_PKEY* read_privatekey(string filename)
{
  EVP_PKEY* prkey = 0;
  FILE* file = fopen(filename.c_str(), "r");
  if (file)
  {
    prkey = PEM_read_PrivateKey(file, 0, 0, 0);
    fclose(file);
  }
  return prkey;
}

int TLS_Socket::LoadCertificate(string pem)
{
  X509*       x509      = 0;
  EVP_PKEY*   prkey     = 0;
  int         ret_code  = SUCCESS;
  if (file_exist(pem))
  {
    x509  = read_certificate(pem);
    prkey = read_privatekey(pem);

    if (!x509 || SSL_CTX_use_certificate(m_ctx, x509) <= 0 )
    {
      ret_code = -2;
      goto got_error;
    }
    if (!prkey || SSL_CTX_use_PrivateKey(m_ctx, prkey) <= 0 )
    {
      ret_code = -1;
      goto got_error;
    }
    if ( !SSL_CTX_check_private_key(m_ctx) )
    {
      ret_code = -3;
      goto got_error;
    }
  }
got_error:
  return ret_code;
}

int TLS_Socket::Connect(string server, int port, int option)
{
  SSL_library_init();
  const SSL_METHOD *method;

  printf("1\n");
  OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
  SSL_load_error_strings();   /* Bring in and register error messages */

  printf("2\n");
  if (option == SOCKET_UNKOWN)
  {
      m_sock = Connect(server.c_str(), port);
  }
  else
  {
    printf("3\n");
    method = TLS_client_method(); // TLSv1_2_client_method();  /* Create new client-method instance */
    m_ctx = SSL_CTX_new(method);

    printf("4\n");
    if ( m_ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        return -1;
    }
    printf("5\n");
    // 클라이언트 인증서를 로드한다.
    if (LoadCertificate("certs/connect.pem") != SUCCESS)
    {
        SSL_CTX_free(m_ctx);
        m_ctx = 0;
        return -1;
    }
    printf("6\n");
    m_sock = Connect(server.c_str(), port);
    m_ssl = SSL_new(m_ctx);
    SSL_set_fd(m_ssl, m_sock);
    printf("7\n");
    if ( SSL_connect(m_ssl) == FAIL )
    {
        ERR_print_errors_fp(stderr);
        SSL_free(m_ssl);
        m_ssl = 0;
        SSL_CTX_free(m_ctx);
        m_ctx = 0;
        return -1;
    }
    printf("8\n");
  }
  return 0;
}
