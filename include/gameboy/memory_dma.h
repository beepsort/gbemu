#ifndef __MEMORY_DMA_H__
#define __MEMORY_DMA_H__

#include "gameboy/memory.h"

namespace GAMEBOY
{
    class DmaController
    {
    private:
        AddressDispatcher& memory;
        uint8_t step = 0;
        uint8_t m_dma_addr = 0;
    public:
        DmaController(AddressDispatcher& memory)
        : memory(memory) {}
        void tick();
    };
};

#endif
