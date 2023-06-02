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
#define dgen_word(name) this->fout<<("\t.word " + std::string(name) + " \n") //name必须是string 类型
#define dgen_zero(name) this->fout<<("\t.zero " + std::string(name) + " \n") //name必须是string 类型
#define dgen_label(name) this->fout<<(std::string(name) + ":\n")
#define dgen_ins_str(ins_str) this->fout<<"\t"<<std::string(ins_str)<<" \n" 
#define literal_check(type_) (type_ == ir::Type::FloatLiteral || type_ == ir::Type::IntLiteral)
#define dgen_lw_ins(name, rs1_, rs2_, imm_) rv::rv_inst name; name.op = rv::rvOPCODE::LW; name.rs1 = rs1_; name.rs2 = rs2_; name.imm = imm_; rv_insts.push_back(name)
#define dgen_lwlable_ins(name,rd_,label_) rv::rv_inst name;  name.op = rv::rvOPCODE::LW; name.rd = rd_; name.label = label_; rv_insts.push_back(name)
#define dgen_sw_ins(name, rs1_, rs2_, imm_) rv::rv_inst name; name.op = rv::rvOPCODE::SW; name.rs1 = rs1_; name.rs2 = rs2_; name.imm = imm_; rv_insts.push_back(name)
#define dgen_swlable_ins(name,rd_,label_,rs2_) rv::rv_inst name; name.op = rv::rvOPCODE::SW; name.rd = rd_; name.label = label_; name.rs2 = rs2_; rv_insts.push_back(name)
#define dgen_addi_ins(name, rd_, rs1_, imm_) rv::rv_inst name; name.op = rv::rvOPCODE::ADDI; name.rd = rd_; name.rs1 = rs1_; name.imm = imm_; rv_insts.push_back(name)
#define dgen_addilabel_ins(name, rd_, rs1_, label_) rv::rv_inst name; name.op = rv::rvOPCODE::ADDI; name.rd = rd_; name.rs1 = rs1_; name.label = label_; rv_insts.push_back(name)
#define dgen_li_ins(name, rd_, imm_) rv::rv_inst name; name.op = rv::rvOPCODE::LI; name.rd = rd_; name.imm = imm_; rv_insts.push_back(name)
#define dgen_nop_ins(name) rv::rv_inst name; name.op = rv::rvOPCODE::NOP; rv_insts.push_back(name)
#define dgen_jr_ins(name, rs1_) rv::rv_inst name; name.op = rv::rvOPCODE::JR; name.rs1 = rs1_; rv_insts.push_back(name)
#define dgen_call_ins(name,label_) rv::rv_inst name; name.op = rv::rvOPCODE::CALL; name.label = label_; rv_insts.push_back(name)
#define dgen_mv_ins(name, rd_, rs2_) rv::rv_inst name; name.op = rv::rvOPCODE::MV; name.rd = rd_; name.rs2 = rs2_; rv_insts.push_back(name)
#define dgen_jal_ins(name, label_) rv::rv_inst name; name.op = rv::rvOPCODE::JAL; name.label = label_; rv_insts.push_back(name)
#define dgen_add_ins(name, rd_, rs1_, rs2_) rv::rv_inst name; name.op = rv::rvOPCODE::ADD; name.rd = rd_; name.rs1 = rs1_; name.rs2 = rs2_; rv_insts.push_back(name)
#define dgen_sub_ins(name, rd_, rs1_, rs2_) rv::rv_inst name; name.op = rv::rvOPCODE::SUB; name.rd = rd_; name.rs1 = rs1_; name.rs2 = rs2_; rv_insts.push_back(name)
#define dgen_mul_ins(name, rd_, rs1_, rs2_) rv::rv_inst name; name.op = rv::rvOPCODE::MUL; name.rd = rd_; name.rs1 = rs1_; name.rs2 = rs2_; rv_insts.push_back(name)
#define dgen_div_ins(name, rd_, rs1_, rs2_) rv::rv_inst name; name.op = rv::rvOPCODE::DIV; name.rd = rd_; name.rs1 = rs1_; name.rs2 = rs2_; rv_insts.push_back(name)
#define standard_load_op(reg_name,op) load_op(op,stvm,reg_name,rv_insts);
#define standard_store_op(reg_name,op) store_op(op,stvm,reg_name,rv_insts,stack_ptr);
#define dgen_slli_ins(name, rd_, imm_) rv::rv_inst name; name.op = rv::rvOPCODE::SLLI; name.rd = rd_; name.imm = imm_; rv_insts.push_back(name)
#define dgen_luilabel_ins(name,rd_,label_)  rv::rv_inst name; name.op = rv::rvOPCODE::LUI; name.rd = rd_; name.label = label_; rv_insts.push_back(name)

