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
    close(socket);
    Log::error(strerror(errno));
    std::exit(EXIT_FAILURE);
  }

  if (::listen(socket, 3) < 0) {
    close(socket);
    Log::error(strerror(errno));
    std::exit(EXIT_FAILURE);
  }

  std::cout << "Server listening on port 80" << std::endl;
}

void Server::listen() noexcept {
  while(true) {
    struct sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);

    int clientSocket = accept(socket, (struct sockaddr*)&clientAddress, &clientAddressSize);
    if (clientSocket == -1) {
      Log::error(strerror(errno));
      continue;
    }

    const char *clientIp = inet_ntoa(clientAddress.sin_addr);
    if (clientIp == NULL) {
      close(clientSocket);
      Log::error(strerror(errno));
      continue;
    }

    const std::string responseBody = "<html>\n<body>" + std::string(clientIp) + "</body>\n</html>";
    const std::string response =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/html; charset=UTF-8\r\n"
      "Content-Length: " + std::to_string(responseBody.size()) + "\r\n"
      "Connection: close\r\n"
      "Server: Buildinger\r\n"
      "\r\n" +
      responseBody;

    size_t bytesSent = send(clientSocket, response.c_str(), response.size(), 0);
    if (bytesSent < 0) {
      close(clientSocket);
      Log::error(strerror(errno));
      continue;
    }

    close(clientSocket);
  }
}