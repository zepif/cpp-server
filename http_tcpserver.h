#ifndef INCLUDED_HTTP_TCPSERVER
#define INCLUDED_HTTP_TCPSERVER

#include <stdio.h>
#include <stdlib.h>
#include <string>
#ifdef _WIN32
#include <Winsock2.h>
#endif

namespace http {

class TcpServer {
public:
  TcpServer();
  ~TcpServer();

private:
  int m_port;
  int m_socket;
  int m_new_socket;
  long long m_incomingMessage;
  struct sockaddr_in m_socketAddress;
  int m_socketAddress_len;
  std::string m_serverMessage;
  WSADATA m_wsaData;

  int startServer();
  void closeServer();
};

} // namespace http

#endif
