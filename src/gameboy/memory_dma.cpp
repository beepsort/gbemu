#include "gameboy/memory.h"
#include "gameboy/memory_dma.h"
#include "gameboy/memory_io.h"

void GAMEBOY::DmaController::tick()
{
    if (step==0)
    {
        uint8_t dma_addr = memory.read(IOHandler::PPU_REG_DMA, MemoryAccessSource::DMA);
        if (dma_addr != 0)
        {
            m_dma_addr = dma_addr;
            memory.lock(AddressDispatcher::LOCKABLE::ALL_DMA);
            step++;
        }
    }
    else
    {
        if (step > 0xA0)
        {
            step = 0;
            m_dma_addr = 0;
            memory.unlock(AddressDispatcher::LOCKABLE::ALL_DMA);
            memory.write(IOHandler::PPU_REG_DMA, 0, MemoryAccessSource::DMA);
            return;
        }
        uint16_t src_addr = ((uint16_t)m_dma_addr << 8) + step - 1;
        uint16_t dest_addr = 0xFE00 + step - 1;
        uint8_t data = memory.read(src_addr, MemoryAccessSource::DMA);
        memory.write(dest_addr, data, MemoryAccessSource::DMA);
        step++;
    }
}
