cmake_minimum_required(VERSION 3.14)
add_compile_options(-Wall -Wextra -pedantic)
add_library(gameboy
    gameboy/cpu.cpp
    gameboy/cpu_instruction_alu.cpp
    gameboy/cpu_instruction_control.cpp
    gameboy/cpu_instruction_decode.cpp
    gameboy/cpu_instruction_load.cpp
    gameboy/cpu_instruction_misc.cpp
    gameboy/cpu_interrupt.cpp
    gameboy/memory.cpp
    gameboy/memory_dma.cpp
    gameboy/memory_io.cpp
    gameboy/memory_mbc1.cpp
    gameboy/memory_mbc3.cpp
    gameboy/memory_static.cpp
    gameboy/rom.cpp
    gameboy/serial.cpp
    gameboy/timer.cpp
    gameboy/ppu.cpp
    gameboy/ppu_tile.cpp
    gameboy/ppu_sprite.cpp
    gameboy/input.cpp
    gameboy/gameboy.cpp
    )
target_include_directories(gameboy PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
add_executable(gbemu main.cpp render.cpp)
target_link_libraries(gbemu gameboy SDL2)

