#pragma once

#include <string>
#include <vector>

struct MessageHeader {
  int name_bytes;
  int body_bytes;

  // Message Types:
  // 0 -- Join Notification
  // 1 -- Set Display name
  // 2 -- Send chat
  int type;

  std::vector<char> serialize();

  int full_message_size();

  static MessageHeader deserialize(std::vector<char> bytes);
};

struct Message {
  MessageHeader header;
  std::string name;
  std::string body;

  std::vector<char> serialize();

  static Message deserialize(std::vector<char> bytes);
  static Message create(int type, std::string name, std::string body);
};
