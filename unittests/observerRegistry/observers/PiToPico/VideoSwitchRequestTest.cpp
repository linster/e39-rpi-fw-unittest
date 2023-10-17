//
// Created by stefan on 10/13/23.
//
#include "../../../../e39-rpi-fw/ibus/observerRegistry/observers/PiToPico/VideoRequest/PicoVideoRequestObserver.h"
#include <gtest/gtest.h>


class PicoVideoSwitchRequestObserverTest : public ::testing::Test {

public:
    std::vector<uint8_t> upstream() {
//        DEBUG : SerialPublisherService / Writing message to serial port: IBusMessage( sourceDevice=PI, destDevice=PICO, data=[8, 4]
//        DEBUG : PicoCommsWindow / Sending message: PicoVideoRequestUpstream bytes: [81, 4, 31, 8, 4, b8]
        std::vector<uint8_t> ret = std::vector<uint8_t>();
        ret.push_back(0x81);
        ret.push_back(0x04);
        ret.push_back(0x31);
        ret.push_back(0x08);
        ret.push_back(0x04);
        ret.push_back(0xB8);
        return ret;
    }

    std::vector<uint8_t> pico() {
//        DEBUG : SerialPublisherService / Writing message to serial port: IBusMessage( sourceDevice=PI, destDevice=PICO, data=[8, 5]
//        DEBUG : PicoCommsWindow / Sending message: PicoVideoRequestPico bytes: [81, 4, 31, 8, 5, b9]
        std::vector<uint8_t> ret = std::vector<uint8_t>();
        ret.push_back(0x81);
        ret.push_back(0x04);
        ret.push_back(0x31);
        ret.push_back(0x08);
        ret.push_back(0x05);
        ret.push_back(0xB9);
        return ret;
    }

    std::vector<uint8_t> rpi() {
//        DEBUG : SerialPublisherService / Writing message to serial port: IBusMessage( sourceDevice=PI, destDevice=PICO, data=[8, 6]
//        DEBUG : PicoCommsWindow / Sending message: PicoVideoRequestRpi bytes: [81, 4, 31, 8, 6, ba]
        std::vector<uint8_t> ret = std::vector<uint8_t>();
        ret.push_back(0x81);
        ret.push_back(0x04);
        ret.push_back(0x31);
        ret.push_back(0x08);
        ret.push_back(0x06);
        ret.push_back(0xBA);
        return ret;
    }

    std::vector<uint8_t> rvc() {
//        DEBUG : SerialPublisherService / Writing message to serial port: IBusMessage( sourceDevice=PI, destDevice=PICO, data=[8, 7]
//        DEBUG : PicoCommsWindow / Sending message: PicoVideoRequestRVC bytes: [81, 4, 31, 8, 7, bb]
        std::vector<uint8_t> ret = std::vector<uint8_t>();
        ret.push_back(0x81);
        ret.push_back(0x04);
        ret.push_back(0x31);
        ret.push_back(0x08);
        ret.push_back(0x07);
        ret.push_back(0xBB);
        return ret;
    }
};