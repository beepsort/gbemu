#include <stdexcept>

#include "gbmemory.h"
#include "gbmemory_static.h"
#include "gbmemory_mbc1.h"
#include "gbmemory_mbc3.h"
#include "cpu_interrupt.h"

GAMEBOY::CartMapper* GAMEBOY::CartMapper::create_mapper(ROMDATA& rom)
{
    uint8_t mapper_type = rom[CART_TYPE];
    bool cartRam = false;
    bool cartBattery = false;
    bool cartTimer = false;
    switch (mapper_type)
    {
    case 0x00:
        return new MapperStatic(rom);
    case 0x03:
        cartBattery = true;
        [[fallthrough]];
    case 0x02:
        cartRam = true;
        [[fallthrough]];
    case 0x01:
        return new MapperMbc1(rom, cartRam, cartBattery);
    case 0x10:
        cartRam = true;
        [[fallthrough]];
    case 0x7F:
        cartTimer = true;
        cartBattery = true;
        [[fallthrough]];
    case 0x11:
        return new GAMEBOY::MapperMbc3(rom, cartRam, cartBattery, cartTimer);
    case 0x13:
        cartBattery = true;
        [[fallthrough]];
    case 0x12:
        cartRam = true;
        return new GAMEBOY::MapperMbc3(rom, cartRam, cartBattery, cartTimer);
    default:
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Unsupported mapper type: %d\n", mapper_type);
        throw std::logic_error("Unsupported mapped type");
    }
}

uint8_t GAMEBOY::AddressDispatcher::read(uint16_t addr)
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
        return ioHandler.read(addr);
    }
    else if (addr >= HRAM_LO && addr <= HRAM_HI)
    {
        return highRam[addr - HRAM_LO];
    }
    else if (addr == INTERRUPT_ENABLE)
    {
        return ioHandler.read(addr);
    }
    return 0x00;
}

void GAMEBOY::AddressDispatcher::write(uint16_t addr, uint8_t data)
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
        ioHandler.write(addr, data);
    }
    else if (addr >= HRAM_LO && addr <= HRAM_HI)
    {
        highRam[addr - HRAM_LO] = data;
    }
    else if (addr == INTERRUPT_ENABLE)
    {
        ioHandler.write(addr, data);
    }
}

GAMEBOY::AddressDispatcher::AddressDispatcher(ROMDATA& rom)
{
    cartMapper = CartMapper::create_mapper(rom);
}
