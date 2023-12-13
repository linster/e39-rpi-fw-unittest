//
// Created by stefan on 10/13/23.
//


#include "DummyLogger.h"

//TODO holy yikes I wouldn't want to have include path collisions in a big project.
#include "ibus/outputWriterApi/MockDmaManager.h"

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


//TODO need a way on HeartbeatRequestObserver to call real on onNewPacket, but slot() onNewPiToPicoPacket

TEST_F(HeartbeatRequestObserverTest, Dummy) {

    std::shared_ptr<pico::logger::BaseLogger> logger = std::make_shared<DummyLogger>(DummyLogger());

    MockDmaManager mockDmaManager = MockDmaManager();
    std::shared_ptr<pico::ibus::dma::IDmaManager> iDmaManagerPtr = std::make_shared<MockDmaManager>(mockDmaManager);


    //We'll just use the real class, and have it throw some bytes into our fake dmaManager.
    std::shared_ptr<pico::ibus::output::writer::HeartbeatResponseWriter> writerPtr =
            std::make_shared<pico::ibus::output::writer::HeartbeatResponseWriter>(
                    logger,
                    iDmaManagerPtr);

    pico::ibus::observers::HeatbeatRequestObserver requestObserver = pico::ibus::observers::HeatbeatRequestObserver(
            logger,
            writerPtr
            );


    requestObserver.dispatchPacket(
            logger,
            std::make_shared<pico::ibus::data::IbusPacket>(getHeartbeatRequestRawBytes())
            );

    pico::ibus::data::IbusPacket writtenPacket = std::dynamic_pointer_cast<MockDmaManager>(iDmaManagerPtr)->writtenPacketToPi;
    pico::ibus::data::IbusPacket dummyPacket = pico::ibus::data::IbusPacket(
            pico::ibus::data::BROADCAST,
            pico::ibus::data::BROADCAST,
            std::vector<uint8_t>()
    );

    //Not a great test, but we did set something on the dmaManager, so that means the observer was ok.
    EXPECT_NE(writtenPacket.getRawPacket(), dummyPacket.getRawPacket());
}