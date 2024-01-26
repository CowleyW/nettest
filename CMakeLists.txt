cmake_minimum_required(VERSION 3.21)

project(nettest CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-std=c++17 ${CMAKE_CXX_FLAGS}")

add_subdirectory(lib/fmt)

# -------------------------------
# Common Code
# -------------------------------
add_library(common
  src/common/message.h src/common/message.cpp
  src/common/message_queue.h src/common/message_queue.cpp
  src/common/connection.h src/common/connection.cpp
)

target_include_directories(common PRIVATE
  lib/fmt/include
  lib/asio/asio/include
)

target_link_libraries(common PRIVATE fmt)

set_target_properties(common PROPERTIES LINKER_LANGUAGE CXX)

# -------------------------------
# Server Code
# -------------------------------
add_executable(server
  src/server/server.cpp
)

target_include_directories(server PRIVATE 
  src/
  lib/fmt/include
  lib/asio/asio/include
)

target_link_libraries(server PRIVATE 
  common
  fmt
  ws2_32
  wsock32
)

target_compile_definitions(server PRIVATE _WIN32_WINNT=0x0A00)

# -------------------------------
# Client Code
# -------------------------------
add_executable(client
  src/client/client.cpp
)

target_include_directories(client PRIVATE 
  src/
  lib/fmt/include
  lib/asio/asio/include
)

target_link_libraries(client PRIVATE 
  common
  fmt
  ws2_32
)

target_compile_definitions(client PRIVATE _WIN32_WINNT=0x0A00)
