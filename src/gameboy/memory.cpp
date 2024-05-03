#include <stdexcept>

#include "gameboy/memory.h"
#include "gameboy/memory_static.h"
#include "gameboy/memory_mbc1.h"
#include "gameboy/memory_mbc3.h"
#include "gameboy/cpu_interrupt.h"

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

uint8_t GAMEBOY::AddressDispatcher::read(uint16_t addr, MemoryAccessSource src)
{
    if (addr >= CART_ROM_LO && addr <= CART_ROM_HI)
    {
        if (dmaLocked && src!=MemoryAccessSource::DMA)
        {
            return 0xFF;
        }
        return cartMapper->read(addr);
    }
    else if (addr >= VRAM_LO && addr <= VRAM_HI)
    {
        if (vramLocked && src!=MemoryAccessSource::PPU)
        {
            return 0xFF; // return garbage
        }
        if (dmaLocked && src!=MemoryAccessSource::DMA)
        {
            return 0xFF;
        }
        return videoRam[addr - VRAM_LO];
    }
    else if (addr >= CART_RAM_LO && addr <= CART_RAM_HI)
    {
        if (dmaLocked && src!=MemoryAccessSource::DMA)
        {
            return 0xFF;
        }
        return cartMapper->read(addr);
    }
    else if (addr >= WRAM_LO && addr <= WRAM_HI)
    {
        if (dmaLocked && src!=MemoryAccessSource::DMA)
        {
            return 0xFF;
        }
        return workRam[addr - WRAM_LO];
    }
    else if (addr >= OAM_LO && addr <= OAM_HI)
    {
        if (dmaLocked && src !=MemoryAccessSource::DMA)
        {
            return 0xFF;
        }
        if (oamLocked && src!=MemoryAccessSource::PPU && src!=MemoryAccessSource::DMA)
        {
            return 0xFF; // return garbage
        }
        return oam[addr - OAM_LO];
    }
    else if (addr >= IO_REG_LO && addr <= IO_REG_HI)
    {
        return ioHandler.read(addr, src);
    }
    else if (addr >= HRAM_LO && addr <= HRAM_HI)
    {
        return highRam[addr - HRAM_LO];
    }
    else if (addr == INTERRUPT_ENABLE)
    {
        return ioHandler.read(addr, src);
    }
    return 0x00;
}

void GAMEBOY::AddressDispatcher::write(uint16_t addr, uint8_t data, MemoryAccessSource src)
{
    if (addr >= CART_ROM_LO && addr <= CART_ROM_HI)
    {
        if (dmaLocked && src!=MemoryAccessSource::DMA)
        {
            return;
        }
        cartMapper->write(addr, data);
    }
    else if (addr >= VRAM_LO && addr <= VRAM_HI)
    {
        if (dmaLocked && src!=MemoryAccessSource::DMA)
        {
            return;
        }
        if (vramLocked && src!=MemoryAccessSource::PPU)
        {
            return; // ignore write
        }
        videoRam[addr - VRAM_LO] = data;
    }
    else if (addr >= CART_RAM_LO && addr <= CART_RAM_HI)
    {
        if (dmaLocked && src!=MemoryAccessSource::DMA)
        {
            return;
        }
        cartMapper->write(addr, data);
    }
    else if (addr >= WRAM_LO && addr <= WRAM_HI)
    {
        if (dmaLocked && src!=MemoryAccessSource::DMA)
        {
            return;
        }
        workRam[addr - WRAM_LO] = data;
    }
    else if (addr >= OAM_LO && addr <= OAM_HI)
    {
        if (dmaLocked && src!=MemoryAccessSource::DMA)
        {
            return;
        }
        if (oamLocked && src!=MemoryAccessSource::PPU && src!=MemoryAccessSource::DMA)
        {
            return; // ignore write
        }
        oam[addr - OAM_LO] = data;
    }
    else if (addr >= IO_REG_LO && addr <= IO_REG_HI)
    {
        ioHandler.write(addr, data, src);
    }
    else if (addr >= HRAM_LO && addr <= HRAM_HI)
    {
        highRam[addr - HRAM_LO] = data;
    }
    else if (addr == INTERRUPT_ENABLE)
    {
        ioHandler.write(addr, data, src);
    }
}

GAMEBOY::AddressDispatcher::AddressDispatcher(ROMDATA& rom, InputHandler& input_handler)
: ioHandler(input_handler)
{
    cartMapper = CartMapper::create_mapper(rom);
}

void GAMEBOY::AddressDispatcher::lock(GAMEBOY::AddressDispatcher::LOCKABLE target)
{
    switch (target)
    {
        case LOCKABLE::VRAM:
            vramLocked = true;
            return;
        case LOCKABLE::OAM:
            oamLocked = true;
            return;
        case LOCKABLE::ALL_DMA:
            dmaLocked = true;
            return;
        default:
            return;
    }
}

void GAMEBOY::AddressDispatcher::unlock(GAMEBOY::AddressDispatcher::LOCKABLE target)
{
    switch (target)
    {
        case LOCKABLE::VRAM:
            vramLocked = false;
            return;
        case LOCKABLE::OAM:
            oamLocked = false;
            return;
        case LOCKABLE::ALL_DMA:
            dmaLocked = false;
        default:
            return;
    }
}
