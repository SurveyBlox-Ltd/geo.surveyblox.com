#pragma once

#include <source_location>
#include <string>

namespace Log {
  void error(const std::string& message, const std::source_location& location = std::source_location::current()) noexcept;
}