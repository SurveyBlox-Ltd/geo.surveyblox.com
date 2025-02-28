#include <string>
#include <sys/socket.h>
#include <cstring>
#include <arpa/inet.h>
#include "Server.h"
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include "Log.h"

void Server::create() noexcept {
  struct sockaddr_in address;
  char buffer[1024] = {0};

  socket = ::socket(AF_INET, SOCK_STREAM, 0);
  if (socket == -1) {
    Log::error(strerror(errno));
    std::exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(80);

  if (bind(socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
    Log::error(strerror(errno));
    std::exit(EXIT_FAILURE);
  }

  if (::listen(socket, 3) < 0) {
    Log::error(strerror(errno));
    std::exit(EXIT_FAILURE);
  }

  std::cout << "Server listening on port 80" << std::endl;
}

void Server::listen() noexcept {
  while(true) {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    int clientSocket = accept(socket, NULL, NULL);
    if (clientSocket == -1) {
      Log::error(strerror(errno));
      close(clientSocket);
      continue;
    }

    const char *clientIp = inet_ntoa(clientAddr.sin_addr);
    if (clientIp == NULL) {
      Log::error(strerror(errno));
      close(clientSocket);
      continue;
    }

    const std::string ip = std::string(clientIp);

    const std::string response =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/html; charset=UTF-8\r\n"
      "Connection: close\r\n"
      "\r\n"
      "<html>\n"
      "<body>" + ip + "</body>\n"
      "</html>";

    size_t bytesSent = send(clientSocket, response.c_str(), response.size(), 0);
    if (bytesSent < 0) {
      Log::error(strerror(errno));
      close(clientSocket);
      continue;
    }

    close(clientSocket);
  }
}