//
// Helpers shared by tests.
//

#ifndef E39_RPI_FW_UNITTEST_TESTPACKETUTILS_H
#define E39_RPI_FW_UNITTEST_TESTPACKETUTILS_H

#include <cstdint>
#include <vector>
#include "ibus/data/IbusPacket.h"

/**
 * The data bytes of a packet: everything between dest and the CRC.
 * Replacement for IbusPacket::getData(), removed from the firmware in 5086e46.
 */
inline std::vector<uint8_t> packetData(pico::ibus::data::IbusPacket packet) {
    std::vector<uint8_t> raw = packet.getRawPacket();
    if (raw.size() < pico::ibus::data::IbusPacket::DATA_START + 1) {
        return {};
    }
    return std::vector<uint8_t>(raw.begin() + pico::ibus::data::IbusPacket::DATA_START, raw.end() - 1);
}

#endif //E39_RPI_FW_UNITTEST_TESTPACKETUTILS_H
