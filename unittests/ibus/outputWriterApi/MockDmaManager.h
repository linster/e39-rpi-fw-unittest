//
// Created by stefan on 10/19/23.
//

#ifndef E39_RPI_FW_UNITTEST_MOCKDMAMANAGER_H
#define E39_RPI_FW_UNITTEST_MOCKDMAMANAGER_H

#include "ibus/data/IbusPacket.h"
#include "ibus/dma/IDmaManager.h"

class MockDmaManager : public pico::ibus::dma::IDmaManager {
public:

    void cpu0setup() override {}
    void onCpu0Loop() override {}
    void cpu1Setup() override {}
    void onCpu1Loop() override {}

    //Make a bogus default packet
    pico::ibus::data::IbusPacket writtenPacketToCar = pico::ibus::data::IbusPacket(
            pico::ibus::data::BROADCAST,
            pico::ibus::data::BROADCAST,
            std::vector<uint8_t>()
    );
    pico::ibus::data::IbusPacket writtenPacketToPi = pico::ibus::data::IbusPacket(
            pico::ibus::data::BROADCAST,
            pico::ibus::data::BROADCAST,
            std::vector<uint8_t>()
    );
    //To car
    void cpu0scheduleOutgoingMessage(pico::ibus::data::IbusPacket packet) override {
        writtenPacketToCar = packet;
    }
    //To Pi
    void cpu0scheduleOutgoingProbeOnlyMessage(pico::ibus::data::IbusPacket packet) override {
        writtenPacketToPi = packet;
    };

    MockDmaManager() = default;
    ~MockDmaManager() override = default;
};


#endif //E39_RPI_FW_UNITTEST_MOCKDMAMANAGER_H
