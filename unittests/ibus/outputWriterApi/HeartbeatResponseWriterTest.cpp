//
// Created by stefan on 10/19/23.
//

#include "DummyLogger.h"
#include "ibus/outputWriterApi/PicoToPi/heartbeatResponseWriter/HeartbeatResponseWriter.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockDmaManager.h"

class HeartbeatResponseWriterTest : public ::testing::Test {
public:
    std::vector<uint8_t> picoToPiHeartbeatResponse() {
//        DEBUG : PicoCommsWindow / Sending message: PicoToPi HeartbeatResponse bytes: [31, 4, 81, 8, 1, bd]
        std::vector<uint8_t> ret = std::vector<uint8_t>();
        ret.push_back(0x31);
        ret.push_back(0x04);
        ret.push_back(0x81);
        ret.push_back(0x08);
        ret.push_back(0x01);
        ret.push_back(0xBD);
        return ret;
    }
};

TEST_F(HeartbeatResponseWriterTest, PicoToPiHeartbeatResponse) {
    std::shared_ptr<pico::logger::BaseLogger> logger = std::make_shared<DummyLogger>(DummyLogger());

    MockDmaManager mockDmaManager = MockDmaManager();
    std::shared_ptr<pico::ibus::dma::IDmaManager> iDmaManagerPtr = std::make_shared<MockDmaManager>(mockDmaManager);


    pico::ibus::output::writer::HeartbeatResponseWriter writer = pico::ibus::output::writer::HeartbeatResponseWriter(
            logger,
            iDmaManagerPtr
            );

    writer.sendHeartbeatResponse();

    EXPECT_EQ(pico::ibus::data::IbusDeviceEnum::RPI,
              std::dynamic_pointer_cast<MockDmaManager>(iDmaManagerPtr)->writtenPacketToPi.getDestinationDevice());
    EXPECT_EQ(pico::ibus::data::IbusDeviceEnum::PICO,
              std::dynamic_pointer_cast<MockDmaManager>(iDmaManagerPtr)->writtenPacketToPi.getSourceDevice());

    std::vector<uint8_t> actualDataVector =
            *std::dynamic_pointer_cast<MockDmaManager>(iDmaManagerPtr)->writtenPacketToPi.getData();

    EXPECT_THAT(actualDataVector, testing::ElementsAre(0x08, 0x01));

    std::vector<uint8_t> actualRawPacket =
            std::dynamic_pointer_cast<MockDmaManager>(iDmaManagerPtr)->writtenPacketToPi.getRawPacket();

    EXPECT_THAT(actualRawPacket, testing::ContainerEq(picoToPiHeartbeatResponse()));
}