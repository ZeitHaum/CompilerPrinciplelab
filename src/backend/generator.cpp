#include"backend/generator.h"
#include "backend/rv_def.h"
#include "backend/rv_inst_impl.h"
#include "front/semantic.h"
#include<assert.h>
#include<string>
#include<iostream>

#define todo() assert(0 && "todo")
#define warning_todo() std::cerr<<"This work is not complete in line "<<__LINE__<<"\n"
#define error() assert(0 && "RISCV generator error!")
#define dgen_global(name) this->fout<<("\t.global " + std::string(name) + " \n") //name必须是string 类型
#define dgen_type(name,type) this->fout<<("\t.type " + std::string(name) + ", @" + std::string(type) + " \n")
#define dgen_label(name) this->fout<<(std::string(name) + ":\n")
#define dgen_ins_str(ins_str) this->fout<<"\t"<<std::string(ins_str)<<" \n" 
#define literal_check(type_) (type_ == ir::Type::FloatLiteral || type_ == ir::Type::IntLiteral)
#define dgen_lw_ins(name,rs2_,rs1_,imm_) rv::rv_inst name; name.op = rv::rvOPCODE::LW; name.rs2 = rs2_; name.rs1 = rs1_; name.imm = imm_; rv_insts.push_back(name);
#define dgen_sw_ins(name,rs2_,rs1_,imm_) rv::rv_inst name; name.op = rv::rvOPCODE::SW; name.rs2 = rs2_; name.rs1 = rs1_; name.imm = imm_; stack_space+=4;rv_insts.push_back(name);



backend::Generator::Generator(ir::Program& p, std::ofstream& f): program(p), fout(f) {}

//实现draw函数
std::string rv::rv_inst::draw() const{
    //未考虑imm和label以及浮点数
    warning_todo();
    std::string ret = "\t";
    switch (this->op)
    {
    //op
    case rv::rvOPCODE::NOP:
        ret += toString(this->op) + "\n";
        break;

    //op rd, imm
    case rv::rvOPCODE::LI:
        ret+= rv::toString(this->op) + "\t";
        ret+= rv::toString(this->rd) + ", ";
        ret+= std::to_string(this->imm) + "\n"; 
        break;
    //op rs1
    case rv::rvOPCODE::JR:
        ret+= rv::toString(this->op) + "\t";
        ret+= rv::toString(this->rs1) + "\n";
        break;
    // op rs2, imm(rs1)
    case rv::rvOPCODE::SW:
    case rv::rvOPCODE::LW:
        ret+= rv::toString(this->op) + "\t";
        ret+= rv::toString(this->rs2) + ", ";
        ret+= std::to_string(this->imm) + "("; 
        ret+= rv::toString(this->rs1) + ")\n";
        break;

    // op rd,rs1,imm
    case rv::rvOPCODE::ADDI:
        ret+= rv::toString(this->op) + "\t";
        ret+= rv::toString(this->rd) + ", ";
        ret+= rv::toString(this->rs1) + ", ";
        ret+= std::to_string(this->imm)+ "\n";
        break;
    default:
        error();
        break;
    }
    return ret;
}



void backend::Generator::gen() {
    /***声明全局变量****/
    //声明所有函数(除了global)
    for(ir::Function func:this->program.functions){
        if(func.name=="global"){
            continue;
        }
        dgen_global(func.name);
        dgen_type(func.name,"function");
    }
    //处理全局变量
    warning_todo();
    //处理函数
    for(ir::Function func:this->program.functions){
        if(func.name!="global"){
            gen_func(func);
        }
    }
}



void backend::Generator::gen_func(const ir::Function& func){
    //进入函数声明
    dgen_label(func.name);
    int stack_space = 0;
    std::vector<rv::rv_inst> rv_insts;
    /***函数开始工作**/
    //偏移栈指针
    //考虑数组过大....
    warning_todo();
    rv_insts.push_back({rv::rvOPCODE::ADDI,rv::rvREG::sp,rv::rvREG::sp,0});//imm先占位,走完整个函数修改
    //存入帧指针
    dgen_sw_ins(sw_s0,rv::rvREG::s0,rv::rvREG::sp,0);//imm先占位,走完整个函数修改
    //偏移帧指针
    rv_insts.push_back({rv::rvOPCODE::ADDI,rv::rvREG::s0,rv::rvREG::s0,0});//imm先占位,走完整个函数修改
    /*****进入函数指令*****/
    for(auto ins:func.InstVec){
        gen_instr(ins,rv_insts,stack_space);
    }
    //取出s0
    dgen_lw_ins(lw_s0,rv::rvREG::s0,rv::rvREG::sp,stack_space);
    //恢复sp
    rv_insts.push_back({rv::rvOPCODE::ADDI,rv::rvREG::sp,rv::rvREG::sp,stack_space});
    //填充占位
    rv_insts[0].imm = -stack_space;
    rv_insts[1].imm = stack_space;
    rv_insts.push_back({rv::rvOPCODE::JR,{},rv::rvREG::ra,{}});
    //写入文件
    for(auto ri : rv_insts){
        fout<<ri.draw();
    }
}

