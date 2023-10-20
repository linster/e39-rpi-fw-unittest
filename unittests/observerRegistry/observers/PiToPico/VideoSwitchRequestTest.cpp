//
// Created by stefan on 10/13/23.
//
#include "ibus/observerRegistry/observers/PiToPico/VideoRequest/PicoVideoRequestObserver.h"
#include "DummyLogger.h"
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

class MockVideoSwitch : public pico::hardware::videoSwitch::VideoSwitch {
public:
    pico::hardware::videoSwitch::VideoSource selectedSource;
    void switchTo(pico::hardware::videoSwitch::VideoSource source) override {
        selectedSource = source;
    }

    pico::hardware::videoSwitch::VideoSource getPreviousVideoSource() override {
        return selectedSource;
    }
};

TEST_F(PicoVideoSwitchRequestObserverTest, upstream) {
    std::shared_ptr<pico::logger::BaseLogger> logger = std::make_shared<DummyLogger>(DummyLogger());

    MockVideoSwitch mockVideoSwitch = MockVideoSwitch();
    std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitchPtr = std::make_shared<MockVideoSwitch>(mockVideoSwitch);

    pico::ibus::observers::PicoVideoRequestObserver requestObserver = pico::ibus::observers::PicoVideoRequestObserver(
            logger,
            videoSwitchPtr
            );

    requestObserver.dispatchPacket(
            logger,
            pico::ibus::data::IbusPacket(upstream())
            );

    EXPECT_EQ(
            pico::hardware::videoSwitch::VideoSource::UPSTREAM,
            std::dynamic_pointer_cast<MockVideoSwitch>(videoSwitchPtr)->selectedSource
            );
}
TEST_F(PicoVideoSwitchRequestObserverTest, pico) {
    std::shared_ptr<pico::logger::BaseLogger> logger = std::make_shared<DummyLogger>(DummyLogger());

    MockVideoSwitch mockVideoSwitch = MockVideoSwitch();
    std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitchPtr = std::make_shared<MockVideoSwitch>(mockVideoSwitch);

    pico::ibus::observers::PicoVideoRequestObserver requestObserver = pico::ibus::observers::PicoVideoRequestObserver(
            logger,
            videoSwitchPtr
    );

    requestObserver.dispatchPacket(
            logger,
            pico::ibus::data::IbusPacket(pico())
    );

    EXPECT_EQ(
            pico::hardware::videoSwitch::VideoSource::PICO,
            std::dynamic_pointer_cast<MockVideoSwitch>(videoSwitchPtr)->selectedSource
            );
}
TEST_F(PicoVideoSwitchRequestObserverTest, pi) {
    std::shared_ptr<pico::logger::BaseLogger> logger = std::make_shared<DummyLogger>(DummyLogger());

    MockVideoSwitch mockVideoSwitch = MockVideoSwitch();
    std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitchPtr = std::make_shared<MockVideoSwitch>(mockVideoSwitch);

    pico::ibus::observers::PicoVideoRequestObserver requestObserver = pico::ibus::observers::PicoVideoRequestObserver(
            logger,
            videoSwitchPtr
    );

    requestObserver.dispatchPacket(
            logger,
            pico::ibus::data::IbusPacket(rpi())
    );

    EXPECT_EQ(
            pico::hardware::videoSwitch::VideoSource::PI,
            std::dynamic_pointer_cast<MockVideoSwitch>(videoSwitchPtr)->selectedSource
            );
}
TEST_F(PicoVideoSwitchRequestObserverTest, rvc) {
    std::shared_ptr<pico::logger::BaseLogger> logger = std::make_shared<DummyLogger>(DummyLogger());

    MockVideoSwitch mockVideoSwitch = MockVideoSwitch();
    std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitchPtr = std::make_shared<MockVideoSwitch>(mockVideoSwitch);

    pico::ibus::observers::PicoVideoRequestObserver requestObserver = pico::ibus::observers::PicoVideoRequestObserver(
            logger,
            videoSwitchPtr
    );

    requestObserver.dispatchPacket(
            logger,
            pico::ibus::data::IbusPacket(rvc())
    );

    EXPECT_EQ(
            pico::hardware::videoSwitch::VideoSource::RVC,
            std::dynamic_pointer_cast<MockVideoSwitch>(videoSwitchPtr)->selectedSource
            );
}