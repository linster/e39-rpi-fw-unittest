cmake_minimum_required(VERSION 3.24)
project(e39_rpi_fw_unittest)

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

        unittests/ibus/outputWriterApi/SoftPowerWriterTest.cpp
        e39-rpi-fw/ibus/outputWriterApi/BaseOutputWriter.cpp
        e39-rpi-fw/ibus/outputWriterApi/PicoToPi/heartbeatResponseWriter/HeartbeatResponseWriter.cpp
        e39-rpi-fw/ibus/outputWriterApi/PicoToPi/picoPiSoftPowerRequest/SoftPowerRequestWriter.cpp
        e39-rpi-fw/ibus/outputWriterApi/PicoToPi/picoConfigurationStatusWriter/ConfigurationStatusWriter.cpp

        )


target_include_directories(
        e39_rpi_fw_unittest
        PUBLIC

        e39-rpi-fw/
        e39-rpi-fw/libs/nanopb_cpp
        e39-rpi-fw/libs/nanopb
        e39-rpi-fw/libs/fmt

#        e39-rpi-fw/ibus/data/IbusPacket.h

        unittests/
#        e39-rpi-fw/logging/BaseLogger.h
#
#        e39-rpi-fw/ibus/dma/Packetizer.h
#        e39-rpi-fw/ibus/observerRegistry/observers/PiToPico/heartbeatRequest/HeatbeatRequestObserver.h
#        e39-rpi-fw/ibus/observerRegistry/observers/PiToPico/picoConfigurationReadRequest/PicoConfigurationReadRequestObserver.h
#        e39-rpi-fw/ibus/observerRegistry/observers/PiToPico/PiPowerRequest/PiPowerRequestObserver.h
#        e39-rpi-fw/ibus/observerRegistry/observers/PiToPico/VideoRequest/PicoVideoRequestObserver.h
#        e39-rpi-fw/ibus/data/IBusDevices.h
#        e39-rpi-fw/proto_cpp/ConfigMessage.h
#        e39-rpi-fw/proto_cpp/PicoToPi.h
#        e39-rpi-fw/proto_cpp/PiToPico.h
#        e39-rpi-fw/proto_generated/ConfigProto.pb.h
#        e39-rpi-fw/proto_generated/PicoToPi.pb.h
#        e39-rpi-fw/proto_generated/PiToPico.pb.h
#
#
#        e39-rpi-fw/ibus/outputWriterApi/PicoToPi/heartbeatResponseWriter/HeartbeatResponseWriter.h
#        e39-rpi-fw/ibus/outputWriterApi/PicoToPi/picoPiSoftPowerRequest/SoftPowerRequestWriter.h
)

#target_link_directories()

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
