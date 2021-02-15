#include <spdlog/sinks/stdout_color_sinks.h>

#include "log.h"

std::shared_ptr<spdlog::logger> Log::core_logger;
std::shared_ptr<spdlog::logger> Log::client_logger;

#ifdef DEBUG
  #define CORE_DEF_LVL spdlog::level::trace
#else // RELEASE
  #define CORE_DEF_LVL spdlog::level::warn
#endif // DEBUG

void Log::init(std::string client_name, spdlog::level::level_enum level) {
  Log::init(client_name, CORE_DEF_LVL, level);
}

void Log::init(std::string client_name, spdlog::level::level_enum core_level, spdlog::level::level_enum client_level) {
  spdlog::set_pattern("[%T] [%n] %^[%l]: %v%$");
  Log::core_logger = spdlog::stdout_color_mt("ENGINE");
  Log::core_logger->set_level(core_level);

  Log::client_logger = spdlog::stdout_color_mt(client_name);
  Log::client_logger->set_level(client_level);
}
