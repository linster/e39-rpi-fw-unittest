//
// Created by stefan on 10/13/23.
//

#include "../../../../e39-rpi-fw/ibus/observerRegistry/observers/PiToPico/picoConfigurationReadRequest/PicoConfigurationReadRequestObserver.h"
#include <gtest/gtest.h>


class PicoConfigReadRequestObserverTest : public ::testing::Test {

public:
    std::vector<uint8_t> getConfigStatusRequestRawBytes() {
//        DEBUG : SerialPublisherService / Writing message to serial port: IBusMessage( sourceDevice=PI, destDevice=PICO, data=[8, 2]
//        DEBUG : PicoCommsWindow / Sending message: ConfigStatusRequest bytes: [81, 4, 31, 8, 2, be]
        std::vector<uint8_t> ret = std::vector<uint8_t>();
        ret.push_back(0x81);
        ret.push_back(0x04);
        ret.push_back(0x31);
        ret.push_back(0x08);
        ret.push_back(0x02);
        ret.push_back(0xBE);
        return ret;
    }
};