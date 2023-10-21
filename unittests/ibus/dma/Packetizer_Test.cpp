//
// Created by stefan on 10/5/23.
//


#include "ibus/dma/Packetizer.h"
#include <gtest/gtest.h>

using namespace pico::ibus::dma;

class PacketizerTest : public ::testing::Test {

};

TEST_F(PacketizerTest, DummyTest) {
    ASSERT_TRUE(true);
}

///Add a byte, can read the byte
TEST_F(PacketizerTest, AddAByte) {

    Packetizer p = Packetizer();

    p.addByte(0x01);

    ASSERT_FALSE(p.isPacketComplete());
    ASSERT_EQ(p.getPacketBytes().size(), 1);
    ASSERT_EQ(p.getPacketBytes()[0], 0x01);
}

///Add two bytes, can read two bytes
TEST_F(PacketizerTest, AddTwoBytes) {
    Packetizer p = Packetizer();

    p.addByte(0x01);
    p.addByte(0x02);

    ASSERT_FALSE(p.isPacketComplete());
    ASSERT_EQ(p.getPacketBytes().size(), 2);
    ASSERT_EQ(p.getPacketBytes()[0], 0x01);
    ASSERT_EQ(p.getPacketBytes()[1], 0x02);
}
///Add two bytes in a vector, can read them
TEST_F(PacketizerTest, AddTwoVector) {
    Packetizer p = Packetizer();

    std::vector<uint8_t> bytesToAdd = std::vector<uint8_t>();
    bytesToAdd.push_back(0x01);
    bytesToAdd.push_back(0x02);

    p.addBytes(bytesToAdd);

    ASSERT_FALSE(p.isPacketComplete());
    ASSERT_EQ(p.getPacketBytes().size(), 2);
    ASSERT_EQ(p.getPacketBytes()[0], 0x01);
    ASSERT_EQ(p.getPacketBytes()[1], 0x02);
}
///Add a known byte stream, get a valid checksum
TEST_F(PacketizerTest, AddKnownByteSequenceValidCheckSumAtEnd) {
    Packetizer p = Packetizer();

    std::vector<uint8_t> bytesToAdd = std::vector<uint8_t>();
    //Switch Radio to AUX
    bytesToAdd.push_back(0x3B); //0
    bytesToAdd.push_back(0x05); //1
    bytesToAdd.push_back(0x68); //2
    bytesToAdd.push_back(0x4E); //3
    bytesToAdd.push_back(0x00); //4
    bytesToAdd.push_back(0x00); //5
    bytesToAdd.push_back(0x18); //6

    p.addBytes(bytesToAdd);

    ASSERT_TRUE(p.isPacketComplete());
    ASSERT_EQ(p.getPacketBytes().size(), bytesToAdd.size());
}
///Start adding a known byte stream, don't get a valid checksum until the last byte is added.
TEST_F(PacketizerTest, AddKnownByteSequenceNoValidChecksumUntilLastByteAdded) {
    Packetizer p = Packetizer();

    //Switch Radio to AUX
    p.addByte(0x3B);
    ASSERT_FALSE(p.isPacketComplete());
    p.addByte(0x05);
    ASSERT_FALSE(p.isPacketComplete());
    p.addByte(0x68);
    ASSERT_FALSE(p.isPacketComplete());
    p.addByte(0x4E);
    ASSERT_FALSE(p.isPacketComplete());
    p.addByte(0x00);
    ASSERT_FALSE(p.isPacketComplete());
    p.addByte(0x00);
    ASSERT_FALSE(p.isPacketComplete());
    p.addByte(0x18);
    ASSERT_TRUE(p.isPacketComplete());

    ASSERT_EQ(p.getPacketBytes().size(), 7);

}
///After we have the Packetizer full of a known bytestream, verify we can call reset and all the
///internal state is okay.
TEST_F(PacketizerTest, TestReset) {
    Packetizer p = Packetizer();

    std::vector<uint8_t> bytesToAdd = std::vector<uint8_t>();
    //Switch Radio to AUX
    bytesToAdd.push_back(0x3B);
    bytesToAdd.push_back(0x05);
    bytesToAdd.push_back(0x68);
    bytesToAdd.push_back(0x4E);
    bytesToAdd.push_back(0x00);
    bytesToAdd.push_back(0x00);
    bytesToAdd.push_back(0x18);

    p.addBytes(bytesToAdd);

    ASSERT_TRUE(p.isPacketComplete());
    ASSERT_EQ(p.getPacketBytes().size(), bytesToAdd.size());

    //Reset the packetizer state. Also deletes other packets waiting to be assembled
    p.reset();

    ASSERT_FALSE(p.isPacketComplete());
    ASSERT_EQ(p.getPacketBytes().size(), 0);
}


////Tests wth using recycle to save memory on inflight packets.
TEST_F(PacketizerTest, RecycleNoInterleave) {
    Packetizer p = Packetizer();

    //Switch Radio to AUX
    p.addByte(0x3B);
    ASSERT_FALSE(p.isPacketComplete());
    p.addByte(0x05);
    ASSERT_FALSE(p.isPacketComplete());
    p.addByte(0x68);
    ASSERT_FALSE(p.isPacketComplete());
    p.addByte(0x4E);
    ASSERT_FALSE(p.isPacketComplete());
    p.addByte(0x00);
    ASSERT_FALSE(p.isPacketComplete());
    p.addByte(0x00);
    ASSERT_FALSE(p.isPacketComplete());
    p.addByte(0x18);
    ASSERT_TRUE(p.isPacketComplete());
    ASSERT_EQ(p.getPacketBytes().size(), 7);

    //TODO recycle actually doesn't make any sense.
    //TODO we just want to check when we add a byte whether we have a complete packet.
    //TODO and if we do, then the caller should read it.
    p.recycle(); //Recycle without interleave.
    ASSERT_FALSE(p.isPacketComplete());
    ASSERT_EQ(p.getPacketBytes().size(), 0);

    //Switch Radio to NAV/TV
    p.addByte(0x3B);
    ASSERT_FALSE(p.isPacketComplete());
    p.addByte(0x05);
    ASSERT_FALSE(p.isPacketComplete());
    p.addByte(0x68);
    ASSERT_FALSE(p.isPacketComplete());
    p.addByte(0x4E);
    ASSERT_FALSE(p.isPacketComplete());
    p.addByte(0x01);
    ASSERT_FALSE(p.isPacketComplete());
    p.addByte(0x00);
    ASSERT_FALSE(p.isPacketComplete());
    p.addByte(0x19);

    ASSERT_TRUE(p.isPacketComplete());
    ASSERT_EQ(p.getPacketBytes().size(), 7);
}