#include "common/connection.h"
#include "common/message.h"

#include "asio/error_code.hpp"
#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"
#include "asio/write.hpp"
#include "fmt/base.h"
#include "fmt/format.h"

#include <asio.hpp>
#include <fmt/core.h>

#include <memory>
#include <vector>

/**
 * A class representing a TCP Server
 */
class Server {
public:
  Server(asio::io_context &context)
      : context(context),
        acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 80)),
        connections(), limbos() {
    fmt::println("Creating the server.");
    start_accept();
  }

  void run() {
    while (true) {
      handle_limbos();
      handle_connections();
    }
  }

private:
  void start_accept() {
    fmt::println("Waiting for a connection.");

    std::shared_ptr<Connection> connection =
        std::make_shared<Connection>(this->context);

    auto on_accept = [=](const asio::error_code &err) {
      if (!err) {
        fmt::println("Good connection attempt");
        this->handle_accept(connection);
      } else {
        fmt::println("[ERROR]: {}", err.message());
      }
    };
    this->acceptor.async_accept(connection->socket, on_accept);
  }

  void handle_accept(std::shared_ptr<Connection> connection) {
    this->limbos.push_back(connection);
    connection->begin_queue();

    fmt::println("Connected [{}]  -- In Limbo [{}]", this->connections.size(),
                 this->limbos.size());

    Message message =
        Message::create(0, "SERVER", "Enter your name to continue.");
    connection->write_message(message);

    this->start_accept();
  }

  void write_all(Message &message) {
    for (auto connection : this->connections) {
      connection->write_message(message);
    }
  }

  void handle_limbos() {
    for (int i = this->limbos.size() - 1; i >= 0; i -= 1) {
      std::shared_ptr<Connection> user = this->limbos[i];
      if (user->has_message()) {
        Message message = user->get_message();

        if (message.header.type == 1) {
          this->connections.push_back(user);
          this->limbos.erase(this->limbos.begin() + i);

          std::string notif =
              fmt::format("User {} joined the server.", message.name);
          fmt::println(notif);

          Message broadcast = Message::create(2, "SERVER", notif);
          this->write_all(broadcast);
        }
      }
    }
  }

  void handle_connections() {
    for (auto connection : this->connections) {
      if (connection->has_message()) {
        Message message = connection->get_message();

        if (message.header.type == 2) {
          this->write_all(message);
        }
      }
    }
  }

private:
  asio::io_context &context;
  asio::ip::tcp::acceptor acceptor;

  std::vector<std::shared_ptr<Connection>> connections;
  std::vector<std::shared_ptr<Connection>> limbos;
};

/**
 * The server entry point
 */
int main(int argc, char **argv) {
  asio::io_context context;
  Server server(context);

  asio::io_context::work idle_work(context);
  std::thread context_thread = std::thread([&]() { context.run(); });

  server.run();

  fmt::println("Exiting.");
}
