#ifndef __CPU_INSTRUCTION_H__
#define __CPU_INSTRUCTION_H__

namespace CPU
{
    class CpuInstruction
    {
    public:
        // progress instruction by 1 M-cycle
        // return boolean true when instruction is complete
        virtual bool tick() = 0;
    };
};

#endif