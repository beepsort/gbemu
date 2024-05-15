#include "gameboy/memory_mbc3.h"

GAMEBOY::MapperMbc3::MapperMbc3(ROMDATA& rom, bool cartRam, [[maybe_unused]] bool cartBattery, [[maybe_unused]] bool cartTimer)
{
    uint8_t ram_bank_count = 0;
    uint8_t rom_bank_count = 2;
    if (cartRam)
    {
        // detect amount of ram
        uint8_t ram_size_index = rom[0x0149];
        switch (ram_size_index)
        {
            case 0x00:
                ram_bank_count = 0;
                break;
            case 0x02:
                ram_bank_count = 1;
                break;
            case 0x03:
                ram_bank_count = 4;
                break;
            case 0x04:
                ram_bank_count = 16;
                break;
            case 0x05:
                ram_bank_count = 8;
                break;
            default:
                // invalid
                ram_bank_count = 0;
        }
    }
    uint8_t rom_size_index = rom[0x0148];
    switch (rom_size_index)
    {
        case 0x00:
            rom_bank_count = 2;
            break;
        case 0x01:
            rom_bank_count = 4;
            break;
        case 0x02:
            rom_bank_count = 8;
            break;
        case 0x03:
            rom_bank_count = 16;
            break;
        case 0x04:
            rom_bank_count = 32;
            break;
        case 0x05:
            rom_bank_count = 64;
            break;
        case 0x06:
            rom_bank_count = 128;
            break;
    }
    m_rambanks = std::vector<std::array<uint8_t, 0x2000>>(ram_bank_count);
    m_rombanks = std::vector<std::array<uint8_t, 0x4000>>(rom_bank_count);
    // init rom banks
    auto src_it = rom.cbegin();
    auto src_end = rom.cend();
    for (uint8_t i=0; i<m_rombanks.size(); i++)
    {
        auto& rom_bank = m_rombanks[i];
        auto dest_it = rom_bank.begin();
        auto dest_end = rom_bank.end();
        while (src_it != src_end && dest_it != dest_end) // while neither end reached
        {
            *dest_it = *src_it;
            src_it++;
            dest_it++;
        }
    }
}

uint8_t GAMEBOY::MapperMbc3::read(uint16_t addr)
{
    if (addr <= 0x3FFF)
    {
        // ROM bank 0
        auto rom_bank = m_rombanks[0];
        return rom_bank[addr];
    }
    else if (addr >= 0x4000 && addr <= 0x7FFF)
    {
        // Selected ROM bank
        auto rom_bank = m_rombanks[m_sel_rom_bank];
        return rom_bank[addr - 0x4000];
    }
    else if (m_ram_enable && addr >= 0xA000 && addr <= 0xBFFF)
    {
        auto ram_bank = m_rambanks[m_sel_ram_bank];
        return ram_bank[addr - 0xA000];
    }
    return 0;
}

void GAMEBOY::MapperMbc3::write(uint16_t addr, uint8_t data)
{
    if (addr <= 0x1FFF)
    {
        // RAM & Timer enable register
        // enabled if lsb is 0xA
        // otherwise disabled
        m_ram_enable = 0x0A == (data & 0x0F);
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF)
    {
        // ROM bank select
        uint8_t rom_bank_max = m_rombanks.size();
        m_sel_rom_bank = data % rom_bank_max;
        if (m_sel_rom_bank == 0)
        {
            m_sel_rom_bank = 1;
        }
    }
    else if (addr >= 0x4000 && addr <= 0x5FFF)
    {
        // RAM bank select (or RTC select)
        if (data <= 0x03)
        {
            // RAM bank select
            uint8_t ram_bank_max = m_rambanks.size();
            m_sel_ram_bank = data % ram_bank_max;
        }
        else if (data >= 0x08 && data <= 0x0C)
        {
            // RTC select
            // TODO
        }
    }
    else if (m_ram_enable && addr >= 0xA000 && addr <= 0xBFFF)
    {
        auto ram_bank = m_rambanks[m_sel_ram_bank];
        ram_bank[addr - 0xA000] = data;
    }
}