backend::Generator::Generator(ir::Program& p, std::ofstream& f): program(p), fout(f) {}

//实现draw函数
std::string rv::rv_inst::draw() const{
    std::string ret = "\t";
    switch (this->op)
    {
    //op
    case rv::rvOPCODE::NOP:
        ret += toString(this->op) + "\n";
        break;

    //op rd, imm
    case rv::rvOPCODE::SLLI:
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
        if(this->label==""){
            ret+= rv::toString(this->op) + "\t";
            ret+= rv::toString(this->rs2) + ", ";
            ret+= std::to_string(this->imm) + "("; 
            ret+= rv::toString(this->rs1) + ")\n";
        }
        else{
            ret+= rv::toString(this->op) + "\t";
            ret+= rv::toString(this->rd) + ", ";
            ret+= this->label + ", ";
            ret+= rv::toString(this->rs2) + "\n";
        }
        break;
    // op rs2, imm(rs1)
    case rv::rvOPCODE::LW:
        if(this->label==""){
            ret+= rv::toString(this->op) + "\t";
            ret+= rv::toString(this->rs2) + ", ";
            ret+= std::to_string(this->imm) + "("; 
            ret+= rv::toString(this->rs1) + ")\n";
        }
        else{
            ret+= rv::toString(this->op) + "\t";
            ret+= rv::toString(this->rd) + ", ";
            ret+= this->label + "\n ";
        }
        break;

    // op rd,rs1,imm
    case rv::rvOPCODE::ADDI:
        ret+= rv::toString(this->op) + "\t";
        ret+= rv::toString(this->rd) + ", ";
        ret+= rv::toString(this->rs1) + ", ";
        if(this->label!=""){
            ret+= "\%lo(" + this->label+ ")\n";
        }else{
            ret+= std::to_string(this->imm)+ "\n";
        }
        break;

    //op label
    case rv::rvOPCODE::JAL:
    case rv::rvOPCODE::CALL:
        ret+= rv::toString(this->op) + "\t";
        ret+= this->label + "\n";
        break;

    //op rd,rs2
    case rv::rvOPCODE::MV:
        ret+= rv::toString(this->op) + "\t";
        ret+= rv::toString(this->rd) + ", ";
        ret+= rv::toString(this->rs2) + "\n";
        break;
    //op rd,rs1,rs2
    case rv::rvOPCODE::ADD:
    case rv::rvOPCODE::SUB:
    case rv::rvOPCODE::MUL:
    case rv::rvOPCODE::DIV:
        ret+= rv::toString(this->op) + "\t";
        ret+= rv::toString(this->rd) + ", ";
        ret+= rv::toString(this->rs1) + ", ";
        ret+= rv::toString(this->rs2)+ "\n";
        break;
    //op rd, %hi(label)
    case rv::rvOPCODE::LUI:
        ret+= rv::toString(this->op) + "\t";
        ret+= rv::toString(this->rd) + ", ";
        ret+=  "\%hi(" + this->label+ ")\n";
        break;
    default:
        error();
        break;
    }
    return ret;
}



void backend::Generator::gen() {
    /***声明全局变量****/
    //处理全局变量,分配值
    fout<<"\t.section .sdata\n";
    for(auto g : this->program.globalVal){
        ir::Operand op = g.val;
        this->is_global[op.name] = true;
        if(op.type==ir::Type::Int){
            dgen_global(op.name);
            dgen_label(op.name);
            dgen_word("0");
        }
        else if(op.type==ir::Type::IntPtr){
            dgen_global(op.name);
            dgen_type(op.name,"object");
            dgen_label(op.name);
            dgen_zero(std::to_string(g.maxlen*4));
        }
        else{
            //浮点数
            todo();
        }
    }

    //处理函数
    fout<<"\t.text\n";
    for(ir::Function func:this->program.functions){
        dgen_global(func.name);
        dgen_type(func.name,"function");
        gen_func(func);
    }
}

