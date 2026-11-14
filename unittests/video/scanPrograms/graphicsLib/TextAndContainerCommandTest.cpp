//
// TextCommand font lookup (TODO_MEMORY_BUGS.md 2.3) and RleRunContainerCommand bounds (2.4).
//

#include <gtest/gtest.h>
#include "video/scanProgram/scanPrograms/graphicsLib/command/TextCommand.h"
#include "video/scanProgram/scanPrograms/graphicsLib/command/RleRunContainerCommand.h"
#include "video/scanProgram/scanPrograms/graphicsLib/fonts/FontProvider.h"

using namespace video::scanVideo::graphics::command;
using FP = video::fonts::FontProvider;

namespace {
    size_t pixels(std::map<uint16_t, std::vector<RleRun>> m) {
        size_t n = 0; for (auto &l : m) for (auto r : l.second) n += r.getLen(); return n;
    }
    size_t bits(const uint8_t *b) {
        size_t n = 0; for (int i = 0; i < 8; i++) n += __builtin_popcount(b[i]); return n;
    }
}

TEST(TextCommand, EveryByteValueUsesAValidGlyph) {
    size_t copyright = bits(FP::font8x8_ext_latin[9]);
    for (int v = 0; v < 256; v++) {
        TextCommand t(std::string(1, (char) v), PxCoord(0, 0), 1, 1);
        size_t expected = v < 128 ? bits(FP::font8x8_basic[v]) : copyright;
        ASSERT_EQ(pixels(t.getRleRunsForShape()), expected) << "byte " << v;
    }
}

TEST(RleRunContainerCommand, AffectedScanlines) {
    std::map<uint16_t, std::vector<RleRun>> m;
    m[40].emplace_back(1, 2, 3); m[12].emplace_back(4, 5, 6); m[70].emplace_back(7, 8, 9);
    RleRunContainerCommand c(m);
    auto lines = c.getAffectedScanlines();
    ASSERT_EQ(lines.first, 12);
    ASSERT_EQ(lines.second, 70);
    ASSERT_EQ(c.getRleRunsForShape().size(), 3u);
}

TEST(RleRunContainerCommand, EmptyMap) {
    RleRunContainerCommand c({});
    auto lines = c.getAffectedScanlines();
    ASSERT_EQ(lines.first, 0);
    ASSERT_EQ(lines.second, 0);
}
