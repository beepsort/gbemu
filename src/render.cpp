#include "render.h"
#include <array>

Renderer::Renderer(std::shared_ptr<std::array<uint8_t, 160>> line_buffer, SDL_Window* win)
: m_line_buffer(line_buffer)
{
    m_sdl_renderer = SDL_CreateRenderer(
            win,
            -1,
            SDL_RENDERER_ACCELERATED);
    m_sdl_texture = SDL_CreateTexture(
            m_sdl_renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_STREAMING,
            GB_W,
            GB_H);
    m_screen_buffer = new uint32_t[GB_W * GB_H];
}

Renderer::~Renderer()
{
    delete[] m_screen_buffer;
    // probably need to delete texture and renderer
    // check the order that they will need to be destroyed
    // (alongside the window too, unsure if there will be issues)
}

bool Renderer::add_line()
{
    SDL_LockTexture(m_sdl_texture, NULL, (void**)&m_screen_buffer, &m_buffer_pitch);
    int base_index = GB_W * m_current_line;
    // copy line to screen buffer until full
    for (int i=0; i<GB_W; i++)
    {
        uint32_t pixel;
        switch((*m_line_buffer)[i])
        {
            case 0:
                pixel = 0xFFFFFF88;
                break;
            case 1:
                pixel = 0xB8B8B888;
                break;
            case 2:
                pixel = 0x68686888;
                break;
            case 3:
                pixel = 0x00000088;
                break;
        }
        if (i<40) {

        }
        m_screen_buffer[base_index + i] = pixel;
    }
    SDL_UnlockTexture(m_sdl_texture);
    if (++m_current_line == GB_H)
    {
        // copy screen buffer to texture
        SDL_SetRenderDrawColor(m_sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(m_sdl_renderer);
        SDL_Rect render_quad = {0, 0, GB_W, GB_H};
        //SDL_RenderCopyEx(m_sdl_renderer, m_sdl_texture, NULL, &render_quad, 0.0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopy(m_sdl_renderer, m_sdl_texture, NULL, NULL);
        SDL_RenderPresent(m_sdl_renderer);
        m_current_line = 0;
        return true;
    }
    return false;
}
