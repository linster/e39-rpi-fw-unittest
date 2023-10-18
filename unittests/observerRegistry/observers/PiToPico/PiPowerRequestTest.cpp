//
// Created by stefan on 10/13/23.
//
#include "ibus/observerRegistry/observers/PiToPico/PiPowerRequest/PiPowerRequestObserver.h"
#include <gtest/gtest.h>


class PicoPowerRequestObserverTest : public ::testing::Test {

public:
    std::vector<uint8_t> powerOn() {
//        DEBUG : SerialPublisherService / Writing message to serial port: IBusMessage( sourceDevice=PI, destDevice=PICO, data=[8, 8]
//        DEBUG : PicoCommsWindow / Sending message: PicoPowerRequestOn bytes: [81, 4, 31, 8, 8, b4]
        std::vector<uint8_t> ret = std::vector<uint8_t>();
        ret.push_back(0x81);
        ret.push_back(0x04);
        ret.push_back(0x31);
        ret.push_back(0x08);
        ret.push_back(0x08);
        ret.push_back(0xB4);
        return ret;
    }
    std::vector<uint8_t> powerOff() {
//        DEBUG : SerialPublisherService / Writing message to serial port: IBusMessage( sourceDevice=PI, destDevice=PICO, data=[8, 9]
//        DEBUG : PicoCommsWindow / Sending message: PicoPowerRequestOff bytes: [81, 4, 31, 8, 9, b5]
        std::vector<uint8_t> ret = std::vector<uint8_t>();
        ret.push_back(0x81);
        ret.push_back(0x04);
        ret.push_back(0x31);
        ret.push_back(0x08);
        ret.push_back(0x09);
        ret.push_back(0xB5);
        return ret;
    }
};