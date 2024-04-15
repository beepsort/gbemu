#ifndef __CPU_INSTRUCTION_CONTROL_H__
#define __CPU_INSTRUCTION_CONTROL_H__

#include <stdint.h>

#include "gameboy/cpu_instruction.h"
#include "gameboy/cpu_registers.h"
#include "gameboy/memory.h"

namespace GAMEBOY
{
    class HALT: public CpuInstruction
    {
    /**
     * @brief CPU suspends until an interrupt occurs
     * Used to reduce power consumption when the CPU is not needed
     * Depending on the current state of the IME register will behave differently
     * IME SET:
     *  CPU enters low-power mode until AFTER an interrupt requires processing
     *  After which the CPU resumes, handles the interrupt and then resumes execution after HALT
     * IME DISABLED:
     *  Behaviour diverges  depending on whether an interrupt is pending (IE & IF != 0)
     *  - No interrupt pending: CPU halts until pending interrupt, resumes without handling
     *  - Interrupt pending: CPU immediately continues execution, but takes an extra cycle
     **/
    private:
        CpuRegisters& registers;
        AddressDispatcher& memory;
        uint8_t step = 0;
    public:
        HALT(CpuRegisters& registers, AddressDispatcher& memory)
        : registers(registers), memory(memory) {}
        InstructionResult tick();
    };

    class STOP: public CpuInstruction
    {
    /**
     * @brief CPU (and display) suspends until button pressed
     **/
    private:
        CpuRegisters& registers;
        AddressDispatcher& memory;
        uint8_t step = 0;
    public:
        STOP(CpuRegisters& registers, AddressDispatcher& memory)
        : registers(registers), memory(memory) {}
        InstructionResult tick();
    };

    class JP_HL: public CpuInstruction
    {
    private:
        uint16_t* pc;
        uint16_t* src;
    public:
        JP_HL(uint16_t* pc, uint16_t* src)
        : pc(pc), src(src) {}
        InstructionResult tick();
    };

    bool cond_TRUE(CpuRegisters& registers);
    bool cond_NZ(CpuRegisters& registers);
    bool cond_NC(CpuRegisters& registers);
    bool cond_Z(CpuRegisters& registers);
    bool cond_C(CpuRegisters& registers);

    class JP_NN: public CpuInstruction
    {
    private:
        bool (*const condition)(CpuRegisters&);
        uint16_t jump_addr = 0;
        uint8_t step = 0;
        CpuRegisters& registers;
        AddressDispatcher& memory;
    public:
        JP_NN(CpuRegisters& registers, AddressDispatcher& memory, bool (*const condition)(CpuRegisters&))
        : condition(condition), registers(registers), memory(memory) {}
        InstructionResult tick();
    };

    class JR_N: public CpuInstruction
    {
    private:
        bool (*const condition)(CpuRegisters&);
        int16_t jump_offset = 0;
        uint8_t step = 0;
        CpuRegisters& registers;
        AddressDispatcher& memory;
    public:
        JR_N(CpuRegisters& registers, AddressDispatcher& memory, bool (*const condition)(CpuRegisters&))
        : condition(condition), registers(registers), memory(memory) {}
        InstructionResult tick();
    };

    class SCF: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
    public:
        SCF(CpuRegisters& registers)
        : registers(registers) {}
        InstructionResult tick();
    };

    class CCF: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
    public:
        CCF(CpuRegisters& registers)
        : registers(registers) {}
        InstructionResult tick();
    };

    class DI: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
    public:
        DI(CpuRegisters& registers)
        : registers(registers) {}
        InstructionResult tick();
    };

    class EI: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
    public:
        EI(CpuRegisters& registers)
        : registers(registers) {}
        InstructionResult tick();
    };

    class RET: public CpuInstruction
    {
    private:
        uint16_t jump_addr = 0;
        uint8_t step = 0;
        CpuRegisters& registers;
        AddressDispatcher& memory;
    public:
        RET(CpuRegisters& registers, AddressDispatcher& memory)
        : registers(registers), memory(memory) {}
        InstructionResult tick();
    };

    class RET_CC: public CpuInstruction
    {
    private:
        bool (*const condition)(CpuRegisters&);
        uint16_t jump_addr = 0;
        uint8_t step = 0;
        CpuRegisters& registers;
        AddressDispatcher& memory;
    public:
        RET_CC(CpuRegisters& registers, AddressDispatcher& memory, bool (*const condition)(CpuRegisters&))
        : condition(condition), registers(registers), memory(memory) {}
        InstructionResult tick();
    };

    class RETI: public CpuInstruction
    {
    private:
        uint16_t jump_addr = 0;
        uint8_t step = 0;
        CpuRegisters& registers;
        AddressDispatcher& memory;
    public:
        RETI(CpuRegisters& registers, AddressDispatcher& memory)
        : registers(registers), memory(memory) {}
        InstructionResult tick();
    };

    class CALL_NN: public CpuInstruction
    {
    private:
        bool (*const condition)(CpuRegisters&);
        uint16_t jump_addr = 0;
        uint8_t step = 0;
        CpuRegisters& registers;
        AddressDispatcher& memory;
    public:
        CALL_NN(CpuRegisters& registers, AddressDispatcher& memory, bool (*const condition)(CpuRegisters&))
        : condition(condition), registers(registers), memory(memory) {}
        InstructionResult tick();
    };

    class RST: public CpuInstruction
    {
    private:
        uint8_t jump_lsb;
        uint8_t step = 0;
        CpuRegisters& registers;
        AddressDispatcher& memory;
    public:
        RST(CpuRegisters& registers, AddressDispatcher& memory, uint8_t jump_lsb)
        : jump_lsb(jump_lsb), registers(registers), memory(memory) {}
        InstructionResult tick();
    };
};

#endif
