#include "message.h"
#include "fmt/format.h"

#include <fmt/core.h>

#include <stdexcept>
#include <vector>

std::vector<char> MessageHeader::serialize() {
  std::vector<char> buf(sizeof(MessageHeader));
  buf[0] = this->name_bytes >> 24;
  buf[1] = this->name_bytes >> 16;
  buf[2] = this->name_bytes >> 8;
  buf[3] = this->name_bytes;

  buf[4] = this->body_bytes >> 24;
  buf[5] = this->body_bytes >> 16;
  buf[6] = this->body_bytes >> 8;
  buf[7] = this->body_bytes;

  buf[8] = this->type >> 24;
  buf[9] = this->type >> 16;
  buf[10] = this->type >> 8;
  buf[11] = this->type;

  return buf;
}

int MessageHeader::full_message_size() {
  return this->name_bytes + this->body_bytes + sizeof(MessageHeader);
}

MessageHeader MessageHeader::deserialize(std::vector<char> bytes) {
  if (bytes.size() != sizeof(MessageHeader)) {
    throw std::invalid_argument(
        fmt::format("Expected 12 bytes, got {}", bytes.size()));
  }

  int name_bytes = 0;
  name_bytes += bytes[0] << 24;
  name_bytes += bytes[1] << 16;
  name_bytes += bytes[2] << 8;
  name_bytes += bytes[3];

  int body_bytes = 0;
  body_bytes += bytes[4] << 24;
  body_bytes += bytes[5] << 16;
  body_bytes += bytes[6] << 8;
  body_bytes += bytes[7];

  int type = 0;
  type += bytes[8] << 24;
  type += bytes[9] << 16;
  type += bytes[10] << 8;
  type += bytes[11];

  return MessageHeader{name_bytes, body_bytes, type};
}

std::vector<char> Message::serialize() {
  std::vector<char> buf = this->header.serialize();
  std::vector<char> name =
      std::vector<char>(this->name.begin(), this->name.end());
  std::vector<char> body =
      std::vector<char>(this->body.begin(), this->body.end());

  buf.insert(buf.end(), name.begin(), name.end());
  buf.insert(buf.end(), body.begin(), body.end());

  return buf;
}

Message Message::deserialize(std::vector<char> bytes) {
  int hz = sizeof(MessageHeader);
  if (bytes.size() < hz) {
    throw std::invalid_argument(
        fmt::format("Expected at least 12 bytes, got {}."));
  }
  std::vector<char> headerData(bytes.begin(), bytes.begin() + hz);
  MessageHeader header = MessageHeader::deserialize(headerData);

  if (header.name_bytes + header.body_bytes + hz != bytes.size()) {
    throw std::invalid_argument("Header is malformed.");
  }

  std::string name(bytes.begin() + hz, bytes.begin() + hz + header.name_bytes);
  std::string body(bytes.begin() + hz + header.name_bytes, bytes.end());

  return {header, name, body};
}

Message Message::create(int type, std::string name, std::string body) {
  MessageHeader header = {(int)name.size(), (int)body.size(), type};

  return {header, name, body};
}
