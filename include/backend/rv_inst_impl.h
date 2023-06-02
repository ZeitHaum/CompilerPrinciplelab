#ifndef RV_INST_IMPL_H
#define RV_INST_IMPL_H

#include "backend/rv_def.h"

namespace rv {

struct rv_inst {
    rvREG rd, rs1, rs2;         // operands of rv inst
    rvOPCODE op;                // opcode of rv inst
    
    int imm;  //32 bit             // optional, in immediate inst
    std::string label;          // optional, in beq/jarl inst

    std::string draw() const;
    // rv_inst(rvOPCODE op,rvREG rd,rvREG rs1, int imm):op(op),rd(rd),rs1(rs1),imm(imm){}
    rv_inst():label(""){}
};

//TODO rv_finst


};

#endif