void backend::Generator::gen_func(ir::Function& func){
    //进入函数声明
    dgen_label(func.name);
    int stack_space = 4;
    int stack_ptr = 0;//相对于s0
    int reverse_ptr = 0;//相对于sp
    std::vector<rv::rv_inst> rv_insts;
    /***函数开始工作**/
    //偏移栈指针
    dgen_addi_ins(add_sp,rv::rvREG::sp,rv::rvREG::sp,0);//imm先占位,走完整个函数修改
    //存入帧指针
    dgen_sw_ins(sw_s0,rv::rvREG::sp,rv::rvREG::s0,0);//imm先占位,走完整个函数修改
    stack_ptr+=4;
    //偏移帧指针
    dgen_addi_ins(add_s0,rv::rvREG::s0,rv::rvREG::sp,0);//imm先占位,走完整个函数修改
    stackVarMap stvm;
    /****存储参数偏移量****/
    for(int i = func.ParameterList.size()-1;i>=0;i--){
        int off = -(func.ParameterList.size()-1-i) * 4;
        stvm._table[func.ParameterList[i].name] = off;
    }
    /*****进入函数指令*****/
    for(auto ins:func.InstVec){
        gen_instr(ins,rv_insts,stack_ptr,reverse_ptr,stvm);
    }
    //填充占位
    stack_space = stack_ptr  + reverse_ptr;
    rv_insts[0].imm = -stack_space;//add_sp
    rv_insts[1].imm = stack_space-4;//sw_s0
    rv_insts[2].imm = stack_space;
    //取出s0
    dgen_lw_ins(lw_s0,rv::rvREG::sp,rv::rvREG::s0,rv_insts[1].imm);
    //恢复sp
    dgen_addi_ins(recover_sp,rv::rvREG::sp,rv::rvREG::sp,-rv_insts[0].imm);
    dgen_jr_ins(jr_ra,rv::rvREG::ra);
    //写入文件
    for(auto ri : rv_insts){
        fout<<ri.draw();
    }
}

void backend::Generator::load_op(ir::Operand op, backend::stackVarMap& stvm, rv::rvREG reg, std::vector<rv::rv_inst>& rv_insts){
    if(op.type==ir::Type::IntLiteral){
        dgen_li_ins(li_op1,reg,this->analyzer.intstring_to_int(op.name));
    }
    else if(op.type==ir::Type::Int){
        //判断全局变量
        if(this->is_global.count(op.name)!=0){
            dgen_lwlable_ins(lw_gv,reg,op.name);
        }
        else{
            int off = stvm._table[op.name];
            dgen_lw_ins(lw_lv,rv::rvREG::s0,reg,-off);
        }
    }
    else{
        todo();
    }
}

void backend::Generator::store_op(ir::Operand op,  backend::stackVarMap& stvm, rv::rvREG reg, std::vector<rv::rv_inst>& rv_insts, int& stack_space){
    //判断是否为全局变量
    if(this->is_global.count(op.name)!=0){
        dgen_swlable_ins(sw_gv,reg,op.name,getRs2({}));//Operand为空表示临时变量
    }
    else{
        //内存管理映射
        if(stvm._table.count(op.name)==0){
            if(op.type==ir::Type::Int){
                stack_space+=4;
                stvm._table[op.name] = stack_space;
            }
            else if(op.type==ir::Type::Float){
                todo();
            }
            else{
                error();
            }
        }
        int off = stvm._table[op.name];
        dgen_sw_ins(sw_def_des,rv::rvREG::s0,reg,-off);
    }
}

