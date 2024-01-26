#include "message_queue.h"

#include "asio/buffer.hpp"
#include "asio/registered_buffer.hpp"
#include "fmt/base.h"
#include "message.h"

#include <cstddef>
#include <queue>
#include <system_error>
#include <vector>

#include <asio.hpp>

MessageQueue::MessageQueue(asio::ip::tcp::socket &socket)
    : socket(socket), queue(), buf(1024) {}

void MessageQueue::begin() { this->begin_enqueue_next(); }

bool MessageQueue::is_empty() { return this->queue.size() == 0; }

Message MessageQueue::dequeue() {
  Message front = this->queue.front();
  this->queue.pop();
  return front;
}

void MessageQueue::begin_enqueue_next() {
  auto read_callback = [this](std::error_code err, size_t len) {
    fmt::println("received some data: {} bytes.", len);
    if (err) {
      fmt::println("[ERROR]: {}", err.message());
    }

    // TODO: handle case where less than 8 bytes read somehow
    std::vector<char> headerData(this->buf.begin(),
                                 this->buf.begin() + sizeof(MessageHeader));
    MessageHeader header = MessageHeader::deserialize(headerData);

    if (len < header.full_message_size()) {
      fmt::println("lets handle this case later...");
      // finish_enqueue_next(header, len);
    } else {
      std::vector<char> message_data(this->buf.begin(),
                                     this->buf.begin() + len);
      this->queue.push(Message::deserialize(message_data));

      this->begin_enqueue_next();
    }
  };

  this->socket.async_read_some(asio::buffer(this->buf.data(), this->buf.size()),
                               read_callback);
}

void MessageQueue::finish_enqueue_next(MessageHeader header, size_t len) {}
