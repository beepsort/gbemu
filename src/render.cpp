#include "render.h"
#include <array>

Renderer::Renderer(GAMEBOY::LINE_BUFFERS& line_buffers, SDL_Window* win)
: m_line_buffers(line_buffers)
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

inline uint32_t gb2bpp_to_rgba(uint8_t gbpix)
{
    switch(gbpix)
    {
        case 0:
            return 0xFFFFFF88;
        case 1:
            return 0xB8B8B888;
        case 2:
            return 0x68686888;
        case 3:
            return 0x00000088;
        default:
            return 0xFFFFFF88;
    }
}

bool Renderer::add_line()
{
    SDL_LockTexture(m_sdl_texture, NULL, (void**)&m_screen_buffer, &m_buffer_pitch);
    int base_index = GB_W * m_current_line;
    // copy line to screen buffer until full
    for (int i=0; i<GB_W; i++)
    {
        uint32_t pixel = gb2bpp_to_rgba((*m_line_buffers.bg)[i]);
        auto sprite_pix = (*m_line_buffers.sprite)[i];
        if (sprite_pix.has_value())
        {
            pixel = gb2bpp_to_rgba(sprite_pix.value());
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
