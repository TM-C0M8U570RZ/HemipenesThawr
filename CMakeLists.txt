cmake_minimum_required(VERSION 3.16)

project(HemipenesThawr LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(HemipenesThawr main.cpp)

target_include_directories(HemipenesThawr PUBLIC
    $ENV{TMC_SDK_PATH}/current/helpers/include
    $ENV{TMC_SDK_PATH}/current/include)

target_link_directories(HemipenesThawr PUBLIC
    $ENV{TMC_SDK_PATH}/current/lib/${CMAKE_SYSTEM_PROCESSOR})

target_link_libraries(HemipenesThawr PUBLIC -lTmcHelpers -lTmcSdkCore)

include(GNUInstallDirs)
install(TARGETS HemipenesThawr
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
