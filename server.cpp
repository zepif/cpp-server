#include <http_tcpserver.h>
#include <iostream>

int main() {
  using namespace http;

  TcpServer server = TcpServer("127.0.0.1", 8000);
  server.startListen();
  std::cout << "goyda";

  return 0;
}
