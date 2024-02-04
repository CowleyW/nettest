#include "asio/io_context.hpp"
#include "fmt/base.h"
#include <asio.hpp>

#include <cstdint>
#include <system_error>

class Server {
public:
  Server()
      : context(std::make_shared<asio::io_context>()),
        socket(*context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 80)),
        buf(128) {}

  void begin() {
    asio::io_context::work idle_work(*this->context);
    this->context_thread = std::thread([this]() { this->context->run(); });

    this->start_receive();
  }

private:
  void start_receive() {
    auto receive_callback = [this](const std::error_code &err, uint64_t size) {
      this->handle_receive(err, size);

      this->start_receive();
    };

    this->socket.async_receive_from(asio::buffer(this->buf), this->endpoint,
                                    receive_callback);
  }

  void handle_receive(const std::error_code &err, uint64_t size) {
    if (err) {
      fmt::println("error: {}", err.message());
      return;
    }

    std::string message(this->buf.begin(), this->buf.begin() + size);
    fmt::println("message ({}): {}", size, message);
    fmt::println("endpoint: {}:{}", this->endpoint.address().to_string(),
                 this->endpoint.port());

    // Respond
    this->respond("Hello, Client!");
  }

  void respond(const std::string &message) {
    std::shared_ptr<std::string> response =
        std::make_shared<std::string>("Hello, Client!");

    auto send_callback = [this](const std::error_code &err, uint64_t size) {
      this->handle_send(err, size);
    };

    this->socket.async_send_to(asio::buffer(*response), this->endpoint,
                               send_callback);
  }

  void handle_send(const std::error_code &err, uint64_t size) {}

private:
  std::shared_ptr<asio::io_context> context;
  asio::ip::udp::socket socket;
  asio::ip::udp::endpoint endpoint;

  std::thread context_thread;

  std::vector<char> buf;
};

int main(int argc, char **argv) {
  Server server;

  server.begin();
  while (true) {
    /*...*/
  }
}
