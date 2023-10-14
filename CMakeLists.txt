cmake_minimum_required(VERSION 3.24)
project(e39_rpi_fw_unittest)

set(CMAKE_CXX_STANDARD 17)


#add_library(e39_rpi_fw_base)


add_executable(e39_rpi_fw_unittest
        main.cpp
        unittests/ibus/dma/Packetizer_Test.cpp
        unittests/ibus/outputWriterApi/IbusLoggerOutputTest.cpp
        unittests/observerRegistry/observers/BaseObserverPiToPicoMessageDecodeTests.cpp


        e39-rpi-fw/ibus/dma/Packetizer.cpp
        )


target_include_directories(
        e39_rpi_fw_unittest
        PUBLIC
        e39-rpi-fw/ibus/dma/Packetizer.h
)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)



add_subdirectory(e39-rpi-fw/libs/fmt)
add_subdirectory(e39-rpi-fw/libs/nanopb)
add_subdirectory(e39-rpi-fw/libs/nanopb_cpp)

target_link_libraries(e39_rpi_fw_unittest
        fmt::fmt-header-only
        GTest::gtest_main
        #        nanopb
        #        nanopb_cpp
        )

enable_testing()
include(GoogleTest)
gtest_discover_tests(e39_rpi_fw_unittest)
