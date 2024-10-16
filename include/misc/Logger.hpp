//
// Created by root on 5/24/24.
//

#ifndef LOGGER_H
#define LOGGER_H

#define LOG_ENABLED 1
#define VERBOSE_LOGGING 1

#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <string>
#include <libgen.h>

// Define log levels
enum class LogLevel {
    None = 0,
    Error,
    Warning,
    Info,
    Debug,
    Trace,
};

class Logger {
private:
    static Logger* logger;
    LogLevel logLevel;
    std::ofstream fileStream;
    std::ostream* outputStream;
    Logger() : logLevel(LogLevel::Error), fileStream("db.log"), outputStream(&std::cerr) {}
    std::map<LogLevel, std::string> logEscape = {
            {LogLevel::Error, "\033[31m"},
            {LogLevel::Warning, "\033[33m"},
            {LogLevel::Debug, "\033[36m"},
            {LogLevel::Info, "\033[37m"},
            {LogLevel::Trace, "\033[90m"}
    };
public:

    static Logger& getInstance(){
        if (logger== nullptr){
            logger = new Logger();
        }
        return *logger;
    }

    ~Logger() {
        if (fileStream.is_open()) {
            fileStream.close();
        }
    }

    Logger& setStream(std::ostream& os) {
        outputStream = &os;
        return *this;
    }

    static void setLevel(LogLevel aLevel) {
        getInstance().logLevel = aLevel;
    }

    template <typename... Args>
    static void log(LogLevel level, Args... args) {
        if (logger== nullptr){
            logger = new Logger();
        }
        if (LOG_ENABLED){
            if (level <= logger->logLevel) {
                std::stringstream ss;
                (ss << ... << args);
                *logger->outputStream << logger->logEscape.at(level) << ss.str() << std::endl;
            }
        }
    }
};


#if VERBOSE_LOGGING
//

constexpr std::string_view get_basename(std::string_view path) {
    size_t last_slash = path.find_last_of("/\\");
    return (last_slash == std::string_view::npos) ? path : path.substr(last_slash + 1);
}

// Macro to simplify usage
#define BASENAME get_basename(__FILE__).data()

#define ERROR(...) Logger::log(LogLevel::Error, BASENAME, ":", __LINE__, " ", __VA_ARGS__);
#define WARN(...) Logger::log(LogLevel::Warning, BASENAME, ":", __LINE__, " ", __VA_ARGS__);
#define INFO(...) Logger::log(LogLevel::Info, BASENAME, ":", __LINE__, " ", __VA_ARGS__);
#define DEBUG(...) Logger::log(LogLevel::Debug, BASENAME, ":", __LINE__, " ", __VA_ARGS__);
#define TRACE(...) Logger::log(LogLevel::Trace, BASENAME, ":", __LINE__, " ", __VA_ARGS__);

#else

#define ERROR(...) Logger::log(LogLevel::Error, __VA_ARGS__);
#define WARN(...) Logger::log(LogLevel::Warning, __VA_ARGS__);
#define INFO(...) Logger::log(LogLevel::Info, __VA_ARGS__);
#define DEBUG(...) Logger::log(LogLevel::Debug, __VA_ARGS__);
#define TRACE(...) Logger::log(LogLevel::Trace, __VA_ARGS__);

#endif

#endif