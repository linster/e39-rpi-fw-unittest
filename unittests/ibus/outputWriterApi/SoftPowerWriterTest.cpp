//
// Created by stefan on 10/16/23.
//
#include "ibus/outputWriterApi/PicoToPi/picoPiSoftPowerRequest/SoftPowerRequestWriter.h"
#include <gtest/gtest.h>

class SoftPowerWriterTest : public ::testing::Test {
public:
    std::vector<uint8_t> restartX() {
        std::vector<uint8_t> ret = std::vector<uint8_t>();
//        DEBUG : PicoCommsWindow / Sending message: PicoToPi RestartX bytes: [31, 4, 81, 8, 4, b8]
        ret.push_back(0x31);
        ret.push_back(0x04);
        ret.push_back(0x81);
        ret.push_back(0x08);
        ret.push_back(0x04);
        ret.push_back(0x08);
        return ret;
    }
    std::vector<uint8_t> restartPi() {
        std::vector<uint8_t> ret = std::vector<uint8_t>();
//        DEBUG : PicoCommsWindow / Sending message: PicoToPi RestartPi bytes: [31, 4, 81, 8, 5, b9]
        ret.push_back(0x31);
        ret.push_back(0x04);
        ret.push_back(0x81);
        ret.push_back(0x08);
        ret.push_back(0x05);
        ret.push_back(0x09);
        return ret;
    }
};