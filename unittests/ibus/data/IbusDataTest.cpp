//
// Created by stefan on 10/20/23.
//

//Test all the constructors
//Test all the getters
//Make sure the data part returns the right length

//Test with valgrind that there's no destructor leaks.

#include <cstddef>
#include "ibus/data/IbusPacket.h"
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

using namespace pico::ibus::data;

class IbusPacketMemoryLeakTests : public ::testing::Test {
public:
    std::vector<uint8_t> ignitionKeyPositionTwoVector() {
        //https://github.com/piersholt/wilhelm-docs/blob/master/ike/11.md
        //80 04 BF 11 00 2A   # KL-30  -- position 0
        //80 04 BF 11 01 2B   # KL-R   -- position 1
        //80 04 BF 11 03 29   # KL-15  -- position 2
        //80 04 BF 11 07 2D   # KL-50  -- position 3
        std::vector<uint8_t> ret = std::vector<uint8_t>();
        ret.push_back(0x80);
        ret.push_back(0x04);
        ret.push_back(0xBF);
        ret.push_back(0x11);
        ret.push_back(0x01);
        ret.push_back(0x2B);
        return ret;
    }
    std::array<uint8_t, 255> ignitionKeyPositionTwoArray() {
        std::array<uint8_t, 255> array = std::array<uint8_t, 255>();
        array.fill(0);
        array[0] = 0x80;
        array[1] = 0x04;
        array[2] = 0xBF;
        array[3] = 0x11;
        array[4] = 0x01;
        array[5] = 0x2B;
        return array;
    }
};

TEST_F(IbusPacketMemoryLeakTests, ConstructWithVectorThenDelete) {
    //Run with valgrind memcheck in CLion
    auto* packet = new pico::ibus::data::IbusPacket(ignitionKeyPositionTwoVector());
    delete packet;
}

TEST_F(IbusPacketMemoryLeakTests, ConstructWithArrayThenDelete) {
    //Run with valgrind memcheck in CLion
    auto* packet = new pico::ibus::data::IbusPacket(ignitionKeyPositionTwoArray());
    delete packet;
}

TEST_F(IbusPacketMemoryLeakTests, ConstructWithBuilderThenDelete) {
    //Run with valgrind memcheck in CLion

    std::vector<uint8_t> data = std::vector<uint8_t>();
    data.push_back(0x11);
    data.push_back(0x01);

    auto* packet = new pico::ibus::data::IbusPacket(
            pico::ibus::data::IbusDeviceEnum::IKE,
            pico::ibus::data::IbusDeviceEnum::BROADCAST_BF,
            data
            );
    delete packet;
}

TEST_F(IbusPacketMemoryLeakTests, TestArrayConstructorGivesSameAsVector) {
    auto vecPacket = std::make_shared<IbusPacket>(ignitionKeyPositionTwoVector());
    auto arrayPacket = std::make_shared<IbusPacket>(ignitionKeyPositionTwoArray());

    EXPECT_EQ(*vecPacket->getData(), *arrayPacket->getData());
    EXPECT_EQ(vecPacket->getRawPacket(), arrayPacket->getRawPacket());
}

TEST_F(IbusPacketMemoryLeakTests, TestBuilderHasValidChecksum) {

    //80 04 BF 11 01 2B   # KL-R   -- position 1
    std::vector<uint8_t> data = std::vector<uint8_t>();
    data.push_back(0x11);
    data.push_back(0x01);

    auto packet = pico::ibus::data::IbusPacket(
            pico::ibus::data::IbusDeviceEnum::IKE,
            pico::ibus::data::IbusDeviceEnum::BROADCAST_BF,
            data
    );

    ASSERT_TRUE(packet.isPacketValid());
    ASSERT_EQ((uint8_t)0x2B, packet.getActualCrc());
}