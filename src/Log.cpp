#include <source_location>
#include <string>
#include "Log.h"
#include <iostream>

namespace Log {
  void error(const std::string& message, const std::source_location& location) noexcept {
    std::cerr << location.file_name() << " (" << std::to_string(location.line()) << ":" << std::to_string(location.column()) << ") at " << location.function_name() << " got " << message << std::endl;
  }
}