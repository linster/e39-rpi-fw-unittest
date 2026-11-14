//
// Created by stefan on 10/19/23.
//

#include "DummyLogger.h"
#include "TestPacketUtils.h"
#include "ibus/outputWriterApi/PicoToPi/heartbeatResponseWriter/HeartbeatResponseWriter.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockDmaManager.h"

class HeartbeatResponseWriterTest : public ::testing::Test {
public:
    std::vector<uint8_t> picoToPiHeartbeatResponse() {
//        DEBUG : PicoCommsWindow / Sending message: PicoToPi HeartbeatResponse bytes: [31, 4, 81, 8, 1, bd]
//        Captured before d129c72 ("Shift enums up") added EmptyMessage = 0; type byte +1 for the current proto.
        std::vector<uint8_t> ret = std::vector<uint8_t>();
        ret.push_back(0x31);
        ret.push_back(0x04);
        ret.push_back(0x81);
        ret.push_back(0x08);
        ret.push_back(0x02);
        ret.push_back(0xBE);
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
            packetData(std::dynamic_pointer_cast<MockDmaManager>(iDmaManagerPtr)->writtenPacketToPi);

    EXPECT_THAT(actualDataVector, testing::ElementsAre(0x08, 0x02)); // HeartbeatResponse = 2 since d129c72

    std::vector<uint8_t> actualRawPacket =
            std::dynamic_pointer_cast<MockDmaManager>(iDmaManagerPtr)->writtenPacketToPi.getRawPacket();

    EXPECT_THAT(actualRawPacket, testing::ContainerEq(picoToPiHeartbeatResponse()));
}