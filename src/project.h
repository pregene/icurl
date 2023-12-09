#if !defined(__ICURL_PROJECT__)
#define __ICURL_PROJECT__

#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <sstream>
#include <ctime>
#include <cstring>

#include <stdarg.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
/*
#include "openssl/ssl.h"
#include "openssl/err.h"
*/
#include <dirent.h>   // file 목록을 얻기 위해..
#include <sys/stat.h> // file 속성을 얻기 위해..

using namespace std;

std::string ltrim(const std::string &s);
std::string rtrim(const std::string &s);
std::string trim(const std::string &s);
std::vector<std::string> splitstring(std::string line, char ch);
bool file_exist (const std::string& name);
std::string strformat(const char* format, ...);

#define CA_SUCCESS  0
#define CA_FAIL    -1
#define CA_BUFFER_LEN 2048

typedef int (*CASOCKETCB)(void* pclass, int sockfd, int event_type, char* buffer, int bufferlen);

#endif // __ICURL_PROJECT__
