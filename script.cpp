#define dgen_lw_ins(name, rs1_, rs2_, imm_) rv::rv_inst name; name.op = rv::rvOPCODE::LW; name.rs1 = rs1_; name.rs2 = rs2_; name.imm = imm_; rv_insts.push_back(name)
#define dgen_sw_ins(name, rs1_, rs2_, imm_) rv::rv_inst name; name.op = rv::rvOPCODE::SW; name.rs1 = rs1_; name.rs2 = rs2_; name.imm = imm_; stack_space+=4; rv_insts.push_back(name)
#define dgen_addi_ins(name, rd_, rs1_, imm_) rv::rv_inst name; name.op = rv::rvOPCODE::ADDI; name.rd = rd_; name.rs1 = rs1_; name.imm = imm_; rv_insts.push_back(name)
#define dgen_li_ins(name, rd_, imm_) rv::rv_inst name; name.op = rv::rvOPCODE::LI; name.rd = rd_; name.imm = imm_; rv_insts.push_back(name)
#define dgen_nop_ins(name) rv::rv_inst name; name.op = rv::rvOPCODE::NOP; rv_insts.push_back(name)
#define dgen_jr_ins(name, rs1_) rv::rv_inst name; name.op = rv::rvOPCODE::JR; name.rs1 = rs1_; rv_insts.push_back(name)

std::string rv::toString(rvOPCODE r){
	if(r == rv::rvOPCODE::LW){return "lw";}
	else if(r == rv::rvOPCODE::SW){return "sw";}
	else if(r == rv::rvOPCODE::ADDI){return "addi";}
	else if(r == rv::rvOPCODE::LI){return "li";}
	else if(r == rv::rvOPCODE::NOP){return "nop";}
	else if(r == rv::rvOPCODE::JR){return "jr";}
	else{error();}
}

