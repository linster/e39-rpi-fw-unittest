//
// Created by stefan on 10/16/23.
//

#ifndef E39_RPI_FW_UNITTEST_DUMMYLOGGER_H
#define E39_RPI_FW_UNITTEST_DUMMYLOGGER_H

#include "../e39-rpi-fw/logging/BaseLogger.h"

class DummyLogger : public pico::logger::BaseLogger {

protected:
    void print(pico::logger::BaseLogger::Level level, std::string tag,
               std::string message) override;
};


#endif //E39_RPI_FW_UNITTEST_DUMMYLOGGER_H
