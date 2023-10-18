//
// Created by stefan on 10/16/23.
//

#ifndef E39_RPI_FW_UNITTEST_DUMMYLOGGER_H
#define E39_RPI_FW_UNITTEST_DUMMYLOGGER_H

#include "logging/BaseLogger.h"

class DummyLogger : public pico::logger::BaseLogger {
public:
    void d(std::string tag, std::string message) override;
    void e(std::string tag, std::string message) override;
    void i(std::string tag, std::string message) override;
    void w(std::string tag, std::string message) override;
    void wtf(std::string tag, std::string message) override;
protected:
    void print(pico::logger::BaseLogger::Level level, std::string tag,
               std::string message) override;
};


#endif //E39_RPI_FW_UNITTEST_DUMMYLOGGER_H
