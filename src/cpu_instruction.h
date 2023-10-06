#ifndef __CPU_INSTRUCTION_H__
#define __CPU_INSTRUCTION_H__

namespace CPU
{
    enum class InstructionResult
    {
        RUNNING,
        FINISHED,
        HALT,
        STOP
    };

    class CpuInstruction
    {
    public:
        // progress instruction by 1 M-cycle
        // return boolean true when instruction is complete
        virtual InstructionResult tick() = 0;
    };
};

#endif
