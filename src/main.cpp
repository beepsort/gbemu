#include <stdio.h>
#include <SDL2/SDL.h>
#include <optional>
#include <string>

#include "rom.h"
#include "cpu.h"

void display_help(char* exec_name)
{
    printf("Missing or incorrect launch parameters.\n");
    printf("Need rom file path to load.\n\n");
    printf("Usage: %s rom_file\n", exec_name);
}

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
    if (argc!=2)
    {
        display_help(argv[0]);
        return 0;
    }
    std::optional<ROMDATA> romopt = open_rom(argv[1]);
    if (!romopt.has_value())
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "ROM file could not be read, aborting.\n");
        return -1;
    }
    ROMDATA rom = romopt.value();
    auto str_begin = rom.cbegin() + ROM::TITLE_BEGIN;
    auto str_end = rom.cbegin() + ROM::TITLE_END;
    std::string title(str_begin, str_end);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,"Loaded: %s\n", title.c_str());
    CPU::Cpu gameboy(rom);
    SDL_Quit();
    return 0;
}