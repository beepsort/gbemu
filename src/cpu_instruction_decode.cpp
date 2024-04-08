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
            return new NOP(registers);
        case 0x01:
            return new LD_rr_nn(registers, registers.BC, memory);
        case 0x02:
            return new LD_absrr_r(registers, registers.BC, registers.A, memory);
        case 0x03:
            return new INC_rr(registers.BC, registers);
        case 0x04:
            return new INC_r(registers.B, registers);
        case 0x05:
            return new DEC_r(registers.B, registers);
        case 0x06:
            return new LD_r_n(registers, registers.B, memory);
        case 0x07:
            return new RLA_r(registers, false);
        case 0x08:
            return new LD_absnn_rr(registers, registers.SP, memory);
        case 0x09:
            return new ADD_HL_rr(registers.BC, registers);
        case 0x0A:
            return new LD_r_absrr(registers, registers.A, registers.BC, memory);
        case 0x0B:
            return new DEC_rr(registers.BC, registers);
        case 0x0C:
            return new INC_r(registers.C, registers);
        case 0x0D:
            return new DEC_r(registers.C, registers);
        case 0x0E:
            return new LD_r_n(registers, registers.C, memory);
        case 0x0F:
            return new RRA_r(registers, false);
        case 0x10:
            return new STOP(registers);
        case 0x11:
            return new LD_rr_nn(registers, registers.DE, memory);
        case 0x12:
            return new LD_absrr_r(registers, registers.DE, registers.A, memory);
        case 0x13:
            return new INC_rr(registers.DE, registers);
        case 0x14:
            return new INC_r(registers.D, registers);
        case 0x15:
            return new DEC_r(registers.D, registers);
        case 0x16:
            return new LD_r_n(registers, registers.D, memory);
        case 0x17:
            return new RLA_r(registers, true);
        case 0x18:
            return new JR_N(registers, memory, CPU::cond_TRUE);
        case 0x19:
            return new ADD_HL_rr(registers.DE, registers);
        case 0x1A:
            return new LD_r_absrr(registers, registers.A, registers.DE, memory);
        case 0x1B:
            return new DEC_rr(registers.DE, registers);
        case 0x1C:
            return new INC_r(registers.E, registers);
        case 0x1D:
            return new DEC_r(registers.E, registers);
        case 0x1E:
            return new LD_r_n(registers, registers.E, memory);
        case 0x1F:
            return new RRA_r(registers, true);
        case 0x20:
            return new JR_N(registers, memory, CPU::cond_NZ);
        case 0x21:
            return new LD_rr_nn(registers, registers.HL, memory);
        case 0x22:
            return new LD_absrr_r(registers, registers.HL, registers.A, memory, AddressMutOperation::INC);
        case 0x23:
            return new INC_rr(registers.HL, registers);
        case 0x24:
            return new INC_r(registers.H, registers);
        case 0x25:
            return new DEC_r(registers.H, registers);
        case 0x26:
            return new LD_r_n(registers, registers.H, memory);
        case 0x27:
            return new DAA(registers);
        case 0x28:
            return new JR_N(registers, memory, CPU::cond_Z);
        case 0x29:
            return new ADD_HL_rr(registers.HL, registers);
        case 0x2A:
            return new LD_r_absrr(registers, registers.A, registers.HL, memory, AddressMutOperation::INC);
        case 0x2B:
            return new DEC_rr(registers.HL, registers);
        case 0x2C:
            return new INC_r(registers.L, registers);
        case 0x2D:
            return new DEC_r(registers.L, registers);
        case 0x2E:
            return new LD_r_n(registers, registers.L, memory);
        case 0x2F:
            return new CPL(registers);
        case 0x30:
            return new JR_N(registers, memory, CPU::cond_NC);
        case 0x31:
            return new LD_rr_nn(registers, registers.SP, memory);
        case 0x32:
            return new LD_absrr_r(registers, registers.HL, registers.A, memory, AddressMutOperation::DEC);
        case 0x33:
            return new INC_rr(registers.SP, registers);
        case 0x34:
            return new INC_absrr(registers.HL, registers, memory);
        case 0x35:
            return new DEC_absrr(registers.HL, registers, memory);
        case 0x36:
            return new LD_absrr_n(registers, registers.HL, memory);
        case 0x37:
            return new SCF(registers);
        case 0x38:
            return new JR_N(registers, memory, CPU::cond_C);
        case 0x39:
            return new ADD_HL_rr(registers.SP, registers);
        case 0x3A:
            return new LD_r_absrr(registers, registers.A, registers.HL, memory, AddressMutOperation::DEC);
        case 0x3B:
            return new DEC_rr(registers.SP, registers);
        case 0x3C:
            return new INC_r(registers.A, registers);
        case 0x3D:
            return new DEC_r(registers.A, registers);
        case 0x3E:
            return new LD_r_n(registers, registers.A, memory);
        case 0x3F:
            return new CCF(registers);
        case 0x40:
            return new LD_r_r(registers, registers.B, registers.B);
        case 0x41:
            return new LD_r_r(registers, registers.B, registers.C);
        case 0x42:
            return new LD_r_r(registers, registers.B, registers.D);
        case 0x43:
            return new LD_r_r(registers, registers.B, registers.E);
        case 0x44:
            return new LD_r_r(registers, registers.B, registers.H);
        case 0x45:
            return new LD_r_r(registers, registers.B, registers.L);
        case 0x46:
            return new LD_r_absrr(registers, registers.B, registers.HL, memory);
        case 0x47:
            return new LD_r_r(registers, registers.B, registers.A);
        case 0x48:
            return new LD_r_r(registers, registers.C, registers.B);
        case 0x49:
            return new LD_r_r(registers, registers.C, registers.C);
        case 0x4A:
            return new LD_r_r(registers, registers.C, registers.D);
        case 0x4B:
            return new LD_r_r(registers, registers.C, registers.E);
        case 0x4C:
            return new LD_r_r(registers, registers.C, registers.H);
        case 0x4D:
            return new LD_r_r(registers, registers.C, registers.L);
        case 0x4E:
            return new LD_r_absrr(registers, registers.C, registers.HL, memory);
        case 0x4F:
            return new LD_r_r(registers, registers.C, registers.A);
        case 0x50:
            return new LD_r_r(registers, registers.D, registers.B);
        case 0x51:
            return new LD_r_r(registers, registers.D, registers.C);
        case 0x52:
            return new LD_r_r(registers, registers.D, registers.D);
        case 0x53:
            return new LD_r_r(registers, registers.D, registers.E);
        case 0x54:
            return new LD_r_r(registers, registers.D, registers.H);
        case 0x55:
            return new LD_r_r(registers, registers.D, registers.L);
        case 0x56:
            return new LD_r_absrr(registers, registers.D, registers.HL, memory);
        case 0x57:
            return new LD_r_r(registers, registers.D, registers.A);
        case 0x58:
            return new LD_r_r(registers, registers.E, registers.B);
        case 0x59:
            return new LD_r_r(registers, registers.E, registers.C);
        case 0x5A:
            return new LD_r_r(registers, registers.E, registers.D);
        case 0x5B:
            return new LD_r_r(registers, registers.E, registers.E);
        case 0x5C:
            return new LD_r_r(registers, registers.E, registers.H);
        case 0x5D:
            return new LD_r_r(registers, registers.E, registers.L);
        case 0x5E:
            return new LD_r_absrr(registers, registers.E, registers.HL, memory);
        case 0x5F:
            return new LD_r_r(registers, registers.E, registers.A);
        case 0x60:
            return new LD_r_r(registers, registers.H, registers.B);
        case 0x61:
            return new LD_r_r(registers, registers.H, registers.C);
        case 0x62:
            return new LD_r_r(registers, registers.H, registers.D);
        case 0x63:
            return new LD_r_r(registers, registers.H, registers.E);
        case 0x64:
            return new LD_r_r(registers, registers.H, registers.H);
        case 0x65:
            return new LD_r_r(registers, registers.H, registers.L);
        case 0x66:
            return new LD_r_absrr(registers, registers.H, registers.HL, memory);
        case 0x67:
            return new LD_r_r(registers, registers.H, registers.A);
        case 0x68:
            return new LD_r_r(registers, registers.L, registers.B);
        case 0x69:
            return new LD_r_r(registers, registers.L, registers.C);
        case 0x6A:
            return new LD_r_r(registers, registers.L, registers.D);
        case 0x6B:
            return new LD_r_r(registers, registers.L, registers.E);
        case 0x6C:
            return new LD_r_r(registers, registers.L, registers.H);
        case 0x6D:
            return new LD_r_r(registers, registers.L, registers.L);
        case 0x6E:
            return new LD_r_absrr(registers, registers.L, registers.HL, memory);
        case 0x6F:
            return new LD_r_r(registers, registers.L, registers.A);
        case 0x70:
            return new LD_absrr_r(registers, registers.HL, registers.B, memory);
        case 0x71:
            return new LD_absrr_r(registers, registers.HL, registers.C, memory);
        case 0x72:
            return new LD_absrr_r(registers, registers.HL, registers.D, memory);
        case 0x73:
            return new LD_absrr_r(registers, registers.HL, registers.E, memory);
        case 0x74:
            return new LD_absrr_r(registers, registers.HL, registers.H, memory);
        case 0x75:
            return new LD_absrr_r(registers, registers.HL, registers.L, memory);
        case 0x76:
            return new HALT(registers, memory);
        case 0x77:
            return new LD_absrr_r(registers, registers.HL, registers.A, memory);
        case 0x78:
            return new LD_r_r(registers, registers.A, registers.B);
        case 0x79:
            return new LD_r_r(registers, registers.A, registers.C);
        case 0x7A:
            return new LD_r_r(registers, registers.A, registers.D);
        case 0x7B:
            return new LD_r_r(registers, registers.A, registers.E);
        case 0x7C:
            return new LD_r_r(registers, registers.A, registers.H);
        case 0x7D:
            return new LD_r_r(registers, registers.A, registers.L);
        case 0x7E:
            return new LD_r_absrr(registers, registers.A, registers.HL, memory);
        case 0x7F:
            return new LD_r_r(registers, registers.A, registers.A);
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
        case 0xC0:
            return new RET_CC(registers, memory, &cond_NZ);
        case 0xC1:
            return new POP_rr(registers, registers.BC, memory);
        case 0xC2:
            return new JP_NN(registers, memory, &cond_NZ);
        case 0xC3:
            return new JP_NN(registers, memory, &cond_TRUE);
        case 0xC4:
            return new CALL_NN(registers, memory, &cond_NZ);
        case 0xC5:
            return new PUSH_rr(registers, registers.BC, memory);
        case 0xC6:
            return new ADD_r_n(registers.A, registers, memory);
        case 0xC7:
            return new RST(registers, memory, 0x00);
        case 0xC8:
            return new RET_CC(registers, memory, &cond_Z);
        case 0xC9:
            return new RET(registers, memory);
        case 0xCA:
            return new JP_NN(registers, memory, &cond_Z);
        case 0xCB:
            return new CB_PREFIX(registers, memory);
        case 0xCC:
            return new CALL_NN(registers, memory, &cond_Z);
        case 0xCD:
            return new CALL_NN(registers, memory, &cond_TRUE);
        case 0xCE:
            return new ADD_r_n(registers.A, registers, memory, true);
        case 0xCF:
            return new RST(registers, memory, 0x08);
        case 0xD0:
            return new RET_CC(registers, memory, &cond_NC);
        case 0xD1:
            return new POP_rr(registers, registers.DE, memory);
        case 0xD2:
            return new JP_NN(registers, memory, &cond_NC);
        case 0xD3:
            return new NOP(registers);
        case 0xD4:
            return new CALL_NN(registers, memory, &cond_NC);
        case 0xD5:
            return new PUSH_rr(registers, registers.DE, memory);
        case 0xD6:
            return new SUB_r_n(registers.A, registers, memory);
        case 0xD7:
            return new RST(registers, memory, 0x10);
        case 0xD8:
            return new RET_CC(registers, memory, &cond_C);
        case 0xD9:
            return new RETI(registers, memory);
        case 0xDA:
            return new JP_NN(registers, memory, &cond_C);
        case 0xDB:
            return new NOP(registers);
        case 0xDC:
            return new CALL_NN(registers, memory, &cond_C);
        case 0xDD:
            return new NOP(registers);
        case 0xDE:
            return new SUB_r_n(registers.A, registers, memory, true);
        case 0xDF:
            return new RST(registers, memory, 0x18);
        case 0xE0:
            return new LD_reln_r(registers, registers.A, memory);
        case 0xE1:
            return new POP_rr(registers, registers.HL, memory);
        case 0xE2:
            return new LD_relr_r(registers, registers.C, registers.A, memory);
        case 0xE3:
            return new NOP(registers);
        case 0xE4:
            return new NOP(registers);
        case 0xE5:
            return new PUSH_rr(registers, registers.HL, memory);
        case 0xE6:
            return new AND_r_n(registers.A, registers, memory);
        case 0xE7:
            return new RST(registers, memory, 0x20);
        case 0xE8:
            return new ADD_SP_n(registers, memory);
        case 0xE9:
            return new JP_HL(registers.PC, registers.HL);
        case 0xEA:
            return new LD_absnn_r(registers, registers.A, memory);
        case 0xEB:
            return new NOP(registers);
        case 0xEC:
            return new NOP(registers);
        case 0xED:
            return new NOP(registers);
        case 0xEE:
            return new XOR_r_n(registers.A, registers, memory);
        case 0xEF:
            return new RST(registers, memory, 0x28);
        case 0xF0:
            return new LD_r_reln(registers, registers.A, memory);
        case 0xF1:
            return new POP_rr(registers, registers.AF, memory);
        case 0xF2:
            return new LD_r_relr(registers, registers.A, registers.C, memory);
        case 0xF3:
            return new DI(registers);
        case 0xF4:
            return new NOP(registers);
        case 0xF5:
            return new PUSH_rr(registers, registers.AF, memory);
        case 0xF6:
            return new OR_r_n(registers.A, registers, memory);
        case 0xF7:
            return new RST(registers, memory, 0x30);
        case 0xF8:
            return new LD_HL_SP_n(registers, memory);
        case 0xF9:
            return new LD_rr_rr(registers, registers.SP, registers.HL);
        case 0xFA:
            return new LD_r_absnn(registers, registers.A, memory);
        case 0xFB:
            return new EI(registers);
        case 0xFC:
            return new NOP(registers);
        case 0xFD:
            return new NOP(registers);
        case 0xFE:
            return new CP_r_n(registers.A, registers, memory);
        case 0xFF:
            return new RST(registers, memory, 0x38);
        default:
            return new NOP(registers);
    }
}

