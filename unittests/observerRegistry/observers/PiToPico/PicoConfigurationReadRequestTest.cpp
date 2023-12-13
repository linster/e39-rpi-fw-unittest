//
// Created by stefan on 10/13/23.
//

#include "ibus/observerRegistry/observers/PiToPico/picoConfigurationReadRequest/PicoConfigurationReadRequestObserver.h"
#include <gtest/gtest.h>

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

class MockConfigurationStatusWriter : public pico::ibus::output::writer::ConfigurationStatusWriter {
    //Override
    std::shared_ptr<pico::logger::BaseLogger> logger = std::make_shared<DummyLogger>(DummyLogger());
public:
    //Mock this to do nothing
    MOCK_METHOD(std::shared_ptr<pico::ibus::dma::IDmaManager>, getDmaManager, (), (override));

    MOCK_METHOD(void, scheduleEmit, (pico::messages::ConfigMessage));

    MockConfigurationStatusWriter(
            std::shared_ptr<pico::logger::BaseLogger> logger,
            std::shared_ptr<pico::ibus::dma::IDmaManager> dmaManager
    ) : ConfigurationStatusWriter(logger, dmaManager) {}
};

TEST_F(PicoConfigReadRequestObserverTest, TestDecode) {
    std::shared_ptr<pico::logger::BaseLogger> logger = std::make_shared<DummyLogger>(DummyLogger());

    MockConfigurationStatusWriter mockConfigurationStatusWriter = MockConfigurationStatusWriter(
            logger,
            nullptr
            );



    pico::ibus::observers::PicoConfigurationReadRequestObserver requestObserver = pico::ibus::observers::PicoConfigurationReadRequestObserver(
            logger,
            std::make_shared<pico::ibus::output::writer::ConfigurationStatusWriter>(mockConfigurationStatusWriter),
            nullptr
            );

    requestObserver.dispatchPacket(
            logger,
            std::make_shared<pico::ibus::data::IbusPacket>(configRequest())
            );

    EXPECT_CALL(
            mockConfigurationStatusWriter,
            scheduleEmit(testing::_)
            );
}