//
// Created by root on 5/24/24.
//

#include "misc/Logger.hpp"

Logger* Logger::logger = nullptr;

//
//Logger& Logger::getInstance(){
//    if (logger== nullptr){
//        logger = new Logger();
//    }
//    return *logger;
//}
//
//Logger::~Logger() {
//    if (fileStream.is_open()) {
//        fileStream.close();
//    }
//}
//
//Logger& Logger::setStream(std::ostream& os) {
//    outputStream = &os;
//    return *this;
//}
//
//template <typename... Args>
//void Logger::log(LogLevel level, Args... args) {
//    if (logger== nullptr){
//        logger = new Logger();
//    }
//    if (LOG_ENABLED){
//        if (level <= logger->logLevel) {
//            std::stringstream ss;
//            (ss << ... << args);
//            *logger->outputStream << logger->logEscape.at(level) << ss.str() << std::endl;
//        }
//    }
//}
