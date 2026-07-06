cmake_minimum_required(VERSION 3.19)
project(AirSimYoloV8Nano LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

set(AIRSIM_ROOT "${CMAKE_SOURCE_DIR}/../AirSimCOTS")


set(AIRSIM_INCLUDE_DIRS
    ${AIRSIM_ROOT}/AirLib/include
    ${AIRSIM_ROOT}/rpc/include/rpc
    ${AIRSIM_ROOT}/MavLinkCom/include
)

set(AIRSIM_LIB_DIR "${AIRSIM_ROOT}/lib")

find_package(Qt6 6.5 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

qt_add_executable(AirSimYoloV8Nano main.cpp)

target_include_directories(AirSimYoloV8Nano PRIVATE
    ${AIRSIM_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
)

target_link_directories(AirSimYoloV8Nano PRIVATE
    ${AIRSIM_LIB_DIR}
    ${AIRSIM_ROOT}/AirLib/deps/rpclib/lib/x64/Release
)

target_link_libraries(AirSimYoloV8Nano PRIVATE
    Qt::Core
    ${OpenCV_LIBS}
    AirLib
    rpc
)

if(WIN32)
    target_link_libraries(AirSimYoloV8Nano PRIVATE wsock32 ws2_32)
endif()

install(TARGETS AirSimYoloV8Nano
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)