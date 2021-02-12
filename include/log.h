#ifndef LOG_H
#define LOG_H

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

class Log {
public:
    static void init(std::string client_name, spdlog::level::level_enum level);
    static void init(std::string client_name, spdlog::level::level_enum core_level, spdlog::level::level_enum client_level);

    inline static std::shared_ptr<spdlog::logger> &getCoreLogger() {
        return Log::coreLogger;
    }

    inline static std::shared_ptr<spdlog::logger> &getClientLogger() {
        return Log::clientLogger;
    }

private:
    static std::shared_ptr<spdlog::logger> coreLogger;
    static std::shared_ptr<spdlog::logger> clientLogger;
};

#endif // LOG_H
