//
// BaseObserver::decodePiToPicoMessage (TODO_MEMORY_BUGS.md 4.1). Payloads are encoded the
// way protobuf-java on the Pi encodes them (proto3: defaults omitted, set sub-messages kept).
//

#include <gtest/gtest.h>
#include "ibus/observerRegistry/observers/BaseObserver.h"
#include "DummyLogger.h"

using namespace pico;
using M = messages::PiToPicoMessage;

namespace {
    struct DecodeHarness : ibus::observers::BaseObserver {
        std::string getTag() override { return "DecodeHarness"; }
        void onNewPacket(std::shared_ptr<ibus::data::IbusPacket>) override {}
        M decode(std::vector<uint8_t> payload) {
            ibus::data::IbusPacket pkt(ibus::data::IbusDeviceEnum::RPI, ibus::data::IbusDeviceEnum::PICO, payload);
            return decodePiToPicoMessage(std::make_shared<DummyLogger>(), pkt);
        }
    };
}

TEST(PiToPicoDecode, HeartbeatRequest) {
    ASSERT_EQ(DecodeHarness().decode({0x08, 0x01}).messageType, M::HeartbeatRequest);
}

TEST(PiToPicoDecode, ConfigPushTypical) {
    std::string hash = "0123456789abcdef0123456789abcdef01234567";
    std::vector<uint8_t> cfg = {0x0A, (uint8_t) hash.size()}; cfg.insert(cfg.end(), hash.begin(), hash.end());
    cfg.insert(cfg.end(), {0x10, 0x01, 0x38, 0x02}); // log output on, scan program CLOCK
    std::vector<uint8_t> p = {0x08, 0x04, 0x12, (uint8_t) cfg.size()}; p.insert(p.end(), cfg.begin(), cfg.end());
    M m = DecodeHarness().decode(p);
    ASSERT_EQ(m.messageType, M::ConfigPush);
    ASSERT_EQ(m.newConfig.rpiFwGitCommitHash, hash);
    ASSERT_TRUE(m.newConfig.isIbusLogOutputEnabled);
    ASSERT_EQ(m.newConfig.scanProgramOnBoot, messages::ConfigMessage::ScanProgram::CLOCK);
}

TEST(PiToPicoDecode, ConfigPushAllDefaultsContainsZeroByte) {
    // "12 00" = empty sub-message. Used to be cut off at the 0x00 by c_str().
    ASSERT_EQ(DecodeHarness().decode({0x08, 0x04, 0x12, 0x00}).messageType, M::ConfigPush);
}

TEST(PiToPicoDecode, CorruptMessageIsIgnored) {
    // Truncated varint: decode fails. Used to return an uninitialized messageType.
    ASSERT_EQ(DecodeHarness().decode({0x08, 0x84}).messageType, M::EmptyMessage);
}

TEST(PiToPicoDecode, TypeZeroIsEmptyMessage) {
    // messageType omitted (proto value 0). decode() used to fall off the end.
    ASSERT_EQ(DecodeHarness().decode({0x12, 0x00}).messageType, M::EmptyMessage);
}

TEST(PiToPicoDecode, UnknownTypeIsEmptyMessage) {
    ASSERT_EQ(DecodeHarness().decode({0x08, 0x63}).messageType, M::EmptyMessage); // 99
}
