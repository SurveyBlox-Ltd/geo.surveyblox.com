#pragma once

namespace Server {
  inline int fileDescriptor = -1;
  inline int socket = -1;

  void create() noexcept;
  void listen() noexcept;
} // namespace Server