void backend::Generator::gen_instr(ir::Instruction* ins, std::vector<rv::rv_inst>& rv_insts,int& stack_space){
    if(ins->op==ir::Operator::_return){
        if(literal_check(ins->op1.type)){
            if(ins->op1.type==ir::Type::IntLiteral){
                rv::rv_inst li_ins = rv::rv_inst();
                li_ins.op = rv::rvOPCODE::LI;
                li_ins.imm = this->analyzer.intstring_to_int(ins->op1.name);
                li_ins.rd = rv::rvREG::a0;//存储返回值
                rv_insts.push_back(li_ins);
            }
            else{
                todo();
            }
        }
        else{
            todo();
        }
    }
    else if(ins->op==ir::Operator::call){
        if(((ir::CallInst*)ins)->op1.name=="global"){
            //call global无效
            return;
        }
    }
    else{
        todo();
    }
}

std::string rv::toString(rvFREG r){
    todo();
}

std::string rv::toString(rvOPCODE r){
    if(r == rv::rvOPCODE::ADD){return "add";}
    else if(r == rv::rvOPCODE::SUB){return "sub";}
    else if(r == rv::rvOPCODE::XOR){return "xor";}
    else if(r == rv::rvOPCODE::OR){return "or";}
    else if(r == rv::rvOPCODE::AND){return "and";}
    else if(r == rv::rvOPCODE::SLL){return "sll";}
    else if(r == rv::rvOPCODE::SRL){return "srl";}
    else if(r == rv::rvOPCODE::SRA){return "sra";}
    else if(r == rv::rvOPCODE::SLT){return "slt";}
    else if(r == rv::rvOPCODE::SLTU){return "sltu";}
    else if(r == rv::rvOPCODE::ADDI){return "addi";}
    else if(r == rv::rvOPCODE::XORI){return "xori";}
    else if(r == rv::rvOPCODE::ORI){return "ori";}
    else if(r == rv::rvOPCODE::ANDI){return "andi";}
    else if(r == rv::rvOPCODE::SLLI){return "slli";}
    else if(r == rv::rvOPCODE::SRLI){return "srli";}
    else if(r == rv::rvOPCODE::SRAI){return "srai";}
    else if(r == rv::rvOPCODE::SLTI){return "slti";}
    else if(r == rv::rvOPCODE::SLTIU){return "sltiu";}
    else if(r == rv::rvOPCODE::LW){return "lw";}
    else if(r == rv::rvOPCODE::SW){return "sw";}
    else if(r == rv::rvOPCODE::BEQ){return "beq";}
    else if(r == rv::rvOPCODE::BNE){return "bne";}
    else if(r == rv::rvOPCODE::BLT){return "blt";}
    else if(r == rv::rvOPCODE::BGE){return "bge";}
    else if(r == rv::rvOPCODE::BLTU){return "bltu";}
    else if(r == rv::rvOPCODE::BGEU){return "bgeu";}
    else if(r == rv::rvOPCODE::JAL){return "jal";}
    else if(r == rv::rvOPCODE::JALR){return "jalr";}
    else if(r == rv::rvOPCODE::LA){return "la";}
    else if(r == rv::rvOPCODE::LI){return "li";}
    else if(r == rv::rvOPCODE::MOV){return "mov";}
    else if(r == rv::rvOPCODE::J){return "j";}
    else if(r == rv::rvOPCODE::NOP){return "nop";}
    else if(r == rv::rvOPCODE::JR){return "jr";}
    else{{error();}}
}


std::string rv::toString(rvREG r){
    if(r == rv::rvREG::X0){return "zero";}
    else if(r == rv::rvREG::X1){return "ra";}
    else if(r == rv::rvREG::X2){return "sp";}
    else if(r == rv::rvREG::X3){return "gp";}
    else if(r == rv::rvREG::X4){return "tp";}
    else if(r == rv::rvREG::X5){return "t0";}
    else if(r == rv::rvREG::X6){return "t1";}
    else if(r == rv::rvREG::X7){return "t2";}
    else if(r == rv::rvREG::X8){return "s0";}
    else if(r == rv::rvREG::X9){return "s1";}
    else if(r == rv::rvREG::X10){return "a0";}
    else if(r == rv::rvREG::X11){return "a1";}
    else if(r == rv::rvREG::X12){return "a2";}
    else if(r == rv::rvREG::X13){return "a3";}
    else if(r == rv::rvREG::X14){return "a4";}
    else if(r == rv::rvREG::X15){return "a5";}
    else if(r == rv::rvREG::X16){return "a6";}
    else if(r == rv::rvREG::X17){return "a7";}
    else if(r == rv::rvREG::X18){return "s2";}
    else if(r == rv::rvREG::X19){return "s3";}
    else if(r == rv::rvREG::X20){return "s4";}
    else if(r == rv::rvREG::X21){return "s5";}
    else if(r == rv::rvREG::X22){return "s6";}
    else if(r == rv::rvREG::X23){return "s7";}
    else if(r == rv::rvREG::X24){return "s8";}
    else if(r == rv::rvREG::X25){return "s9";}
    else if(r == rv::rvREG::X26){return "s10";}
    else if(r == rv::rvREG::X27){return "s11";}
    else if(r == rv::rvREG::X28){return "t3";}
    else if(r == rv::rvREG::X29){return "t4";}
    else if(r == rv::rvREG::X30){return "t5";}
    else if(r == rv::rvREG::X31){return "t6";}
    else if(r == rv::rvREG::null){return "null";}
    else{error();}
}




