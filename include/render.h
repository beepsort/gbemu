#ifndef __DRAW_FRAME_H__
#define __DRAW_FRAME_H__

#include <memory>
#include <SDL2/SDL.h>
#include "gameboy/ppu_def.h"

class Renderer
{
private:
    const int GB_W = 160;
    const int GB_H = 144;
    std::shared_ptr<GAMEBOY::LINE_PIXELS> m_line_buffer;
    uint32_t* m_screen_buffer;
    int m_buffer_pitch; // number of bytes per row, set by sdl
    int m_current_line = 0;
    SDL_Renderer* m_sdl_renderer;
    SDL_Texture* m_sdl_texture;
public:
    Renderer(std::shared_ptr<GAMEBOY::LINE_PIXELS> line_buffer, SDL_Window* win);
    ~Renderer();
    bool add_line();
};

#endif
