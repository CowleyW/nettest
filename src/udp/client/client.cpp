#include "asio/buffer.hpp"
#include "fmt/base.h"
#include <asio.hpp>
#include <cstdint>

int main(int argc, char **argv) {
  asio::io_context context;

  asio::ip::udp::resolver resolver(context);
  asio::ip::udp::endpoint endpoint =
      *resolver.resolve("127.0.0.1", "80").begin();

  asio::ip::udp::socket socket(context);
  socket.open(asio::ip::udp::v4());

  std::string connect = "Hello, Server!";
  socket.send_to(asio::buffer(connect), endpoint);

  std::vector<char> buf(128);
  asio::ip::udp::endpoint sender;
  uint64_t len = socket.receive_from(asio::buffer(buf), sender);

  std::string message(buf.begin(), buf.begin() + len);
  fmt::println("Received: {}", message);
}
