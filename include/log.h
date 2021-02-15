#ifndef LOG_H
#define LOG_H

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

class Log {
public:
    static void init(std::string client_name, spdlog::level::level_enum level);
    static void init(std::string client_name, spdlog::level::level_enum core_level, spdlog::level::level_enum client_level);

    inline static std::shared_ptr<spdlog::logger> &get_core_logger() {
        return Log::core_logger;
    }

    inline static std::shared_ptr<spdlog::logger> &get_client_logger() {
        return Log::client_logger;
    }

private:
    static std::shared_ptr<spdlog::logger> core_logger;
    static std::shared_ptr<spdlog::logger> client_logger;
};

#endif // LOG_H