void backend::Generator::gen_instr(ir::Instruction* ins, std::vector<rv::rv_inst>& rv_insts,int& stack_ptr,int&  reverse_ptr,stackVarMap& stvm){
    if(ins->op==ir::Operator::_return){
        if(literal_check(ins->op1.type)){
            if(ins->op1.type==ir::Type::IntLiteral){
                dgen_li_ins(li_literal,rv::rvREG::a0,this->analyzer.intstring_to_int(ins->op1.name));
            }
            else{
                todo();
            }
        }
        else if(ins->op1.type==ir::Type::null){
            //do Nothing.
            dgen_nop_ins(nop_ret);
        }
        else if(ins->op1.type==ir::Type::Int){
            standard_load_op(rv::rvREG::a0,ins->op1);
        }
        else{
            todo();
        }
    }
    else if(ins->op==ir::Operator::call){
        ir::CallInst* cins = ((ir::CallInst*)ins);
        //add call;
        //存入ra
        stack_ptr+=4;
        int tmp = -stack_ptr;
        dgen_sw_ins(sw_ra,rv::rvREG::s0,rv::rvREG::ra,tmp);
        if(!cins->argumentList.empty()){
            //存入参数值
            rv::rvREG tmp_reg = rv::rvREG::t3;
            for(int i = 0;i<cins->argumentList.size();i++){
                ir::Operand op = cins->argumentList[i];
                if(literal_check(op.type)){
                    if(op.type==ir::Type::IntLiteral){
                        dgen_li_ins(li_val,tmp_reg,this->analyzer.intstring_to_int(op.name));
                    }
                    else{
                        todo();
                    }
                }
                else{
                    if(op.type==ir::Type::Int){
                        standard_load_op(tmp_reg,op);
                    }
                    else{
                        todo();
                    }
                }
                //写入
                dgen_sw_ins(sw_param,rv::rvREG::sp,tmp_reg,reverse_ptr);
                reverse_ptr+=4;
            }
        }
        //call
        dgen_call_ins(call_func,cins->op1.name);
        //取出返回值
        if(ins->des.type!=ir::Type::null){
            standard_store_op(rv::rvREG::a0,ins->des);
        }
        //取回ra
        dgen_lw_ins(lw_ra,rv::rvREG::s0,rv::rvREG::ra,tmp);
    }
    else if(ins->op==ir::Operator::def || ins->op==ir::Operator::mov){
        //加载def.op1
        rv::rvREG tmp_op1 = rv::rvREG::t3;
        standard_load_op(tmp_op1,ins->op1);
        //写入值
        standard_store_op(tmp_op1,ins->des);
    }
    else if(ins->op==ir::Operator::add ||ins->op==ir::Operator::sub || ins->op==ir::Operator::mul || ins->op==ir::Operator::div ){
        rv::rvREG tmp_op1 =rv::rvREG::t3;
        rv::rvREG tmp_op2 = rv::rvREG::t4;
        standard_load_op(tmp_op1,ins->op1);
        standard_load_op(tmp_op2,ins->op2);
        rv::rvREG tmp_des = rv::rvREG::t5;
        if(ins->op==ir::Operator::add){
            dgen_add_ins(add_ins,tmp_des,tmp_op1,tmp_op2);
        }else if(ins->op==ir::Operator::sub){
            dgen_sub_ins(sub_ins,tmp_des,tmp_op1,tmp_op2);
        }else if(ins->op==ir::Operator::mul){
            dgen_mul_ins(mul_ins,tmp_des,tmp_op1,tmp_op2);
        }else if(ins->op==ir::Operator::div){
            dgen_div_ins(div_ins,tmp_des,tmp_op1,tmp_op2);
        }else{
            error();
        }
        standard_store_op(tmp_des,ins->des);
    }
    else if(ins->op == ir::Operator::store){
        rv::rvREG off_reg = rv::rvREG::t3;
        if(this->is_global.count(ins->op1.name)!=0){
            dgen_luilabel_ins(lui_ga,off_reg,ins->op1.name);
            dgen_addilabel_ins(addi_ga,off_reg,off_reg,ins->op1.name);
            rv::rvREG tmp_mul4 = rv::rvREG::t4;
            if(literal_check(ins->op2.type)){
                dgen_li_ins(li_mul4,tmp_mul4,this->analyzer.intstring_to_int(ins->op2.name) * 4);
            }else{
                standard_load_op(tmp_mul4,ins->op2);
                dgen_slli_ins(slli_mul4,tmp_mul4,2);
            }
            dgen_add_ins(add_addr,off_reg,off_reg,tmp_mul4);
        }
        else{
            //考虑数组过大
            int off = stvm._table[ins->op1.name];
            dgen_li_ins(li_off,off_reg,off);
            rv::rvREG tmp_mul4 = rv::rvREG::t4;
            if(literal_check(ins->op2.type)){
                dgen_li_ins(li_mul4,tmp_mul4,this->analyzer.intstring_to_int(ins->op2.name) * 4);
            }else{
                standard_load_op(tmp_mul4,ins->op2);
                dgen_slli_ins(slli_mul4,tmp_mul4,2);
            }
            dgen_add_ins(add_off,off_reg,off_reg,tmp_mul4);
            dgen_sub_ins(sub_addr,off_reg,rv::rvREG::s0,off_reg);
        }
        //存值
        rv::rvREG sw_val_reg = rv::rvREG::t5;
        standard_load_op(sw_val_reg,ins->des);
        dgen_sw_ins(sw_arr,off_reg,sw_val_reg,0);
    }
    else if(ins->op == ir::Operator::alloc){
        //只需要移动栈指针即可
        stvm._table[ins->des.name] = stack_ptr+4;
        int len = this->analyzer.intstring_to_int(ins->op1.name) * 4;
        stack_ptr += len;
    }
    else if(ins->op == ir::Operator::load){
        rv::rvREG off_reg = rv::rvREG::t3;
        if(this->is_global.count(ins->op1.name)!=0){
            dgen_luilabel_ins(lui_ga,off_reg,ins->op1.name);
            dgen_addilabel_ins(addi_ga,off_reg,off_reg,ins->op1.name);
            rv::rvREG tmp_mul4 = rv::rvREG::t4;
            if(literal_check(ins->op2.type)){
                dgen_li_ins(li_mul4,tmp_mul4,this->analyzer.intstring_to_int(ins->op2.name) * 4);
            }else{
                standard_load_op(tmp_mul4,ins->op2);
                dgen_slli_ins(slli_mul4,tmp_mul4,2);
            }
            dgen_add_ins(add_addr,off_reg,off_reg,tmp_mul4);
        }
        else{
            //考虑数组过大....
            int off = stvm._table[ins->op1.name];
            dgen_li_ins(li_off,off_reg,off);
            rv::rvREG tmp_mul4 = rv::rvREG::t4;
            if(literal_check(ins->op2.type)){
                dgen_li_ins(li_mul4,tmp_mul4,this->analyzer.intstring_to_int(ins->op2.name) * 4);
            }else{
                standard_load_op(tmp_mul4,ins->op2);
                dgen_slli_ins(slli_mul4,tmp_mul4,2);
            }
            dgen_add_ins(add_off,off_reg,off_reg,tmp_mul4);
            dgen_sub_ins(sub_addr,off_reg,rv::rvREG::s0,off_reg);
        }
        
        //写入值
        rv::rvREG lw_val = rv::rvREG::t5;
        dgen_lw_ins(lw_arr,off_reg,lw_val,0);
        standard_store_op(lw_val,ins->des);
    }
    else{
        todo();
    }
}

