//
// ScanlineEncoder (TODO_MEMORY_BUGS.md 2.2): bounds and end-of-line alignment.
// A small decoder walks the token stream the way scanvideo.pio does.
//

#include <gtest/gtest.h>
#include <memory>
#include "video/scanProgram/scanPrograms/graphicsLib/command/ScanlineEncoder.h"

using namespace video::scanVideo::graphics::command;

namespace {
    const ScanlineTokens T{0xA1, 0xA2, 0xA3, 0xA4, 0xA5};

    //Returns pixel count; fails the test on any alignment/format violation.
    int walk(const uint16_t *d, size_t n) {
        size_t i = 0; int px = 0;
        while (i < n) {
            uint16_t tok = d[i];
            if (tok == T.colorRun) { px += d[i + 2] + 3; i += 3; }
            else if (tok == T.raw1p) { px += 1; i += 2; }
            else if (tok == T.raw2p) { px += 2; i += 3; }
            else if (tok == T.eolSkipAlign) { EXPECT_EQ(i % 2, 0u) << "EOL_SKIP_ALIGN must start on a word"; i += 2; EXPECT_EQ(i, n); return px; }
            else if (tok == T.eolAlign) { EXPECT_EQ(i % 2, 1u) << "EOL_ALIGN must start on an odd halfword"; i += 1; EXPECT_EQ(i, n); return px; }
            else { ADD_FAILURE() << "bad token at " << i; return -1; }
        }
        ADD_FAILURE() << "no end-of-line token"; return -1;
    }

    void check(std::vector<RleRun> runs, size_t cap, bool expectTruncated) {
        std::unique_ptr<uint16_t[]> buf(new uint16_t[cap]); // exact size: ASan catches overruns
        auto r = encodeScanline(runs, buf.get(), cap, T, 400, 7);
        ASSERT_TRUE(r.ok);
        ASSERT_LE(r.halfwordsWritten, cap);
        ASSERT_EQ(r.halfwordsWritten % 2, 0u);
        ASSERT_EQ(r.wordsUsed * 2, r.halfwordsWritten);
        ASSERT_EQ(r.truncated, expectTruncated);
        ASSERT_EQ(walk(buf.get(), r.halfwordsWritten), 401); // 400 px + the final black pixel
    }
}

TEST(ScanlineEncoder, OneFullWidthRun) { check({RleRun(0, 400, 1)}, 360, false); }
TEST(ScanlineEncoder, OddParityUsesEolAlign) { check({RleRun(0, 1, 1), RleRun(1, 399, 2)}, 360, false); }
TEST(ScanlineEncoder, EvenParityUsesEolSkipAlign) { check({RleRun(0, 2, 1), RleRun(2, 398, 2)}, 360, false); }

TEST(ScanlineEncoder, WorstCaseSinglePixelRunsTruncate) {
    std::vector<RleRun> runs; for (int i = 0; i < 400; i++) runs.emplace_back(i, 1, i & 1);
    check(runs, 360, true); // old code wrote 804 halfwords here
}

TEST(ScanlineEncoder, TwoPixelRunsTruncate) {
    std::vector<RleRun> runs; for (int i = 0; i < 200; i++) runs.emplace_back(i * 2, 2, i & 1);
    check(runs, 360, true);
}

TEST(ScanlineEncoder, TinyBuffer) {
    std::vector<RleRun> runs; for (int i = 0; i < 400; i++) runs.emplace_back(i, 1, i & 1);
    check(runs, 8, true);
    std::unique_ptr<uint16_t[]> b(new uint16_t[6]);
    ASSERT_FALSE(encodeScanline(runs, b.get(), 6, T, 400, 7).ok);
}
