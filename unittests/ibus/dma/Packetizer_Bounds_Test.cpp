#include "ibus/dma/Packetizer.h"
#include <gtest/gtest.h>
#include <random>
using namespace pico::ibus::dma;

static std::vector<uint8_t> makePacket(uint8_t src, uint8_t dest, std::vector<uint8_t> data) {
    std::vector<uint8_t> p = {src, (uint8_t)(data.size() + 2), dest};
    p.insert(p.end(), data.begin(), data.end());
    uint8_t crc = 0; for (auto b : p) crc ^= b;
    p.push_back(crc);
    return p;
}
static const std::vector<uint8_t> RADIO_AUX = {0x3B, 0x05, 0x68, 0x4E, 0x00, 0x00, 0x18};

TEST(PacketizerBounds, LargestPacketLen253) {
    Packetizer p;
    auto pkt = makePacket(0x68, 0x18, std::vector<uint8_t>(251, 0xAB));
    ASSERT_EQ(pkt[1], 253); ASSERT_EQ(pkt.size(), 255u);
    p.addBytes(pkt);
    ASSERT_TRUE(p.isPacketComplete());
    ASSERT_EQ(p.getPacketBytes(), pkt);
}
TEST(PacketizerBounds, Len254RejectedThenRecovers) {
    Packetizer p;
    p.addBytes({0x68, 0xFE});
    ASSERT_FALSE(p.isPacketComplete());
    p.addBytes(RADIO_AUX);
    ASSERT_TRUE(p.isPacketComplete());
    ASSERT_EQ(p.getPacketBytes(), RADIO_AUX);
}
TEST(PacketizerBounds, Len255RejectedThenRecovers) {
    Packetizer p;
    p.addBytes({0x68, 0xFF});
    ASSERT_FALSE(p.isPacketComplete());
    p.addBytes(RADIO_AUX);
    ASSERT_TRUE(p.isPacketComplete());
    ASSERT_EQ(p.getPacketBytes(), RADIO_AUX);
}
TEST(PacketizerBounds, Len254Or255StreamNeverOverflows) {
    for (uint8_t len : {254, 255}) {
        Packetizer p;
        p.addByte(0x68); p.addByte(len);
        for (int i = 0; i < 600; i++) p.addByte(0x00);   // used to wrap position forever
        ASSERT_LE(p.getPacketBytes().size(), (size_t)Packetizer::MAX_PACKET_BYTES);
    }
}
TEST(PacketizerBounds, BadChecksumThenValidPacket) {
    Packetizer p;
    auto bad = RADIO_AUX; bad.back() ^= 0xFF;
    p.addBytes(bad);
    ASSERT_FALSE(p.isPacketComplete());
    p.addBytes(RADIO_AUX);               // previously the first 2 bytes were swallowed
    ASSERT_TRUE(p.isPacketComplete());
    ASSERT_EQ(p.getPacketBytes(), RADIO_AUX);
}
TEST(PacketizerBounds, BadChecksumLen253NoOverflow) {
    Packetizer p;
    auto pkt = makePacket(0x68, 0x18, std::vector<uint8_t>(251, 0xAB));
    pkt.back() ^= 0xFF;
    p.addBytes(pkt);
    p.addBytes({0x11, 0x22, 0x33});      // previously wrote packetBytes[255]
    ASSERT_FALSE(p.isPacketComplete());
}
TEST(PacketizerBounds, RandomBytesFuzz) {
    std::mt19937 rng(1234);
    Packetizer p; int complete = 0;
    for (int i = 0; i < 200000; i++) {
        p.addByte((uint8_t)rng());
        if (p.isPacketComplete()) {
            auto b = p.getPacketBytes();
            ASSERT_EQ(b.size(), (size_t)b[1] + 2);
            ASSERT_LE(b.size(), (size_t)Packetizer::MAX_PACKET_BYTES);
            complete++; p.reset();
        }
    }
    printf("fuzz: %d packets completed\n", complete);
}
