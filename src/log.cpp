#include "log.h"

std::shared_ptr<spdlog::logger> Log::core_logger;
std::shared_ptr<spdlog::logger> Log::client_logger;
std::vector<spdlog::sink_ptr> Log::sinks;

#ifdef DEBUG
  #define CORE_DEF_LVL spdlog::level::trace
#else // RELEASE
  #define CORE_DEF_LVL spdlog::level::warn
#endif // DEBUG

void Log::init(std::string client_name, spdlog::level::level_enum level) {
  Log::init(client_name, CORE_DEF_LVL, level);
}

void Log::init(std::string client_name, spdlog::level::level_enum core_level, spdlog::level::level_enum client_level) {
  // Log::sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  Log::sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/last.log"));
  spdlog::set_pattern("[%T] [%n] %^[%l]: %v%$");
  Log::core_logger = std::make_shared<spdlog::logger>("ENGINE", begin(Log::sinks), end(Log::sinks));
  Log::core_logger->set_level(core_level);

  Log::client_logger = std::make_shared<spdlog::logger>(client_name, begin(Log::sinks), end(Log::sinks));
  Log::client_logger->set_level(client_level);
}
