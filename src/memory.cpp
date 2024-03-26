#include <stdexcept>

#include "memory.h"
#include "memory_static.h"
#include "memory_mbc1.h"
#include "memory_mbc3.h"

MEMORY::CartMapper* MEMORY::CartMapper::create_mapper(ROMDATA& rom)
{
    uint8_t mapper_type = rom[ROM::CART_TYPE];
    bool cartRam = false;
    bool cartBattery = false;
    bool cartTimer = false;
    switch (mapper_type)
    {
    case 0x00:
        return new MEMORY::MapperStatic(rom);
    case 0x03:
        cartBattery = true;
    case 0x02:
        cartRam = true;
    case 0x01:
        return new MEMORY::MapperMbc1(rom, cartRam, cartBattery);
    case 0x10:
        cartRam = true;
    case 0x7F:
        cartTimer = true;
        cartBattery = true;
    case 0x11:
        return new MEMORY::MapperMbc3(rom, cartRam, cartBattery, cartTimer);
    case 0x13:
        cartBattery = true;
    case 0x12:
        cartRam = true;
        return new MEMORY::MapperMbc3(rom, cartRam, cartBattery, cartTimer);
    default:
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Unsupported mapper type: %d\n", mapper_type);
        throw std::logic_error("Unsupported mapped type");
    }
}

uint8_t MEMORY::AddressDispatcher::read(uint16_t addr)
{
    if (addr >= CART_ROM_LO && addr <= CART_ROM_HI)
    {
        return cartMapper->read(addr);
    }
    else if (addr >= VRAM_LO && addr <= VRAM_HI)
    {
        // TODO: Check VRAM memory behaviour
        return videoRam[addr - VRAM_LO];
    }
    else if (addr >= CART_RAM_LO && addr <= CART_RAM_HI)
    {
        return cartMapper->read(addr);
    }
    else if (addr >= WRAM_LO && addr <= WRAM_HI)
    {
        return workRam[addr - WRAM_LO];
    }
    else if (addr >= OAM_LO && addr <= OAM_HI)
    {
        // TODO: Check OAM behaviour
        return 0x00;
    }
    else if (addr >= IO_REG_LO && addr <= IO_REG_HI)
    {
        // TODO: Implement IO registers
        return 0x00;
    }
    else if (addr >= HRAM_LO && addr <= HRAM_HI)
    {
        return highRam[addr - HRAM_LO];
    }
    else if (addr == INTERRUPT_ENABLE)
    {
        // TODO: Check interrupt enable register behaviour
        return 0x00;
    }
    return 0x00;
}

void MEMORY::AddressDispatcher::write(uint16_t addr, uint8_t data)
{
    if (addr >= CART_ROM_LO && addr <= CART_ROM_HI)
    {
        cartMapper->write(addr, data);
    }
    else if (addr >= VRAM_LO && addr <= VRAM_HI)
    {
        // TODO: Check VRAM memory behaviour
        videoRam[addr - VRAM_LO] = data;
    }
    else if (addr >= CART_RAM_LO && addr <= CART_RAM_HI)
    {
        cartMapper->write(addr, data);
    }
    else if (addr >= WRAM_LO && addr <= WRAM_HI)
    {
        workRam[addr - WRAM_LO] = data;
    }
    else if (addr >= OAM_LO && addr <= OAM_HI)
    {
        // TODO: Check OAM behaviour
        // do nothing
    }
    else if (addr >= IO_REG_LO && addr <= IO_REG_HI)
    {
        // TODO: Implement IO registers
        // do nothing
    }
    else if (addr >= HRAM_LO && addr <= HRAM_HI)
    {
        highRam[addr - HRAM_LO] = data;
    }
    else if (addr == INTERRUPT_ENABLE)
    {
        // TODO: Check interrupt enable register behaviour
        // do nothing
    }
}


MEMORY::AddressDispatcher::AddressDispatcher(ROMDATA& rom)
{
    cartMapper = CartMapper::create_mapper(rom);
}
