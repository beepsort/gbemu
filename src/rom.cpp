#include <SDL2/SDL_rwops.h>
#include <SDL2/SDL_log.h>

#include "rom.h"

std::optional<ROMDATA> open_rom(char* rom_path) {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading rom file: %s\n", rom_path);
    SDL_RWops* file = SDL_RWFromFile(rom_path, "rb");
    if (file==NULL)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to open file\n");
        return {}; // File failed to open
    }
    size_t rom_size = SDL_RWsize(file);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Rom file size: %lu\n", rom_size);
    if (rom_size > 10*1000*1000)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Rom size over 10MB not allowed\n");
        return {}; // File size over 10MB is likely invalid
    }
    ROMDATA rom(rom_size);
    for (size_t i=0; i<rom_size; i++)
    {
        rom[i] = SDL_ReadU8(file);
    }
    SDL_RWclose(file);
    return rom;
}

int num_rom_banks(ROMDATA& data)
{
    uint8_t rom_size_byte = data[GAMEBOY::ROM_SIZE];
    uint16_t bank_count = round(pow(2, rom_size_byte+1));
    return bank_count;
}

int num_ram_banks(ROMDATA& data)
{
    uint8_t ram_size_byte = data[GAMEBOY::RAM_SIZE];
    switch (ram_size_byte)
    {
    case 0x02:
        return 1;
    case 0x03:
        return 4;
    case 0x04:
        return 16;
    case 0x05:
        return 8;
    default:
        return 0;
    }
}
