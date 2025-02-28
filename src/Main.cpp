#include "Server.h"
#include <stdlib.h>
#include <string>
#include <iostream>

int main() {
  Server::create();
  Server::listen();
  return EXIT_SUCCESS;
}