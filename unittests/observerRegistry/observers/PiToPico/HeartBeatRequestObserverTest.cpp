//
// Created by stefan on 10/13/23.
//


#include "../../../DummyLogger.h"
#include "ibus/outputWriterApi/PicoToPi/heartbeatResponseWriter/HeartbeatResponseWriter.h"
#include "ibus/observerRegistry/observers/PiToPico/heartbeatRequest/HeatbeatRequestObserver.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

//using namespace pico::ibus::;

class HeartbeatRequestObserverTest : public ::testing::Test {

public:
    std::vector<uint8_t> getHeartbeatRequestRawBytes() {
//        DEBUG : SerialPublisherService / Writing message to serial port: IBusMessage( sourceDevice=PI, destDevice=PICO, data=[]
//        DEBUG : PicoCommsWindow / Sending message: HeartbeatRequest bytes: [81, 2, 31, b2]
        std::vector<uint8_t> ret = std::vector<uint8_t>();
        ret.push_back(0x81);
        ret.push_back(0x02);
        ret.push_back(0x31);
        ret.push_back(0xB2);
        return ret;
    }

    std::vector<uint8_t> getHeartbeatResponseRawBytes() {
//        DEBUG : SerialPublisherService / Writing message to serial port: IBusMessage( sourceDevice=PI, destDevice=PICO, data=[8, 1]
//        DEBUG : PicoCommsWindow / Sending message: HeartbeatResponse bytes: [81, 4, 31, 8, 1, bd]
        std::vector<uint8_t> ret = std::vector<uint8_t>();
        ret.push_back(0x81);
        ret.push_back(0x04);
        ret.push_back(0x31);
        ret.push_back(0x08);
        ret.push_back(0x01);
        ret.push_back(0xBD);
        return ret;
    }
};

//So, let's have the test class have a getter that either reads a hard-coded
//byte vector, and later we can swap it out for a generated Cpp file to practise
//with CMake.

class MockHeartbeatResponseWriter : public pico::ibus::output::writer::HeartbeatResponseWriter {

    //Override
    std::shared_ptr<pico::logger::BaseLogger> logger = std::make_shared<DummyLogger>(DummyLogger());
public:
    //Mock this to do nothing
    MOCK_METHOD(std::shared_ptr<pico::ibus::dma::IDmaManager>, getDmaManager, (), (override));
    //Mock this to do nothing, but we can record it's called.
    MOCK_METHOD(void, sendHeartbeatResponse, () );

    MockHeartbeatResponseWriter(
            std::shared_ptr<pico::logger::BaseLogger> logger,
            std::shared_ptr<pico::ibus::dma::IDmaManager> dmaManager
            ) : HeartbeatResponseWriter(logger, dmaManager) {}
};

//TODO need a way on HeartbeatRequestObserver to call real on onNewPacket, but slot() onNewPiToPicoPacket

TEST_F(HeartbeatRequestObserverTest, Dummy) {

    std::shared_ptr<pico::logger::BaseLogger> logger = std::make_shared<DummyLogger>(DummyLogger());

    MockHeartbeatResponseWriter mockHeartbeatResponseWriter = MockHeartbeatResponseWriter(logger, nullptr);

    pico::ibus::observers::HeatbeatRequestObserver requestObserver = pico::ibus::observers::HeatbeatRequestObserver(
            logger,
            std::make_shared<pico::ibus::output::writer::HeartbeatResponseWriter>(mockHeartbeatResponseWriter)
            );


    requestObserver.dispatchPacket(
            logger,
            //TODO the decode logic fills auto* input string with junk.
            pico::ibus::data::IbusPacket(getHeartbeatRequestRawBytes())
            );

    EXPECT_CALL(mockHeartbeatResponseWriter, sendHeartbeatResponse());

    //Now we ask the writer mock? or the observer mock? for the PiToPicoMessage?
}