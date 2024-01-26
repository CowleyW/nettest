#include "connection.h"

Connection::Connection(asio::io_context &context)
    : socket(context), queue(socket) {}

void Connection::write_message(Message &message) {
  fmt::println("Attempting to write a message.");

  auto write_callback = [&](const asio::error_code &err, size_t len) {
    this->handle_write(err, len);
  };
  asio::async_write(this->socket, asio::buffer(message.serialize()),
                    write_callback);
}

void Connection::handle_write(const asio::error_code &err, size_t len) {
  if (!err) {
    fmt::println("Wrote {} bytes.", len);
  } else {
    fmt::println("[ERROR]: {}", err.message());
  }
}

void Connection::begin_queue() { this->queue.begin(); }

bool Connection::has_message() { return !this->queue.is_empty(); }

Message Connection::get_message() { return this->queue.dequeue(); }
