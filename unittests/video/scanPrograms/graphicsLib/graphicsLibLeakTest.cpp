//
// Created by stefan on 12/14/23.
//

#include <gtest/gtest.h>
#include "video/scanProgram/scanPrograms/graphicsLib/graphicsLib.h"
#include "DummyLogger.h"
#include "fmt/format.h"

//Little bit of magic that allows includes to work a bit
#ifndef PICO_NO_HARDWARE
#define PICO_NO_HARDWARE
#endif

class GraphicsLeakTestUtils {

    //Method to take the render method, call it for days,
    //then print the contents out as ascii art.

    std::string rleRunToString(
            std::shared_ptr<DummyLogger> dummyLogger,
            std::string tag,
            video::scanVideo::graphics::command::RleRun run) {


        dummyLogger->i(tag, fmt::format("RleRun(uint16_t startX = {:d};\n"
                                        "        uint16_t len = {:d};\n"
                                        "        uint32_t colour = {:d};)",
                                        run.getStartX(),
                                        run.getLen(),
                                        run.getColour())
                                        );
    }

    //TODO we want a TestableCommandProcessor that overrides skipScanline to print <skipped>
    //TODO and drawScanline() to print the RLE runs for the scanline.
};

class GraphicsLeakTest : public ::testing::Test {

public:
    std::shared_ptr<DummyLogger> dummyLogger;
    std::shared_ptr<video::scanProgram::graphicsLib> graphicsLib;
    std::shared_ptr<video::scanVideo::graphics::command::CommandProcessor> commandProcessor;

    void SetUp() override {
        dummyLogger = std::make_shared<DummyLogger>();
        commandProcessor = std::make_shared<video::scanVideo::graphics::command::CommandProcessor>(dummyLogger);
        graphicsLib = std::make_shared<video::scanProgram::graphicsLib>(commandProcessor);
    }

    void TearDown() override {

    }

};

///We want to take the command processor and valgrind it for days for memory leaks.

// empty screen, fill with one shape, print out the screen as ascii art.

// empty screen, fill with one shape, delete the shape, print out the screen

// empty screen, print some text. print out the screen

// empty screen, print some text, clear the screen

// big memory saving will be to implement fill with line colour, and delete shapes
// affecting certain scanlines.

//as well, we need non-immediate graphics: addShape() needs to take a bool for
//"completeFrameOnCommand". default is true. if false, then let the commands get built up
//

// api to delete commands that affect scanlines. deletePendingCommandsForScanlineRange()
// addCommand() goes to addImmediateCommand() and addPendingCommand()
// every addPendingCommand() should return a token of sorts that allows the command to be
// deleted from the draw list. ??


//