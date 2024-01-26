#include "client.h"
#include "fmt/base.h"
#include "server.h"

#include <asio.hpp>
#include <fmt/core.h>

#include <cstring>

int main(int argc, char **argv) {
  if (argc != 2) {
    fmt::println("USAGE: nettest <server | client>");
    return 1;
  }

  if (std::strcmp(argv[1], "server") == 0) {
    fmt::println("Creating a server.");
    Server::run();
  } else if (std::strcmp(argv[1], "client") == 0) {
    fmt::println("Creating a client.");
    Client::run();
  } else {
    fmt::println("USAGE: nettest <server | client>");
    return 1;
  }

  return 0;
}
