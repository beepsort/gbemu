#include "gameboy/memory.h"
#include "gameboy/cpu_interrupt.h"
#include "gameboy/serial.h"
#include "gameboy/timer.h"

uint8_t GAMEBOY::IOHandler::read(uint16_t addr, MemoryAccessSource src)
{
    switch (addr)
    {
        case INPUT_JOYP:
            return m_input_handler.joyp(
                    ~ioRam[0] & 0x20,
                    ~ioRam[0] & 0x10);
        case TIMER_REG_DIV:
            return Timer::getInstance().read(Timer::Register::DIV);
        case TIMER_REG_TIMA:
            return Timer::getInstance().read(Timer::Register::TIMA);
        case TIMER_REG_TMA:
            return Timer::getInstance().read(Timer::Register::TMA);
        case TIMER_REG_TAC:
            return Timer::getInstance().read(Timer::Register::TAC);
        case INTERRUPT_REG_IF:
        {
            uint8_t joyp_irq = m_input_handler.poll_irq();
            return (joyp_irq & 0x10)
                | (ioRam[addr - 0xFF00] & 0xEF);
        }
        case PPU_REG_DMA:
            if (src!=MemoryAccessSource::DMA)
            {
                return 0xFF;
            }
            return ioRam[addr - 0xFF00];
        case 0xFFFF:
            return IE;
        default:
            if (addr < 0xFF00 || addr > 0xFF80)
            {
                return 0xFF;
            }
            return ioRam[addr - 0xFF00];
    }
}

void GAMEBOY::IOHandler::write(uint16_t addr, uint8_t data, MemoryAccessSource src)
{
    switch (addr)
    {
        case SERIAL_CONTROL:
            if (data&0x80)
            {
                // TODO: delay output, trigger interrupt, collect input (very low priority)
                // begin transfer
                GAMEBOY::SerialEventSupervisor& events = GAMEBOY::SerialEventSupervisor::getInstance();
                events.publish(GAMEBOY::SerialEventType::SERIAL_OUT, ioRam[SERIAL_DATA - 0xFF00]);
                data &= 0x7F;
            }
            ioRam[SERIAL_CONTROL - 0xFF00] = data;
            break;
        case TIMER_REG_DIV:
            Timer::getInstance().write(Timer::Register::DIV, data);
            break;
        case TIMER_REG_TIMA:
            Timer::getInstance().write(Timer::Register::TIMA, data);
            break;
        case TIMER_REG_TMA:
            Timer::getInstance().write(Timer::Register::TMA, data);
            break;
        case TIMER_REG_TAC:
            Timer::getInstance().write(Timer::Register::TAC, data);
            break;
        case PPU_REG_LY:
            if (src==MemoryAccessSource::PPU)
            {
                ioRam[addr - 0xFF00] = data;
            }
            break;
        case PPU_REG_STAT:
        {
            if (src==MemoryAccessSource::PPU)
            {
                ioRam[addr - 0xFF00] = data;
                break;
            }
            // keep bits 0-2, as not writable
            uint8_t new_stat = ioRam[addr - 0xFF00] & 0x07;
            // write bits 6-3
            new_stat |= data & 0x78;
            ioRam[addr - 0xFF00] = new_stat;
            break;
        }
        case INTERRUPT_REG_IF:
            m_input_handler.set_irq(data);
            ioRam[addr - 0xFF00] = data;
            break;
        case 0xFFFF:
            IE = data;
            break;
        // explicitly define writable addresses
        case INPUT_JOYP:
        case SERIAL_DATA:
        case PPU_REG_LCDC:
        case PPU_REG_SCY:
        case PPU_REG_SCX:
        case PPU_REG_LYC:
        case PPU_REG_DMA:
        case PPU_REG_BGP:
        case PPU_REG_OBP0:
        case PPU_REG_OBP1:
            ioRam[addr - 0xFF00] = data;
            break;
        default:
            break;
    }
}
