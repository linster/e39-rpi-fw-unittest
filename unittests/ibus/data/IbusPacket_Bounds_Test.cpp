#include "ibus/data/IbusPacket.h"
#include <gtest/gtest.h>
using namespace pico::ibus::data;

static std::vector<uint8_t> makePacket(uint8_t src, uint8_t dest, std::vector<uint8_t> data) {
    std::vector<uint8_t> p = {src, (uint8_t)(data.size() + 2), dest};
    p.insert(p.end(), data.begin(), data.end());
    uint8_t crc = 0; for (auto b : p) crc ^= b;
    p.push_back(crc);
    return p;
}
static std::unique_ptr<std::array<uint8_t, 255>> heapArray(const std::vector<uint8_t> &bytes) {
    auto a = std::make_unique<std::array<uint8_t, 255>>(); a->fill(0);
    for (size_t i = 0; i < bytes.size() && i < 255; i++) (*a)[i] = bytes[i];
    return a;
}
static const std::vector<uint8_t> RADIO_AUX = {0x3B, 0x05, 0x68, 0x4E, 0x00, 0x00, 0x18};

TEST(IbusPacketBounds, ArrayValidPacketIsValid) {
    IbusPacket p(heapArray(RADIO_AUX));
    ASSERT_EQ(p.getRawPacket(), RADIO_AUX);
    ASSERT_EQ(p.getGivenCrc(), 0x18);
    ASSERT_TRUE(p.isPacketValid());            // was false: CRC read from raw[len + 2]
}
TEST(IbusPacketBounds, StackArrayValidPacketIsValid) {
    std::array<uint8_t, 255> a{}; for (size_t i = 0; i < RADIO_AUX.size(); i++) a[i] = RADIO_AUX[i];
    IbusPacket p(a);
    ASSERT_TRUE(p.isPacketValid());
}
TEST(IbusPacketBounds, QueueRoundTripLikeDmaManager) {
    // writePacketToQ copies getRawPacket() into a 255-byte buffer; readIncomingQ builds from it.
    IbusPacket built(IbusDeviceEnum::PICO, IbusDeviceEnum::RPI, {1, 2, 3, 4});
    IbusPacket received(heapArray(built.getRawPacket()));
    ASSERT_EQ(received.getRawPacket(), built.getRawPacket());
    ASSERT_TRUE(received.isPacketValid());
    ASSERT_EQ(received.getDataLength(), 4);
}
TEST(IbusPacketBounds, Len253LargestFits) {
    auto pkt = makePacket(0x68, 0x18, std::vector<uint8_t>(251, 0xAB));
    ASSERT_EQ(pkt.size(), 255u);
    IbusPacket p(heapArray(pkt));
    ASSERT_EQ(p.getRawPacket(), pkt);
    ASSERT_TRUE(p.isPacketValid());
}
TEST(IbusPacketBounds, Len254And255RejectedWithoutOverread) {
    for (uint8_t len : {254, 255}) {
        auto a = std::make_unique<std::array<uint8_t, 255>>(); a->fill(0x55); (*a)[1] = len;
        IbusPacket p(std::move(a));
        ASSERT_EQ(p.getRawPacket().size(), 4u);
        ASSERT_EQ(p.getDataLength(), 0);
    }
}
TEST(IbusPacketBounds, Len0And1RejectedNoDataLengthUnderflow) {
    for (uint8_t len : {0, 1}) {
        auto a = std::make_unique<std::array<uint8_t, 255>>(); a->fill(0x55); (*a)[1] = len;
        IbusPacket p(std::move(a));
        ASSERT_EQ(p.getRawPacket().size(), 4u);
        ASSERT_EQ(p.getDataLength(), 0);       // len 1 used to give a 3-byte packet -> 255
    }
}
TEST(IbusPacketBounds, BadCrcIsInvalid) {
    auto bad = RADIO_AUX; bad.back() ^= 0xFF;
    IbusPacket p(heapArray(bad));
    ASSERT_FALSE(p.isPacketValid());
}
TEST(IbusPacketBounds, NullUniquePtr) {
    IbusPacket p(std::unique_ptr<std::array<uint8_t, 255>>{});
    ASSERT_EQ(p.getRawPacket().size(), 4u);
}
