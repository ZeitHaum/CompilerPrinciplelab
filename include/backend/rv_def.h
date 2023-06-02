#ifndef RV_DEF_H
#define RV_DEF_H

#include<string>

namespace rv {

// rv interger registers
enum class rvREG {
    /* Xn       its ABI name*/
    X0,         // zero
    X1,         // ra
    X2,         // sp
    X3,         // gp
    X4,         // tp
    X5,         // t0
    X6,         //
    X7,
    X8,
    X9,
    X10,
    X11,
    X12,
    X13,
    X14,
    X15,
    X16,
    X17,
    X18,
    X19,
    X20,
    X21,
    X22,
    X23,
    X24,
    X25,
    X26,
    X27,
    X28,
    X29,
    X30,
    X31,
	zero = X0,
	ra = X1,
	sp = X2,
	gp = X3,
	tp = X4,
	t0 = X5,
	t1 = X6,
	t2 = X7,
	s0 = X8,
	s1 = X9,
	a0 = X10,
	a1 = X11,
	a2 = X12,
	a3 = X13,
	a4 = X14,
	a5 = X15,
	a6 = X16,
	a7 = X17,
	s2 = X18,
	s3 = X19,
	s4 = X20,
	s5 = X21,
	s6 = X22,
	s7 = X23,
	s8 = X24,
	s9 = X25,
	s10 = X26,
	s11 = X27,
	t3 = X28,
	t4 = X29,
	t5 = X30,
	t6 = X31,
    null,//不使用寄存器
};
std::string toString(rvREG r);  // implement this in ur own way

enum class rvFREG {
    F0,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    F16,
    F17,
    F18,
    F19,
    F20,
    F21,
    F22,
    F23,
    F24,
    F25,
    F26,
    F27,
    F28,
    F29,
    F30,
    F31,
    null,
};
std::string toString(rvFREG r);  // implement this in ur own way

// rv32i instructions
// add instruction u need here!
enum class rvOPCODE {
    // // RV32I Base Integer Instructions
    // ADD, SUB, XOR, OR, AND, SLL, SRL, SRA, SLT, SLTU,       // arithmetic & logic
    // ADDI, XORI, ORI, ANDI, SLLI, SRLI, SRAI, SLTI, SLTIU,   // immediate
    // LW, SW,                                                 // load & store
    // BEQ, BNE, BLT, BGE, BLTU, BGEU,                         // conditional branch
    // JAL, JALR, JR,                                         // jump

    // // RV32M Multiply Extension
    // MUL,
    // // RV32F / D Floating-Point Extensions

    // // Pseudo Instructions
    // LA, LI, MOV, J,                                         // ...
    // //不使用
    // NOP,
    //未添加浮点数
    LW, SW, ADDI, LI, NOP, JR, CALL, MV, JAL,ADD, SUB, MUL, DIV, SLLI, LUI,
};
std::string toString(rvOPCODE r);  // implement this in ur own way


} // namespace rv



#endif