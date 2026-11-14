cmake_minimum_required(VERSION 3.24)
project(e39_rpi_fw_unittest)

# Build with AddressSanitizer + UBSan + libstdc++ bounds assertions, so out-of-bounds
# reads/writes fail the test run instead of passing silently.
option(E39_UNITTEST_SANITIZERS "Build tests with ASan/UBSan and _GLIBCXX_ASSERTIONS" ON)
if(E39_UNITTEST_SANITIZERS)
    # -fno-sanitize-recover: a UBSan report aborts the test run instead of just printing.
    add_compile_options(-fsanitize=address,undefined -fno-sanitize-recover=undefined -fno-omit-frame-pointer -D_GLIBCXX_ASSERTIONS)
    add_link_options(-fsanitize=address,undefined)
endif()

# graphicsLib/CommandProcessor include Pico SDK headers (pico/types.h, pico/mutex.h) that
# don't exist on the host yet. Off until there's a host shim for them.
option(E39_UNITTEST_GRAPHICS "Build the graphicsLib tests (needs Pico SDK host shims)" OFF)

set(CMAKE_CXX_STANDARD 17)


#add_library(e39_rpi_fw_base)


add_executable(e39_rpi_fw_unittest
        main.cpp

        e39-rpi-fw/libs/nanopb/pb.h
        e39-rpi-fw/libs/nanopb/pb_common.h
        e39-rpi-fw/libs/nanopb/pb_common.c
        e39-rpi-fw/libs/nanopb/pb_decode.h
        e39-rpi-fw/libs/nanopb/pb_decode.c
        e39-rpi-fw/libs/nanopb/pb_encode.h
        e39-rpi-fw/libs/nanopb/pb_encode.c
        e39-rpi-fw/libs/nanopb_cpp/nanopb_cpp.h
        e39-rpi-fw/libs/nanopb_cpp/nanopb_cpp.cpp

        e39-rpi-fw/ibus/data/IbusPacket.cpp
        unittests/ibus/data/IbusDataTest.cpp

        unittests/DummyLogger.cpp

        unittests/ibus/dma/Packetizer_Test.cpp
        e39-rpi-fw/ibus/dma/Packetizer.cpp

        unittests/ibus/outputWriterApi/IbusLoggerOutputTest.cpp



        unittests/observerRegistry/observers/BaseObserverPiToPicoMessageDecodeTests.cpp
        unittests/observerRegistry/observers/PiToPico/PicoConfigurationReadRequestTest.cpp
        unittests/observerRegistry/observers/PiToPico/PiPowerRequestTest.cpp
        unittests/observerRegistry/observers/PiToPico/VideoSwitchRequestTest.cpp
        unittests/observerRegistry/observers/PiToPico/HeartBeatRequestObserverTest.cpp
        e39-rpi-fw/ibus/observerRegistry/observers/BaseObserver.cpp
        e39-rpi-fw/ibus/observerRegistry/observers/PiToPico/heartbeatRequest/HeatbeatRequestObserver.cpp
        e39-rpi-fw/ibus/observerRegistry/observers/PiToPico/picoConfigurationReadRequest/PicoConfigurationReadRequestObserver.cpp
        e39-rpi-fw/ibus/observerRegistry/observers/PiToPico/PiPowerRequest/PiPowerRequestObserver.cpp
        e39-rpi-fw/ibus/observerRegistry/observers/PiToPico/VideoRequest/PicoVideoRequestObserver.cpp
        e39-rpi-fw/configuration/Configuration.cpp
        e39-rpi-fw/configuration/ConfigurationManager.cpp


        e39-rpi-fw/proto_cpp/ConfigMessage.cpp
        e39-rpi-fw/proto_cpp/PicoToPi.cpp
        e39-rpi-fw/proto_cpp/PiToPico.cpp
        e39-rpi-fw/proto_generated/ConfigProto.pb.c
        e39-rpi-fw/proto_generated/PicoToPi.pb.c
        e39-rpi-fw/proto_generated/PiToPico.pb.c


        unittests/observerRegistry/observers/PiToPico/HeartBeatRequestObserverTest.cpp

        unittests/ibus/outputWriterApi/MockDmaManager.h
        unittests/ibus/outputWriterApi/SoftPowerWriterTest.cpp
        unittests/ibus/outputWriterApi/HeartbeatResponseWriterTest.cpp
        e39-rpi-fw/ibus/outputWriterApi/BaseOutputWriter.cpp
        e39-rpi-fw/ibus/outputWriterApi/PicoToPi/heartbeatResponseWriter/HeartbeatResponseWriter.cpp
        e39-rpi-fw/ibus/outputWriterApi/PicoToPi/picoPiSoftPowerRequest/SoftPowerRequestWriter.cpp
        e39-rpi-fw/ibus/outputWriterApi/PicoToPi/picoConfigurationStatusWriter/ConfigurationStatusWriter.cpp

        # Classes whose vtable/typeinfo live in these .cpp files (needed by UBSan's vptr check)
        e39-rpi-fw/logging/BaseLogger.cpp
        e39-rpi-fw/configuration/store/IConfigurationStore.cpp
        e39-rpi-fw/configuration/store/memory/MemoryConfigurationStore.cpp
        e39-rpi-fw/configuration/store/ibusConfigMessageStore/IBusConfigMessageStore.cpp
        e39-rpi-fw/configuration/default/DefaultConfigurationProvider.cpp
        e39-rpi-fw/configuration/default/PimoroniDefaultConfigurationProvider.cpp

        # Flash config store against a fake flash block (unittests/hoststubs)
        unittests/hoststubs/HostFlash.cpp
        e39-rpi-fw/configuration/store/flash/FlashConfigurationStore.cpp
        unittests/configuration/FlashConfigurationStoreTest.cpp

        # Bounds / memory-safety regression tests (e39-rpi-fw TODO_MEMORY_BUGS.md)
        unittests/ibus/dma/Packetizer_Bounds_Test.cpp
        unittests/ibus/data/IbusPacket_Bounds_Test.cpp
        unittests/observerRegistry/observers/PiToPicoDecodeTest.cpp
        unittests/video/scanPrograms/graphicsLib/ScanlineEncoderTest.cpp
        unittests/video/scanPrograms/graphicsLib/TextAndContainerCommandTest.cpp

        # graphicsLib pieces that don't need Pico SDK headers
        e39-rpi-fw/video/scanProgram/scanPrograms/graphicsLib/command/BaseCommand.cpp
        e39-rpi-fw/video/scanProgram/scanPrograms/graphicsLib/command/PxCoord.cpp
        e39-rpi-fw/video/scanProgram/scanPrograms/graphicsLib/command/RleRun.cpp
        e39-rpi-fw/video/scanProgram/scanPrograms/graphicsLib/command/RleRunContainerCommand.cpp
        e39-rpi-fw/video/scanProgram/scanPrograms/graphicsLib/command/TextCommand.cpp
        e39-rpi-fw/video/scanProgram/scanPrograms/graphicsLib/fonts/FontProvider.cpp

        )


