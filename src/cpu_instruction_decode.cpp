#include "cpu_instruction_alu.h"
#include "cpu_instruction_control.h"
#include "cpu_instruction_decode.h"
#include "cpu_instruction_load.h"
#include "cpu_instruction_misc.h"

/*
 * For a full overview of the Sharp SM83 CPU opcodes
 * https://meganesulli.com/generate-gb-opcodes/
 */
CPU::CpuInstruction* CPU::decode_opcode(uint8_t opcode, CPU::CpuRegisters& registers, MEMORY::AddressDispatcher& memory)
{
    switch (opcode)
    {
        case 0x00:
            return new NOP();
        case 0x01:
            return new LD_rr_nn(registers.BC, registers.PC, memory);
        case 0x02:
            return new LD_absrr_r(registers.BC, registers.A, memory);
        case 0x03:
            return new INC_rr(registers.BC, registers);
        case 0x04:
            return new INC_r(registers.B, registers);
        case 0x05:
            return new DEC_r(registers.B, registers);
        case 0x06:
            return new LD_r_n(registers.B, registers.PC, memory);
        case 0x08:
            return new LD_absnn_rr(registers.PC, registers.SP, memory);
        case 0x09:
            return new ADD_HL_rr(registers.BC, registers);
        case 0x0A:
            return new LD_r_absrr(registers.A, registers.BC, memory);
        case 0x0B:
            return new DEC_rr(registers.BC, registers);
        case 0x0C:
            return new INC_r(registers.C, registers);
        case 0x0D:
            return new DEC_r(registers.C, registers);
        case 0x0E:
            return new LD_r_n(registers.C, registers.PC, memory);
        case 0x11:
            return new LD_rr_nn(registers.DE, registers.PC, memory);
        case 0x12:
            return new LD_absrr_r(registers.DE, registers.A, memory);
        case 0x13:
            return new INC_rr(registers.DE, registers);
        case 0x14:
            return new INC_r(registers.D, registers);
        case 0x15:
            return new DEC_r(registers.D, registers);
        case 0x16:
            return new LD_r_n(registers.D, registers.PC, memory);
        case 0x18:
            return new JR_N(registers, memory, CPU::cond_TRUE);
        case 0x19:
            return new ADD_HL_rr(registers.DE, registers);
        case 0x1A:
            return new LD_r_absrr(registers.A, registers.DE, memory);
        case 0x1B:
            return new DEC_rr(registers.DE, registers);
        case 0x1C:
            return new INC_r(registers.E, registers);
        case 0x1D:
            return new DEC_r(registers.E, registers);
        case 0x1E:
            return new LD_r_n(registers.E, registers.PC, memory);
        case 0x20:
            return new JR_N(registers, memory, CPU::cond_NZ);
        case 0x21:
            return new LD_rr_nn(registers.HL, registers.PC, memory);
        case 0x22:
            return new LD_absrr_r(registers.HL, registers.A, memory, AddressMutOperation::INC);
        case 0x23:
            return new INC_rr(registers.HL, registers);
        case 0x24:
            return new INC_r(registers.H, registers);
        case 0x25:
            return new DEC_r(registers.H, registers);
        case 0x26:
            return new LD_r_n(registers.H, registers.PC, memory);
        case 0x27:
            return new DAA(registers);
        case 0x28:
            return new JR_N(registers, memory, CPU::cond_Z);
        case 0x29:
            return new ADD_HL_rr(registers.HL, registers);
        case 0x2A:
            return new LD_r_absrr(registers.A, registers.HL, memory, AddressMutOperation::INC);
        case 0x2B:
            return new DEC_rr(registers.HL, registers);
        case 0x2C:
            return new INC_r(registers.L, registers);
        case 0x2D:
            return new DEC_r(registers.L, registers);
        case 0x2E:
            return new LD_r_n(registers.L, registers.PC, memory);
        case 0x2F:
            return new CPL(registers);
        case 0x30:
            return new JR_N(registers, memory, CPU::cond_NC);
        case 0x31:
            return new LD_rr_nn(registers.SP, registers.PC, memory);
        case 0x32:
            return new LD_absrr_r(registers.HL, registers.A, memory, AddressMutOperation::DEC);
        case 0x33:
            return new INC_rr(registers.SP, registers);
        case 0x34:
            return new INC_absrr(registers.HL, registers, memory);
        case 0x35:
            return new DEC_absrr(registers.HL, registers, memory);
        case 0x36:
            return new LD_absrr_n(registers.HL, registers.PC, memory);
        case 0x38:
            return new JR_N(registers, memory, CPU::cond_C);
        case 0x39:
            return new ADD_HL_rr(registers.SP, registers);
        case 0x3A:
            return new LD_r_absrr(registers.A, registers.HL, memory, AddressMutOperation::DEC);
        case 0x3B:
            return new DEC_rr(registers.SP, registers);
        case 0x3C:
            return new INC_r(registers.A, registers);
        case 0x3D:
            return new DEC_r(registers.A, registers);
        case 0x3E:
            return new LD_r_n(registers.A, registers.PC, memory);
        case 0x40:
            return new LD_r_r(registers.B, registers.B);
        case 0x41:
            return new LD_r_r(registers.B, registers.C);
        case 0x42:
            return new LD_r_r(registers.B, registers.D);
        case 0x43:
            return new LD_r_r(registers.B, registers.E);
        case 0x44:
            return new LD_r_r(registers.B, registers.H);
        case 0x45:
            return new LD_r_r(registers.B, registers.L);
        case 0x46:
            return new LD_r_absrr(registers.B, registers.HL, memory);
        case 0x47:
            return new LD_r_r(registers.B, registers.A);
        case 0x48:
            return new LD_r_r(registers.C, registers.B);
        case 0x49:
            return new LD_r_r(registers.C, registers.C);
        case 0x4A:
            return new LD_r_r(registers.C, registers.D);
        case 0x4B:
            return new LD_r_r(registers.C, registers.E);
        case 0x4C:
            return new LD_r_r(registers.C, registers.H);
        case 0x4D:
            return new LD_r_r(registers.C, registers.L);
        case 0x4E:
            return new LD_r_absrr(registers.C, registers.HL, memory);
        case 0x4F:
            return new LD_r_r(registers.C, registers.A);
        case 0x50:
            return new LD_r_r(registers.D, registers.B);
        case 0x51:
            return new LD_r_r(registers.D, registers.C);
        case 0x52:
            return new LD_r_r(registers.D, registers.D);
        case 0x53:
            return new LD_r_r(registers.D, registers.E);
        case 0x54:
            return new LD_r_r(registers.D, registers.H);
        case 0x55:
            return new LD_r_r(registers.D, registers.L);
        case 0x56:
            return new LD_r_absrr(registers.D, registers.HL, memory);
        case 0x57:
            return new LD_r_r(registers.D, registers.A);
        case 0x58:
            return new LD_r_r(registers.E, registers.B);
        case 0x59:
            return new LD_r_r(registers.E, registers.C);
        case 0x5A:
            return new LD_r_r(registers.E, registers.D);
        case 0x5B:
            return new LD_r_r(registers.E, registers.E);
        case 0x5C:
            return new LD_r_r(registers.E, registers.H);
        case 0x5D:
            return new LD_r_r(registers.E, registers.L);
        case 0x5E:
            return new LD_r_absrr(registers.E, registers.HL, memory);
        case 0x5F:
            return new LD_r_r(registers.E, registers.A);
        case 0x60:
            return new LD_r_r(registers.H, registers.B);
        case 0x61:
            return new LD_r_r(registers.H, registers.C);
        case 0x62:
            return new LD_r_r(registers.H, registers.D);
        case 0x63:
            return new LD_r_r(registers.H, registers.E);
        case 0x64:
            return new LD_r_r(registers.H, registers.H);
        case 0x65:
            return new LD_r_r(registers.H, registers.L);
        case 0x66:
            return new LD_r_absrr(registers.H, registers.HL, memory);
        case 0x67:
            return new LD_r_r(registers.H, registers.A);
        case 0x68:
            return new LD_r_r(registers.L, registers.B);
        case 0x69:
            return new LD_r_r(registers.L, registers.C);
        case 0x6A:
            return new LD_r_r(registers.L, registers.D);
        case 0x6B:
            return new LD_r_r(registers.L, registers.E);
        case 0x6C:
            return new LD_r_r(registers.L, registers.H);
        case 0x6D:
            return new LD_r_r(registers.L, registers.L);
        case 0x6E:
            return new LD_r_absrr(registers.L, registers.HL, memory);
        case 0x6F:
            return new LD_r_r(registers.L, registers.A);
        case 0x70:
            return new LD_absrr_r(registers.HL, registers.B, memory);
        case 0x71:
            return new LD_absrr_r(registers.HL, registers.C, memory);
        case 0x72:
            return new LD_absrr_r(registers.HL, registers.D, memory);
        case 0x73:
            return new LD_absrr_r(registers.HL, registers.E, memory);
        case 0x74:
            return new LD_absrr_r(registers.HL, registers.H, memory);
        case 0x75:
            return new LD_absrr_r(registers.HL, registers.L, memory);
        case 0x77:
            return new LD_absrr_r(registers.HL, registers.A, memory);
        case 0x78:
            return new LD_r_r(registers.A, registers.B);
        case 0x79:
            return new LD_r_r(registers.A, registers.C);
        case 0x7A:
            return new LD_r_r(registers.A, registers.D);
        case 0x7B:
            return new LD_r_r(registers.A, registers.E);
        case 0x7C:
            return new LD_r_r(registers.A, registers.H);
        case 0x7D:
            return new LD_r_r(registers.A, registers.L);
        case 0x7E:
            return new LD_r_absrr(registers.A, registers.HL, memory);
        case 0x7F:
            return new LD_r_r(registers.A, registers.A);
        case 0x80:
            return new ADD_r_r(registers.A, registers.B, registers);
        case 0x81:
            return new ADD_r_r(registers.A, registers.C, registers);
        case 0x82:
            return new ADD_r_r(registers.A, registers.D, registers);
        case 0x83:
            return new ADD_r_r(registers.A, registers.E, registers);
        case 0x84:
            return new ADD_r_r(registers.A, registers.H, registers);
        case 0x85:
            return new ADD_r_r(registers.A, registers.L, registers);
        case 0x86:
            return new ADD_r_absrr(registers.A, registers.HL, registers, memory);
        case 0x87:
            return new ADD_r_r(registers.A, registers.A, registers);
        case 0x88:
            return new ADD_r_r(registers.A, registers.B, registers, true);
        case 0x89:
            return new ADD_r_r(registers.A, registers.C, registers, true);
        case 0x8A:
            return new ADD_r_r(registers.A, registers.D, registers, true);
        case 0x8B:
            return new ADD_r_r(registers.A, registers.E, registers, true);
        case 0x8C:
            return new ADD_r_r(registers.A, registers.H, registers, true);
        case 0x8D:
            return new ADD_r_r(registers.A, registers.L, registers, true);
        case 0x8E:
            return new ADD_r_absrr(registers.A, registers.HL, registers, memory, true);
        case 0x8F:
            return new ADD_r_r(registers.A, registers.A, registers, true);
        case 0x90:
            return new SUB_r_r(registers.A, registers.B, registers);
        case 0x91:
            return new SUB_r_r(registers.A, registers.C, registers);
        case 0x92:
            return new SUB_r_r(registers.A, registers.D, registers);
        case 0x93:
            return new SUB_r_r(registers.A, registers.E, registers);
        case 0x94:
            return new SUB_r_r(registers.A, registers.H, registers);
        case 0x95:
            return new SUB_r_r(registers.A, registers.L, registers);
        case 0x96:
            return new SUB_r_absrr(registers.A, registers.HL, registers, memory);
        case 0x97:
            return new SUB_r_r(registers.A, registers.A, registers);
        case 0x98:
            return new SUB_r_r(registers.A, registers.B, registers, true);
        case 0x99:
            return new SUB_r_r(registers.A, registers.C, registers, true);
        case 0x9A:
            return new SUB_r_r(registers.A, registers.D, registers, true);
        case 0x9B:
            return new SUB_r_r(registers.A, registers.E, registers, true);
        case 0x9C:
            return new SUB_r_r(registers.A, registers.H, registers, true);
        case 0x9D:
            return new SUB_r_r(registers.A, registers.L, registers, true);
        case 0x9E:
            return new SUB_r_absrr(registers.A, registers.HL, registers, memory, true);
        case 0x9F:
            return new SUB_r_r(registers.A, registers.A, registers, true);
        case 0xA0:
            return new AND_r_r(registers.A, registers.B, registers);
        case 0xA1:
            return new AND_r_r(registers.A, registers.C, registers);
        case 0xA2:
            return new AND_r_r(registers.A, registers.D, registers);
        case 0xA3:
            return new AND_r_r(registers.A, registers.E, registers);
        case 0xA4:
            return new AND_r_r(registers.A, registers.H, registers);
        case 0xA5:
            return new AND_r_r(registers.A, registers.L, registers);
        case 0xA6:
            return new AND_r_absrr(registers.A, registers.HL, registers, memory);
        case 0xA7:
            return new AND_r_r(registers.A, registers.A, registers);
        case 0xA8:
            return new XOR_r_r(registers.A, registers.B, registers);
        case 0xA9:
            return new XOR_r_r(registers.A, registers.C, registers);
        case 0xAA:
            return new XOR_r_r(registers.A, registers.D, registers);
        case 0xAB:
            return new XOR_r_r(registers.A, registers.E, registers);
        case 0xAC:
            return new XOR_r_r(registers.A, registers.H, registers);
        case 0xAD:
            return new XOR_r_r(registers.A, registers.L, registers);
        case 0xAE:
            return new XOR_r_absrr(registers.A, registers.HL, registers, memory);
        case 0xAF:
            return new XOR_r_r(registers.A, registers.A, registers);
        case 0xB0:
            return new OR_r_r(registers.A, registers.B, registers);
        case 0xB1:
            return new OR_r_r(registers.A, registers.C, registers);
        case 0xB2:
            return new OR_r_r(registers.A, registers.D, registers);
        case 0xB3:
            return new OR_r_r(registers.A, registers.E, registers);
        case 0xB4:
            return new OR_r_r(registers.A, registers.H, registers);
        case 0xB5:
            return new OR_r_r(registers.A, registers.L, registers);
        case 0xB6:
            return new OR_r_absrr(registers.A, registers.HL, registers, memory);
        case 0xB7:
            return new OR_r_r(registers.A, registers.A, registers);
        case 0xB8:
            return new CP_r_r(registers.A, registers.B, registers);
        case 0xB9:
            return new CP_r_r(registers.A, registers.C, registers);
        case 0xBA:
            return new CP_r_r(registers.A, registers.D, registers);
        case 0xBB:
            return new CP_r_r(registers.A, registers.E, registers);
        case 0xBC:
            return new CP_r_r(registers.A, registers.H, registers);
        case 0xBD:
            return new CP_r_r(registers.A, registers.L, registers);
        case 0xBE:
            return new CP_r_absrr(registers.A, registers.HL, registers, memory);
        case 0xBF:
            return new CP_r_r(registers.A, registers.A, registers);
        case 0xC1:
            return new POP_rr(registers.BC, registers.SP, memory);
        case 0xC2:
            return new JP_NN(registers, memory, &cond_NZ);
        case 0xC3:
            return new JP_NN(registers, memory, &cond_TRUE);
        case 0xC5:
            return new PUSH_rr(registers.SP, registers.BC, memory);
        case 0xC6:
            return new ADD_r_n(registers.A, registers.PC, registers, memory);
        case 0xCA:
            return new JP_NN(registers, memory, &cond_Z);
        case 0xCE:
            return new ADD_r_n(registers.A, registers.PC, registers, memory, true);
        case 0xD1:
            return new POP_rr(registers.DE, registers.SP, memory);
        case 0xD2:
            return new JP_NN(registers, memory, &cond_NC);
        case 0xD5:
            return new PUSH_rr(registers.SP, registers.DE, memory);
        case 0xD6:
            return new SUB_r_n(registers.A, registers.PC, registers, memory);
        case 0xDA:
            return new JP_NN(registers, memory, &cond_C);
        case 0xDE:
            return new SUB_r_n(registers.A, registers.PC, registers, memory, true);
        case 0xE0:
            return new LD_reln_r(registers.PC, registers.A, memory);
        case 0xE1:
            return new POP_rr(registers.HL, registers.SP, memory);
        case 0xE2:
            return new LD_relr_r(registers.C, registers.A, memory);
        case 0xE5:
            return new PUSH_rr(registers.SP, registers.HL, memory);
        case 0xE6:
            return new AND_r_n(registers.A, registers.PC, registers, memory);
        case 0xE9:
            return new JP_HL(registers.PC, registers.HL);
        case 0xEA:
            return new LD_absnn_r(registers.PC, registers.A, memory);
        case 0xEE:
            return new XOR_r_n(registers.A, registers.PC, registers, memory);
        case 0xF0:
            return new LD_r_reln(registers.A, registers.PC, memory);
        case 0xF1:
            return new POP_rr(registers.AF, registers.SP, memory);
        case 0xF2:
            return new LD_r_relr(registers.A, registers.C, memory);
        case 0xF5:
            return new PUSH_rr(registers.SP, registers.AF, memory);
        case 0xF6:
            return new OR_r_n(registers.A, registers.PC, registers, memory);
        case 0xF9:
            return new LD_rr_rr(registers.SP, registers.HL);
        case 0xFA:
            return new LD_r_absnn(registers.A, registers.PC, memory);
        case 0xFE:
            return new CP_r_n(registers.A, registers.PC, registers, memory);
        default:
            return new NOP();
    }
}