std::string rv::toString(rvFREG r){
    todo();
}

std::string rv::toString(rvOPCODE r){
	if(r == rv::rvOPCODE::LW){return "lw";}
	else if(r == rv::rvOPCODE::SW){return "sw";}
	else if(r == rv::rvOPCODE::ADDI){return "addi";}
	else if(r == rv::rvOPCODE::LI){return "li";}
	else if(r == rv::rvOPCODE::NOP){return "nop";}
	else if(r == rv::rvOPCODE::JR){return "jr";}
    else if(r == rv::rvOPCODE::CALL){return "call";}
    else if(r == rv::rvOPCODE::MV){return "mv";}
    else if(r == rv::rvOPCODE::JAL){return "jal";}
    else if(r == rv::rvOPCODE::ADD){return "add";}
	else if(r == rv::rvOPCODE::SUB){return "sub";}
	else if(r == rv::rvOPCODE::MUL){return "mul";}
	else if(r == rv::rvOPCODE::DIV){return "div";}
    else if(r == rv::rvOPCODE::SLLI){return "slli";}
    else if(r == rv::rvOPCODE::LUI){return "lui";}
	else{error();}
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

rv::rvREG backend::Generator::getRd(ir::Operand){
    return rv::rvREG::t0;
}
rv::rvFREG backend::Generator::fgetRd(ir::Operand){
    todo();
}
rv::rvREG backend::Generator::getRs1(ir::Operand){
    return rv::rvREG::t1;
}
rv::rvREG backend::Generator::getRs2(ir::Operand){
    return rv::rvREG::t2;
}
rv::rvFREG backend::Generator::fgetRs1(ir::Operand){
    todo();
}
rv::rvFREG backend::Generator::fgetRs2(ir::Operand){
    todo();
}




