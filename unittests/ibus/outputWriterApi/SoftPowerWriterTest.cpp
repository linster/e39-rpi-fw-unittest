//
// Created by stefan on 10/16/23.
//
#include "ibus/outputWriterApi/PicoToPi/picoPiSoftPowerRequest/SoftPowerRequestWriter.h"
#include "DummyLogger.h"
#include "MockDmaManager.h"
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

class SoftPowerWriterTest : public ::testing::Test {
public:
    static std::vector<uint8_t> restartX() {
        std::vector<uint8_t> ret = std::vector<uint8_t>();

//        DEBUG : PicoCommsWindow / Sending message: PicoToPi RestartX bytes: [31, 6, 81, 8, 4, b8]
        ret.push_back(0x31);
        ret.push_back(0x06);
        ret.push_back(0x81);
        ret.push_back(0x08);
        ret.push_back(0x04);
        ret.push_back(0xB8);
        return ret;
    }
    static std::vector<uint8_t> restartPi() {
        std::vector<uint8_t> ret = std::vector<uint8_t>();
//        DEBUG : PicoCommsWindow / Sending message: PicoToPi RestartPi bytes: [31, 6, 81, 8, 5, 1a, 0, a1]
        ret.push_back(0x31);
        ret.push_back(0x06);
        ret.push_back(0x81);
        ret.push_back(0x08);
        ret.push_back(0x05);
        ret.push_back(0x1A);
        ret.push_back(0x00);
        ret.push_back(0xA1);
        return ret;
    }
};


TEST_F(SoftPowerWriterTest, RestartX) {

    std::shared_ptr<pico::logger::BaseLogger> logger = std::make_shared<DummyLogger>(DummyLogger());

    MockDmaManager mockDmaManager = MockDmaManager();
    std::shared_ptr<pico::ibus::dma::IDmaManager> iDmaManagerPtr = std::make_shared<MockDmaManager>(mockDmaManager);

    pico::ibus::output::writer::SoftPowerRequestWriter writer = pico::ibus::output::writer::SoftPowerRequestWriter(
            logger,
            iDmaManagerPtr
            );

    writer.requestRpiRestartX();

    EXPECT_EQ(pico::ibus::data::IbusDeviceEnum::RPI,
              std::dynamic_pointer_cast<MockDmaManager>(iDmaManagerPtr)->writtenPacketToPi.getDestinationDevice());
    EXPECT_EQ(pico::ibus::data::IbusDeviceEnum::PICO,
              std::dynamic_pointer_cast<MockDmaManager>(iDmaManagerPtr)->writtenPacketToPi.getSourceDevice());

    std::vector<uint8_t> actualRawPacket =
            std::dynamic_pointer_cast<MockDmaManager>(iDmaManagerPtr)->writtenPacketToPi.getRawPacket();

    //Don't actually test the complete byte, just try and parse it and see we set the right MessageType

    pico::ibus::data::IbusPacket sentPacket = pico::ibus::data::IbusPacket(actualRawPacket);

    std::string inputString = std::string(0, '\0');
    std::vector<uint8_t> data = *sentPacket.getData();
    for (const uint8_t item: data) {
        inputString.push_back(item);
    }
    auto inputStream = NanoPb::StringInputStream(std::make_unique<std::string>(inputString.c_str()));

    pico::messages::PicoToPiMessage decoded;
    if(!NanoPb::decode<pico::messages::PicoToPiMessageConverter>(inputStream, decoded)) {
        FAIL();
    }

    ASSERT_EQ(decoded.messageType, pico::messages::PicoToPiMessage::MessageType::PiSoftPowerRestartX);
}

TEST_F(SoftPowerWriterTest, RestartPi) {
    std::shared_ptr<pico::logger::BaseLogger> logger = std::make_shared<DummyLogger>(DummyLogger());

    MockDmaManager mockDmaManager = MockDmaManager();
    std::shared_ptr<pico::ibus::dma::IDmaManager> iDmaManagerPtr = std::make_shared<MockDmaManager>(mockDmaManager);

    pico::ibus::output::writer::SoftPowerRequestWriter writer = pico::ibus::output::writer::SoftPowerRequestWriter(
            logger,
            iDmaManagerPtr
    );

    writer.requestRpiRestart();

    EXPECT_EQ(pico::ibus::data::IbusDeviceEnum::RPI,
              std::dynamic_pointer_cast<MockDmaManager>(iDmaManagerPtr)->writtenPacketToPi.getDestinationDevice());
    EXPECT_EQ(pico::ibus::data::IbusDeviceEnum::PICO,
              std::dynamic_pointer_cast<MockDmaManager>(iDmaManagerPtr)->writtenPacketToPi.getSourceDevice());

    std::vector<uint8_t> actualDataVector =
            *std::dynamic_pointer_cast<MockDmaManager>(iDmaManagerPtr)->writtenPacketToPi.getData();

//    EXPECT_THAT(actualDataVector, testing::ElementsAre(0x08, 0x05, 0x1A, 0x00));

    std::vector<uint8_t> actualRawPacket =
            std::dynamic_pointer_cast<MockDmaManager>(iDmaManagerPtr)->writtenPacketToPi.getRawPacket();

    EXPECT_THAT(actualRawPacket, testing::ContainerEq(restartPi()));
}