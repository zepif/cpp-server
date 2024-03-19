#include <cstdio>
#include <http_tcpserver.h>
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

namespace {
const int BUFFER_SIZE = 30720;

void log(const std::string &message) { std::cout << message << "\n"; }

void exitWithError(const std::string &errorMessage) {
  std::cout << WSAGetLastError() << "\n";
  log("ERROR : " + errorMessage);
  exit(1);
}
} // namespace

namespace http {
TcpServer::TcpServer(std::string ip_address, int port)
    : m_ip_address(ip_address), m_port(port), m_socket(), m_new_socket(),
      m_incomingMessage(), m_socketAddress(),
      m_socketAddress_len(sizeof(m_socketAddress)),
      m_serverMessage("Server has started..."), m_wsaData() {
  m_socketAddress.sin_family = AF_INET;
  m_socketAddress.sin_port = htons(m_port);
  m_socketAddress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());

  if (startServer() != 0) {
    std::ostringstream ss;
    ss << "Failed to start server with PORT: "
       << htons(m_socketAddress.sin_port);
    log(ss.str());
  }
}

TcpServer::~TcpServer() { closeServer(); }

int TcpServer::startServer() {
  if (WSAStartup(MAKEWORD(2, 0), &m_wsaData) != 0) {
    exitWithError("WSAStartup failed");
  }

  m_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (m_socket < 0) {
    exitWithError("cannot create a socket");
    return 1;
  }

  if (bind(m_socket, (sockaddr *)&m_socketAddress, m_socketAddress_len) < 0) {
    exitWithError("Cannot connect a socket to address");
    return 1;
  }

  return 0;
}

void TcpServer::closeServer() {
  closesocket(m_socket);
  closesocket(m_new_socket);
  WSACleanup();
  exit(0);
}

void TcpServer::startListen() {
  if (listen(m_socket, 20) < 0) {
    exitWithError("Socket listen failed");
  }

  std::ostringstream ss;
  ss << "\n*** Listening on ADDRESS: " << inet_ntoa(m_socketAddress.sin_addr)
     << " PORT: " << ntohs(m_socketAddress.sin_port) << " ***\n\n";
  log(ss.str());

  int bytesReceived;
  while (true) {
    log("====== Waiting for a new connection ======\n\n\n");
    acceptConnection(m_new_socket);

    char buffer[BUFFER_SIZE] = {0};
    bytesReceived = recv(m_new_socket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived < 0) {
      exitWithError("Failed to receive bytes from client socket connection");
    }

    std::ostringstream ss;
    ss << "------ Received message from client ------\n" << buffer;
    log(ss.str());

    if (send(m_new_socket, m_serverMessage.c_str(), sizeof(m_serverMessage),
             0) < 0) {
      log("Error sending response to client");
    } else {
      log("------ Server response sent to client ------\n" + m_serverMessage);
    }

    closesocket(m_new_socket);
  }
}

void TcpServer::acceptConnection(SOCKET &new_socket) {
  new_socket =
      accept(m_socket, (sockaddr *)&m_socketAddress, &m_socketAddress_len);
  if (new_socket < 0) {
    std::ostringstream ss;
    ss << "Server failed to accept incoming connection from ADDRESS: "
       << inet_ntoa(m_socketAddress.sin_addr)
       << "; PORT: " << ntohs(m_socketAddress.sin_port);
    exitWithError(ss.str());
  }
}

} // namespace http
