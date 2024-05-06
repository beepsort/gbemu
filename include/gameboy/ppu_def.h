#ifndef __PPU_DEF_H__
#define __PPU_DEF_H__

#include <stdint.h>
#include <array>
#include <memory>
#include <optional>

namespace GAMEBOY
{
    typedef std::array<uint8_t, 160> LINE_PIXELS;
    typedef std::array<std::optional<uint8_t>,160> LINE_PIXELS_OPT;
    typedef struct _line_pixels
    {
        _line_pixels()
        {
            bg = std::make_shared<LINE_PIXELS>();
            sprite = std::make_shared<LINE_PIXELS_OPT>();
        }
        std::shared_ptr<LINE_PIXELS> bg;
        std::shared_ptr<LINE_PIXELS_OPT> sprite;
    } LINE_BUFFERS;
}

#endif
