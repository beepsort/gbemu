#include <stdio.h>
#include <SDL2/SDL.h>
#include <optional>
#include <string>

#include "render.h"
#include "gameboy/rom.h"
#include "gameboy/gameboy.h"
#include "gameboy/serial.h"
#include "gameboy/input.h"

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

std::optional<GAMEBOY::InputHandler::BUTTON> map_key(SDL_Keycode key)
{
    switch (key)
    {
        case SDLK_w:
            return std::make_optional(
                    GAMEBOY::InputHandler::BUTTON::UP);
        case SDLK_s:
            return std::make_optional(
                    GAMEBOY::InputHandler::BUTTON::DOWN);
        case SDLK_a:
            return std::make_optional(
                    GAMEBOY::InputHandler::BUTTON::LEFT);
        case SDLK_d:
            return std::make_optional(
                    GAMEBOY::InputHandler::BUTTON::RIGHT);
        case SDLK_q:
            return std::make_optional(
                    GAMEBOY::InputHandler::BUTTON::B);
        case SDLK_e:
            return std::make_optional(
                    GAMEBOY::InputHandler::BUTTON::A);
        case SDLK_RETURN:
            return std::make_optional(
                    GAMEBOY::InputHandler::BUTTON::START);
        case SDLK_BACKSPACE:
            return std::make_optional(
                    GAMEBOY::InputHandler::BUTTON::SELECT);
    }
    return {};
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
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "ROM file could not be read, aborting\n");
        return -1;
    }
    ROMDATA rom = romopt.value();
    auto str_begin = rom.cbegin() + GAMEBOY::TITLE_BEGIN;
    auto str_end = rom.cbegin() + GAMEBOY::TITLE_END;
    std::string title(str_begin, str_end);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,"Loaded: %s\n", title.c_str());
    auto& serialSupervisor = GAMEBOY::SerialEventSupervisor::getInstance();
    serialSupervisor.subscribe(GAMEBOY::SerialEventType::SERIAL_OUT, new SerialPrinter());
    GAMEBOY::LINE_BUFFERS line_buffers;
    GAMEBOY::InputHandler input_handler;
    GAMEBOY::Gameboy gameboy(rom, input_handler, line_buffers);
    SDL_Window* win = SDL_CreateWindow(
            "GBEMU",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            2*160,
            2*144,
            SDL_WINDOW_SHOWN);
    if (win == nullptr)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "SDL Window could not be created, aborting\n");
        return -1;
    }
    Renderer renderer(line_buffers, win);
    SDL_Event event;
    uint64_t frame_start;
    int64_t frame_time;
    const int64_t min_frame_time = 1000/60;
    bool quit = false;
    while (!quit)
    {
        frame_start = SDL_GetTicks64();
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
            {
                auto btn = map_key(event.key.keysym.sym);
                if (btn.has_value())
                {
                    input_handler.btn_down(btn.value());
                }
            }
            if (event.type == SDL_KEYUP && event.key.repeat == 0)
            {
                auto btn = map_key(event.key.keysym.sym);
                if (btn.has_value())
                {
                    input_handler.btn_up(btn.value());
                }
            }
        }
        for (int lines=0; lines<160;)
        {
            if (SDL_GetTicks64() - frame_start > min_frame_time * 2)
            {
                break;
            }
            if (gameboy.tick())
            {
                renderer.add_line();
                lines++;
            }
        }
        frame_time = SDL_GetTicks64() - frame_start;
        if (frame_time < min_frame_time)
        {
            SDL_Delay(min_frame_time - frame_time);
        }
    }
    SDL_Quit();
    return 0;
}
