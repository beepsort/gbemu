cmake_minimum_required(VERSION 3.14)
add_library(gameboy cpu.cpp cpu_instruction_alu.cpp cpu_instruction_control.cpp cpu_instruction_decode.cpp cpu_instruction_load.cpp cpu_instruction_misc.cpp gbmemory.cpp gbmemory_mbc1.cpp gbmemory_mbc3.cpp gbmemory_static.cpp rom.cpp)
add_executable(gbemu main.cpp)
target_link_libraries(gbemu gameboy SDL2)