CPU::CpuInstruction* CPU::decode_opcode_prefix(uint8_t opcode, CPU::CpuRegisters& registers, MEMORY::AddressDispatcher& memory)
{
    switch (opcode)
    {
        case 0x00:
            // RLC B
        case 0x01:
            // RLC C
        case 0x02:
            // RLC D
        case 0x03:
            // RLC E
        case 0x04:
            // RLC H
        case 0x05:
            // RLC L
        case 0x06:
            // RLC (HL)
        case 0x07:
            // RLC A
        case 0x08:
            // RRC B
        case 0x09:
            // RRC C
        case 0x0A:
            // RRC D
        case 0x0B:
            // RRC E
        case 0x0C:
            // RRC H
        case 0x0D:
            // RRC L
        case 0x0E:
            // RRC (HL)
        case 0x0F:
            // RRC A
        case 0x10:
            // RL B
        case 0x11:
            // RL C
        case 0x12:
            // RL D
        case 0x13:
            // RL E
        case 0x14:
            // RL H
        case 0x15:
            // RL L
        case 0x16:
            // RL (HL)
        case 0x17:
            // RL A
        case 0x18:
            // RR B
        case 0x19:
            // RR C
        case 0x1A:
            // RR D
        case 0x1B:
            // RR E
        case 0x1C:
            // RR H
        case 0x1D:
            // RR L
        case 0x1E:
            // RL (HL)
        case 0x1F:
            // RR A
        case 0x20:
            // SLA B
        case 0x21:
            // SLA C
        case 0x22:
            // SLA D
        case 0x23:
            // SLA E
        case 0x24:
            // SLA H
        case 0x25:
            // SLA L
        case 0x26:
            // SLA (HL)
        case 0x27:
            // SLA A
        case 0x28:
            // SRA B
        case 0x29:
            // SRA C
        case 0x2A:
            // SRA D
        case 0x2B:
            // SRA E
        case 0x2C:
            // SRA H
        case 0x2D:
            // SRA L
        case 0x2E:
            // SRA (HL)
        case 0x2F:
            // SRA A
        case 0x30:
            // SWAP B
        case 0x31:
            // SWAP C
        case 0x32:
            // SWAP D
        case 0x33:
            // SWAP E
        case 0x34:
            // SWAP H
        case 0x35:
            // SWAP L
        case 0x36:
            // SWAP (HL)
        case 0x37:
            // SWAP A
        case 0x38:
            // SRL B
        case 0x39:
            // SRL C
        case 0x3A:
            // SRL D
        case 0x3B:
            // SRL E
        case 0x3C:
            // SRL H
        case 0x3D:
            // SRL L
        case 0x3E:
            // SRL (HL)
        case 0x3F:
            // SRL A
        case 0x40:
            // BIT 0, B
        case 0x41:
            // BIT 0, C
        case 0x42:
            // BIT 0, D
        case 0x43:
            // BIT 0, E
        case 0x44:
            // BIT 0, H
        case 0x45:
            // BIT 0, L
        case 0x46:
            // BIT 0, (HL)
        case 0x47:
            // BIT 0, A
        case 0x48:
            // BIT 1, B
        case 0x49:
            // BIT 1, C
        case 0x4A:
            // BIT 1, D
        case 0x4B:
            // BIT 1, E
        case 0x4C:
            // BIT 1, H
        case 0x4D:
            // BIT 1, L
        case 0x4E:
            // BIT 1, (HL)
        case 0x4F:
            // BIT 1, A
        case 0x50:
            // BIT 2, B
        case 0x51:
            // BIT 2, C
        case 0x52:
            // BIT 2, D
        case 0x53:
            // BIT 2, E
        case 0x54:
            // BIT 2, H
        case 0x55:
            // BIT 2, L
        case 0x56:
            // BIT 2, (HL)
        case 0x57:
            // BIT 2, A
        case 0x58:
            // BIT 3, B
        case 0x59:
            // BIT 3, C
        case 0x5A:
            // BIT 3, D
        case 0x5B:
            // BIT 3, E
        case 0x5C:
            // BIT 3, H
        case 0x5D:
            // BIT 3, L
        case 0x5E:
            // BIT 3, (HL)
        case 0x5F:
            // BIT 3, A
        case 0x60:
            // BIT 4, B
        case 0x61:
            // BIT 4, C
        case 0x62:
            // BIT 4, D
        case 0x63:
            // BIT 4, E
        case 0x64:
            // BIT 4, H
        case 0x65:
            // BIT 4, L
        case 0x66:
            // BIT 4, (HL)
        case 0x67:
            // BIT 4, A
        case 0x68:
            // BIT 5, B
        case 0x69:
            // BIT 5, C
        case 0x6A:
            // BIT 5, D
        case 0x6B:
            // BIT 5, E
        case 0x6C:
            // BIT 5, H
        case 0x6D:
            // BIT 5, L
        case 0x6E:
            // BIT 5, (HL)
        case 0x6F:
            // BIT 5, A
        case 0x70:
            // BIT 6, B
        case 0x71:
            // BIT 6, C
        case 0x72:
            // BIT 6, D
        case 0x73:
            // BIT 6, E
        case 0x74:
            // BIT 6, H
        case 0x75:
            // BIT 6, L
        case 0x76:
            // BIT 6, (HL)
        case 0x77:
            // BIT 6, A
        case 0x78:
            // BIT 7, B
        case 0x79:
            // BIT 7, C
        case 0x7A:
            // BIT 7, D
        case 0x7B:
            // BIT 7, E
        case 0x7C:
            // BIT 7, H
        case 0x7D:
            // BIT 7, L
        case 0x7E:
            // BIT 7, (HL)
        case 0x7F:
            // BIT 7, A
        case 0x80:
            // RES 0, B
        case 0x81:
            // RES 0, C
        case 0x82:
            // RES 0, D
        case 0x83:
            // RES 0, E
        case 0x84:
            // RES 0, H
        case 0x85:
            // RES 0, L
        case 0x86:
            // RES 0, (HL)
        case 0x87:
            // RES 0, A
        case 0x88:
            // RES 1, B
        case 0x89:
            // RES 1, C
        case 0x8A:
            // RES 1, D
        case 0x8B:
            // RES 1, E
        case 0x8C:
            // RES 1, H
        case 0x8D:
            // RES 1, L
        case 0x8E:
            // RES 1, (HL)
        case 0x8F:
            // RES 1, A
        case 0x90:
            // RES 2, B
        case 0x91:
            // RES 2, C
        case 0x92:
            // RES 2, D
        case 0x93:
            // RES 2, E
        case 0x94:
            // RES 2, H
        case 0x95:
            // RES 2, L
        case 0x96:
            // RES 2, (HL)
        case 0x97:
            // RES 2, A
        case 0x98:
            // RES 3, B
        case 0x99:
            // RES 3, C
        case 0x9A:
            // RES 3, D
        case 0x9B:
            // RES 3, E
        case 0x9C:
            // RES 3, H
        case 0x9D:
            // RES 3, L
        case 0x9E:
            // RES 3, (HL)
        case 0x9F:
            // RES 3, A
        case 0xA0:
            // RES 4, B
        case 0xA1:
            // RES 4, C
        case 0xA2:
            // RES 4, D
        case 0xA3:
            // RES 4, E
        case 0xA4:
            // RES 4, H
        case 0xA5:
            // RES 4, L
        case 0xA6:
            // RES 4, (HL)
        case 0xA7:
            // RES 4, A
        case 0xA8:
            // RES 5, B
        case 0xA9:
            // RES 5, C
        case 0xAA:
            // RES 5, D
        case 0xAB:
            // RES 5, E
        case 0xAC:
            // RES 5, H
        case 0xAD:
            // RES 5, L
        case 0xAE:
            // RES 5, (HL)
        case 0xAF:
            // RES 5, A
        case 0xB0:
            // RES 6, B
        case 0xB1:
            // RES 6, C
        case 0xB2:
            // RES 6, D
        case 0xB3:
            // RES 6, E
        case 0xB4:
            // RES 6, H
        case 0xB5:
            // RES 6, L
        case 0xB6:
            // RES 6, (HL)
        case 0xB7:
            // RES 6, A
        case 0xB8:
            // RES 7, B
        case 0xB9:
            // RES 7, C
        case 0xBA:
            // RES 7, D
        case 0xBB:
            // RES 7, E
        case 0xBC:
            // RES 7, H
        case 0xBD:
            // RES 7, L
        case 0xBE:
            // RES 7, (HL)
        case 0xBF:
            // RES 7, A
        case 0xC0:
            // SET 0, B
        case 0xC1:
            // SET 0, C
        case 0xC2:
            // SET 0, D
        case 0xC3:
            // SET 0, E
        case 0xC4:
            // SET 0, H
        case 0xC5:
            // SET 0, L
        case 0xC6:
            // SET 0, (HL)
        case 0xC7:
            // SET 0, A
        case 0xC8:
            // SET 1, B
        case 0xC9:
            // SET 1, C
        case 0xCA:
            // SET 1, D
        case 0xCB:
            // SET 1, E
        case 0xCC:
            // SET 1, H
        case 0xCD:
            // SET 1, L
        case 0xCE:
            // SET 1, (HL)
        case 0xCF:
            // SET 1, A
        case 0xD0:
            // SET 2, B
        case 0xD1:
            // SET 2, C
        case 0xD2:
            // SET 2, D
        case 0xD3:
            // SET 2, E
        case 0xD4:
            // SET 2, H
        case 0xD5:
            // SET 2, L
        case 0xD6:
            // SET 2, (HL)
        case 0xD7:
            // SET 2, A
        case 0xD8:
            // SET 3, B
        case 0xD9:
            // SET 3, C
        case 0xDA:
            // SET 3, D
        case 0xDB:
            // SET 3, E
        case 0xDC:
            // SET 3, H
        case 0xDD:
            // SET 3, L
        case 0xDE:
            // SET 3, (HL)
        case 0xDF:
            // SET 3, A
        case 0xE0:
            // SET 4, B
        case 0xE1:
            // SET 4, C
        case 0xE2:
            // SET 4, D
        case 0xE3:
            // SET 4, E
        case 0xE4:
            // SET 4, H
        case 0xE5:
            // SET 4, L
        case 0xE6:
            // SET 4, (HL)
        case 0xE7:
            // SET 4, A
        case 0xE8:
            // SET 5, B
        case 0xE9:
            // SET 5, C
        case 0xEA:
            // SET 5, D
        case 0xEB:
            // SET 5, E
        case 0xEC:
            // SET 5, H
        case 0xED:
            // SET 5, L
        case 0xEE:
            // SET 5, (HL)
        case 0xEF:
            // SET 5, A
        case 0xF0:
            // SET 6, B
        case 0xF1:
            // SET 6, C
        case 0xF2:
            // SET 6, D
        case 0xF3:
            // SET 6, E
        case 0xF4:
            // SET 6, H
        case 0xF5:
            // SET 6, L
        case 0xF6:
            // SET 6, (HL)
        case 0xF7:
            // SET 6, A
        case 0xF8:
            // SET 7, B
        case 0xF9:
            // SET 7, C
        case 0xFA:
            // SET 7, D
        case 0xFB:
            // SET 7, E
        case 0xFC:
            // SET 7, H
        case 0xFD:
            // SET 7, L
        case 0xFE:
            // SET 7, (HL)
        case 0xFF:
            // SET 7, A
        default:
            return new NOP(registers);
    }
}
