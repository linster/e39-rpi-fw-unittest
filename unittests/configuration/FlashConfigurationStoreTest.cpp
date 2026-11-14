//
// FlashConfigurationStore against a fake flash block (unittests/hoststubs).
// Covers TODO_MEMORY_BUGS.md 1.1 (read overflow / never decodes), 1.3 (erase/program
// contract, interrupts, core 1 lockout) and 1.4 (no commit hash required).
//

#include <gtest/gtest.h>
#include "configuration/store/flash/FlashConfigurationStore.h"
#include "hardware/flash.h"
#include "pico/multicore.h"
#include "DummyLogger.h"

using namespace pico::config;

class FlashConfigurationStoreTest : public ::testing::Test {
protected:
    std::shared_ptr<DummyLogger> logger = std::make_shared<DummyLogger>();
    void SetUp() override { hoststub::resetHostFlash(); }

    static Configuration configWithHash(std::string hash) {
        pico::messages::ConfigMessage m{};
        m.rpiFwGitCommitHash = hash;
        m.isIbusLogOutputEnabled = true;
        m.scanProgramOnBoot = pico::messages::ConfigMessage::ScanProgram::CLOCK;
        return Configuration(m);
    }
};

TEST_F(FlashConfigurationStoreTest, ErasedFlashHasNoConfiguration) {
    FlashConfigurationStore store(logger);
    ASSERT_FALSE(store.canReadConfiguration());
}

TEST_F(FlashConfigurationStoreTest, ZeroedFlashHasNoConfiguration) {
    memset(hoststub::hostFlash, 0, sizeof hoststub::hostFlash);
    FlashConfigurationStore store(logger);
    ASSERT_FALSE(store.canReadConfiguration());
}

TEST_F(FlashConfigurationStoreTest, SaveThenLoadRoundTrips) {
    FlashConfigurationStore store(logger);
    store.saveConfiguration(configWithHash("abc123"));
    ASSERT_EQ(hoststub::eraseCalls, 1);
    ASSERT_EQ(hoststub::programCalls, 1);
    ASSERT_EQ(hoststub::interruptsDisabledDepth, 0); // restored afterwards

    FlashConfigurationStore reader(logger); // fresh instance: really reads "flash"
    ASSERT_TRUE(reader.canReadConfiguration());
    auto msg = reader.getConfiguration()->toMessage();
    ASSERT_EQ(msg.rpiFwGitCommitHash, "abc123");
    ASSERT_TRUE(msg.isIbusLogOutputEnabled);
    ASSERT_EQ(msg.scanProgramOnBoot, pico::messages::ConfigMessage::ScanProgram::CLOCK);
}

TEST_F(FlashConfigurationStoreTest, LoadsWithoutCommitHash) {
    FlashConfigurationStore store(logger);
    store.saveConfiguration(configWithHash(""));
    ASSERT_TRUE(FlashConfigurationStore(logger).canReadConfiguration());
}

TEST_F(FlashConfigurationStoreTest, SecondSaveReplacesFirst) {
    FlashConfigurationStore store(logger);
    store.saveConfiguration(configWithHash("first-save-with-a-longer-hash"));
    store.saveConfiguration(configWithHash("second"));
    FlashConfigurationStore reader(logger);
    ASSERT_EQ(reader.getConfiguration()->toMessage().rpiFwGitCommitHash, "second");
}

TEST_F(FlashConfigurationStoreTest, LocksOutCore1WhenItIsRunning) {
    hoststub::core1VictimInitialized = true;
    FlashConfigurationStore store(logger);
    store.saveConfiguration(configWithHash("abc"));
    ASSERT_EQ(hoststub::lockoutStarts, 1);
    ASSERT_EQ(hoststub::lockoutEnds, 1);
    ASSERT_EQ(hoststub::programCalls, 1);
}

TEST_F(FlashConfigurationStoreTest, DoesNotWriteIfCore1LockoutTimesOut) {
    hoststub::core1VictimInitialized = true;
    hoststub::lockoutStartSucceeds = false;
    FlashConfigurationStore store(logger);
    store.saveConfiguration(configWithHash("abc"));
    ASSERT_EQ(hoststub::eraseCalls, 0);
    ASSERT_EQ(hoststub::programCalls, 0);
    ASSERT_EQ(hoststub::lockoutEnds, 0);
}

TEST_F(FlashConfigurationStoreTest, NoLockoutWhenCore1NotRunning) {
    FlashConfigurationStore store(logger);
    store.saveConfiguration(configWithHash("abc"));
    ASSERT_EQ(hoststub::lockoutStarts, 0);
    ASSERT_EQ(hoststub::programCalls, 1);
}
