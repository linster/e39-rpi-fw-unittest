//
// Created by stefan on 10/16/23.
//

#include "DummyLogger.h"

void DummyLogger::print(pico::logger::BaseLogger::Level level, std::string tag, std::string message) {

    //enum class Level { DEBUG, INFO, WARN, ERROR, WTF}
    std::string levelString;
    switch (level) {
        case Level::DEBUG : { levelString = "DEBUG" ; break; }
        case Level::INFO : { levelString = "INFO" ; break; }
        case Level::WARN : { levelString = "WARN" ; break; }
        case Level::ERROR : { levelString = "ERROR" ; break; }
        case Level::WTF : { levelString = "WTF" ; break; }
        default: { levelString = "" ; break; }
    }

    //absolute_time_t now = get_absolute_time();
    //TODO use the RTC to get a human-readable log-line datetime here.

    printf("[%s]    [%s]:[%s] \n", levelString.c_str(), tag.c_str(), message.c_str());

}