# The firmware includes <CMakeHasVideoFlags.h>; generate it from the firmware's own template
# with video support off (CMAKE_HAS_VIDEO_SUPPORT left undefined).
configure_file(
        e39-rpi-fw/video/CMakeHasVideoFlags.in.h
        ${CMAKE_CURRENT_BINARY_DIR}/generated/CMakeHasVideoFlags.h
)

target_include_directories(
        e39_rpi_fw_unittest
        PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}/generated
        # Host stand-ins for hardware/flash.h, hardware/sync.h, pico/multicore.h
        unittests/hoststubs

        e39-rpi-fw/
        e39-rpi-fw/libs/nanopb_cpp
        e39-rpi-fw/libs/nanopb
        e39-rpi-fw/libs/fmt

        unittests/
)

if(E39_UNITTEST_GRAPHICS)
    target_sources(e39_rpi_fw_unittest PRIVATE
        unittests/video/scanPrograms/graphicsLib/graphicsLibLeakTest.cpp
        e39-rpi-fw/video/scanProgram/scanPrograms/graphicsLib/graphicsLib.h
        e39-rpi-fw/video/scanProgram/scanPrograms/graphicsLib/graphicsLib.cpp
        e39-rpi-fw/video/scanProgram/scanPrograms/graphicsLib/DisplayMetrics.h
        e39-rpi-fw/video/scanProgram/scanPrograms/graphicsLib/DisplayMetrics.cpp
        e39-rpi-fw/video/scanProgram/scanPrograms/graphicsLib/command/CommandProcessor.h
        e39-rpi-fw/video/scanProgram/scanPrograms/graphicsLib/command/CommandProcessor.cpp
        e39-rpi-fw/video/scanProgram/scanPrograms/graphicsLib/command/EmptyRectangleCommand.h
        e39-rpi-fw/video/scanProgram/scanPrograms/graphicsLib/command/EmptyRectangleCommand.cpp
        e39-rpi-fw/video/scanProgram/scanPrograms/graphicsLib/command/FilledRectangleCommand.h
        e39-rpi-fw/video/scanProgram/scanPrograms/graphicsLib/command/FilledRectangleCommand.cpp
        e39-rpi-fw/video/scanProgram/scanPrograms/graphicsLib/command/LineCommand.h
        e39-rpi-fw/video/scanProgram/scanPrograms/graphicsLib/command/LineCommand.cpp
        e39-rpi-fw/pico-extras/src/common/pico_scanvideo/include/pico/scanvideo/scanvideo_base.h
        e39-rpi-fw/pico-extras/src/common/pico_scanvideo/include/pico/scanvideo/composable_scanline.h
    )
    target_include_directories(e39_rpi_fw_unittest PUBLIC
            e39-rpi-fw/pico-extras/src/common/pico_scanvideo/include)
endif()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)



add_subdirectory(e39-rpi-fw/libs/fmt)
#add_subdirectory(e39-rpi-fw/libs/nanopb)
#add_subdirectory(e39-rpi-fw/libs/nanopb_cpp)

target_link_libraries(e39_rpi_fw_unittest
        fmt::fmt-header-only
        GTest::gtest_main
        gmock_main
        #        nanopb
        #        nanopb_cpp
        )

enable_testing()
include(GoogleTest)
gtest_discover_tests(e39_rpi_fw_unittest)
