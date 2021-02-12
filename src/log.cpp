#include <spdlog/sinks/stdout_color_sinks.h>

#include "log.h"

std::shared_ptr<spdlog::logger> Log::coreLogger;
std::shared_ptr<spdlog::logger> Log::clientLogger;

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
  Log::coreLogger = spdlog::stdout_color_mt("ENGINE");
  Log::coreLogger->set_level(core_level);

  Log::clientLogger = spdlog::stdout_color_mt(client_name);
  Log::clientLogger->set_level(client_level);
}
