cmake_minimum_required(VERSION 3.14)
add_executable(gbemu_test
    gameboy/cpu_init_helper.cpp
    gameboy/cpu_init_helper.h
    gameboy/cpu_instruction_alu_test.cpp
    gameboy/cpu_instruction_load_test.cpp
    gameboy/cpu_instruction_control_test.cpp
    gameboy/cpu_instruction_misc_test.cpp
    gameboy/cpu_interrupt_test.cpp
    gameboy/ppu_test.cpp
    )
find_package(GTest REQUIRED)
target_include_directories(gbemu_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(gbemu_test gameboy SDL2 GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(gbemu_test)
