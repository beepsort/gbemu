#include <stdio.h>
#include <SDL2/SDL.h>
#include <optional>
#include <string>

#include "rom.h"
#include "cpu.h"
#include "serial.h"

class SerialPrinter: public GAMEBOY::SerialEventSubscriber
{
    void receive(uint8_t data)
    {
        printf("%c", (char)data);
        fflush(stdout);
    }
};

void display_help(char* exec_name)
{
    printf("Missing or incorrect launch parameters.\n");
    printf("Need rom file path to load.\n\n");
    printf("Usage: %s rom_file\n", exec_name);
}

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
    char* debug_env = std::getenv("DEBUG");
    if (debug_env != nullptr)
    {
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
    }
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
    auto str_begin = rom.cbegin() + GAMEBOY::TITLE_BEGIN;
    auto str_end = rom.cbegin() + GAMEBOY::TITLE_END;
    std::string title(str_begin, str_end);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,"Loaded: %s\n", title.c_str());
    auto& serialSupervisor = GAMEBOY::SerialEventSupervisor::getInstance();
    serialSupervisor.subscribe(GAMEBOY::SerialEventType::SERIAL_OUT, new SerialPrinter());
    GAMEBOY::Cpu gameboy(rom);
    gameboy.report();
    SDL_Event event;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        gameboy.tick();
        gameboy.report();
    }
    SDL_Quit();
    return 0;
}
