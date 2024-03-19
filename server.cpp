#include <http_tcpserver.h>
#include <iostream>

int main() {
  using namespace http;

  TcpServer server = TcpServer(8000);
  server.startListen();
  std::cout << "goyda";

  return 0;
}
