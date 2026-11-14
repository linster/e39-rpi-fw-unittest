//
// Created by stefan on 10/13/23.
//

#include "ibus/observerRegistry/observers/PiToPico/picoConfigurationReadRequest/PicoConfigurationReadRequestObserver.h"
#include <gtest/gtest.h>
#include "configuration/store/memory/MemoryConfigurationStore.h"
#include "configuration/default/PimoroniDefaultConfigurationProvider.h"
#include "hardware/flash.h"
#include "ibus/outputWriterApi/MockDmaManager.h"
#include "TestPacketUtils.h"

#include "../../../DummyLogger.h"
#include "ibus/observerRegistry/observers/PiToPico/picoConfigurationReadRequest/PicoConfigurationReadRequestObserver.h"
#include <ibus/outputWriterApi/PicoToPi/picoConfigurationStatusWriter/ConfigurationStatusWriter.h>
#include <configuration/ConfigurationManager.h>
#include <gmock/gmock.h>

class PicoConfigReadRequestObserverTest : public ::testing::Test {

public:
    std::vector<uint8_t> configRequest() {
//        DEBUG : SerialPublisherService / Writing message to serial port: IBusMessage( sourceDevice=PI, destDevice=PICO, data=[8, 2]
//        DEBUG : PicoCommsWindow / Sending message: ConfigStatusRequest bytes: [81, 4, 31, 8, 2, be]
//        Captured before d129c72 ("Shift enums up") added EmptyMessage = 0; type byte +1 for the current proto.
        std::vector<uint8_t> ret = std::vector<uint8_t>();
        ret.push_back(0x81);
        ret.push_back(0x04);
        ret.push_back(0x31);
        ret.push_back(0x08);
        ret.push_back(0x03);
        ret.push_back(0xBF);
        return ret;
    }
};

//The previous version mocked ConfigurationStatusWriter::scheduleEmit, but that method isn't
//virtual, the mock was sliced when copied into make_shared<ConfigurationStatusWriter>, the
//EXPECT_CALL came after the call, and configurationManager was null (the observer dereferences
//it). Use the real classes instead and check what reaches the Pi.
TEST_F(PicoConfigReadRequestObserverTest, TestDecode) {
    std::shared_ptr<pico::logger::BaseLogger> logger = std::make_shared<DummyLogger>();
    hoststub::resetHostFlash();

    auto dmaManager = std::make_shared<MockDmaManager>();
    auto statusWriter = std::make_shared<pico::ibus::output::writer::ConfigurationStatusWriter>(logger, dmaManager);
    auto configurationManager = std::make_shared<pico::config::ConfigurationManager>(
            std::make_shared<pico::config::FlashConfigurationStore>(logger),
            std::make_shared<pico::config::IBusConfigMessageStore>(logger, statusWriter),
            std::make_shared<pico::config::MemoryConfigurationStore>(),
            logger,
            std::make_shared<pico::config::defaults::PimoroniDefaultConfigurationProvider>());

    pico::ibus::observers::PicoConfigurationReadRequestObserver requestObserver(
            logger, statusWriter, configurationManager);

    requestObserver.dispatchPacket(
            logger,
            std::make_shared<pico::ibus::data::IbusPacket>(configRequest())
            );

    std::vector<uint8_t> sent = packetData(dmaManager->writtenPacketToPi);
    ASSERT_GE(sent.size(), 2u);
    EXPECT_EQ(sent[0], 0x08); // field 1 (messageType), varint
    EXPECT_EQ(sent[1], 0x04); // PicoToPi ConfigStatusResponse
    EXPECT_EQ(dmaManager->writtenPacketToPi.getDestinationDevice(), pico::ibus::data::IbusDeviceEnum::RPI);
}
