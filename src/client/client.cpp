#include "common/connection.h"
#include "common/message.h"
#include "common/message_queue.h"
#include "fmt/base.h"

#include <asio.hpp>
#include <fmt/core.h>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

void run() {
  asio::io_context io_context;
  asio::ip::tcp::resolver resolver(io_context);
  asio::ip::tcp::resolver::results_type endpoints =
      resolver.resolve("127.0.0.1", "80");

  asio::io_context::work idle_work(io_context);
  std::thread context_thread = std::thread([&]() { io_context.run(); });

  std::shared_ptr<Connection> connection =
      std::make_shared<Connection>(io_context);
  asio::connect(connection->socket, endpoints);

  connection->begin_queue();
  std::string name;

  while (true) {
    if (connection->has_message()) {
      Message message = connection->get_message();

      fmt::println("[{}]: {}", message.name, message.body);

      if (message.header.type == 0) {
        fmt::print("> ");
        std::getline(std::cin, name);

        Message response = Message::create(1, name, "");
        connection->write_message(response);
      } else if (message.header.type == 2) {
        std::string input;
        fmt::print("> ");
        std::getline(std::cin, input);

        Message response = Message::create(2, name, input);
        connection->write_message(response);
      }
    }
  }
}

int main(int argc, char **argv) { run(); }
