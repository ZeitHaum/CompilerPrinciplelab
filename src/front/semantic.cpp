#include"front/semantic.h"

#include<cassert>
#include<typeinfo>
#include<iostream>

using ir::Instruction;
using ir::Function;
using ir::Operand;
using ir::Operator;


#define todo() assert(0 && "TODO");
// #define GET_CHILD_PTR(node, type, index) auto node = dynamic_cast<type*>(root->children[index]); assert(node); 
// #define ANALYSIS(node, type, index) auto node = dynamic_cast<type*>(root->children[index]); assert(node); analysis##type(node, buffer);
// #define COPY_EXP_NODE(from, to) to->is_computable = from->is_computable; to->v = from->v; to->t = from->t;
#define error() assert(0 && "Semantic anlysis error!")
#define def_analyze(type) void frontend::Analyzer::analyze##type(type* root)
#define def_analyze_withret(type,returntype) returntype frontend::Analyzer::analyze##type(type* root)
#define def_analyze_withparams(type,params) void frontend::Analyzer::analyze##type(type* root,params)
#define def_analyze_withretparams(type,returntype,params) returntype frontend::Analyzer::analyze##type(type* root,params)
#define BEGIN_PTR_INIT() int b_ptr = 0;//begin_ptr
#define parse_bptr(type,name) if(b_ptr>=root->children.size()){error();} type* node_##name = (type*)root->children[b_ptr++]
#define parse_bptr_declared(type,name) if(b_ptr>=root->children.size()){error();} node_##name = (type*)root->children[b_ptr++]
#define new_func() if(this->func!=nullptr){error();} this->func = new Function()
#define add_func() this->anlyzed_p.addFunction(*func); this->func = nullptr
//ADD_INST 注意:不会做任何合法性检查
#define ADD_INST(new_inst) if(this->func==nullptr){this->g_init_inst.push_back(new_inst);} else{this->func->addInst(new_inst);}
#define ADD_INST_DEF(name,op1_,des_) Instruction* name = new Instruction();  name->op1 = op1_;    name->des = des_;   name->op = ir::Operator::def;ADD_INST(name) if(this->func==nullptr){this->anlyzed_p.globalVal.push_back({des_});}
#define ADD_INST_FDEF(name,op1_,des_) Instruction* name = new Instruction();  name->op1 = op1_;    name->des = des_;   name->op = ir::Operator::fdef;ADD_INST(name) if(this->func==nullptr){this->anlyzed_p.globalVal.push_back({des_});}
#define ADD_INST_MOV(name,op1_,des_) Instruction* name = new Instruction();  name->op1 = op1_;    name->des = des_;   name->op = ir::Operator::mov;ADD_INST(name)
#define ADD_INST_FMOV(name,op1_,des_) Instruction* name = new Instruction();  name->op1 = op1_;    name->des = des_;   name->op = ir::Operator::fmov;ADD_INST(name)
#define ADD_INST_ADD(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::add;ADD_INST(name)
#define ADD_INST_ADDI(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::addi;ADD_INST(name)
#define ADD_INST_FADD(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::fadd;ADD_INST(name)
#define ADD_INST_SUB(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::sub;ADD_INST(name)
#define ADD_INST_SUBI(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::subi;ADD_INST(name)
#define ADD_INST_FSUB(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::fsub;ADD_INST(name)
#define ADD_INST_MUL(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::mul;ADD_INST(name)
#define ADD_INST_FMUL(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::fmul;ADD_INST(name)
#define ADD_INST_DIV(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::div;ADD_INST(name)
#define ADD_INST_FDIV(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::fdiv;ADD_INST(name)
#define ADD_INST_MOD(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::mod;ADD_INST(name)
#define ADD_INST_LSS(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::lss;ADD_INST(name)
#define ADD_INST_FLSS(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::flss;ADD_INST(name)
#define ADD_INST_LEQ(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::leq;ADD_INST(name)
#define ADD_INST_FLEQ(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::fleq;ADD_INST(name)
#define ADD_INST_GTR(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::gtr;ADD_INST(name)
#define ADD_INST_FGTR(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::fgtr;ADD_INST(name)
#define ADD_INST_GEQ(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::geq;ADD_INST(name)
#define ADD_INST_FGEQ(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::fgeq;ADD_INST(name)
#define ADD_INST_EQ(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::eq;ADD_INST(name)
#define ADD_INST_FEQ(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::feq;ADD_INST(name)
#define ADD_INST_NEQ(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::neq;ADD_INST(name)
#define ADD_INST_FNEQ(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::fneq;ADD_INST(name)
#define ADD_INST__NOT(name,op1_,des_) Instruction* name = new Instruction();  name->op1 = op1_;    name->des = des_;   name->op = ir::Operator::_not;ADD_INST(name)
#define ADD_INST__AND(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::_and;ADD_INST(name)
#define ADD_INST__OR(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::_or;ADD_INST(name)
#define ADD_INST_ALLOC(name,op1_,des_) Instruction* name = new Instruction();  name->op1 = op1_;    name->des = des_;   name->op = ir::Operator::alloc;ADD_INST(name) if(this->func==nullptr){this->anlyzed_p.globalVal.push_back({des_});}
#define ADD_INST_LOAD(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::load;ADD_INST(name)
#define ADD_INST_STORE(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::store;ADD_INST(name)
#define ADD_INST_GETPTR(name,op1_,op2_,des_) Instruction* name = new Instruction();  name->op1 = op1_;   name->op2 = op2_;   name->des = des_;   name->op = ir::Operator::getptr;ADD_INST(name)
#define ADD_INST_CVT_I2F(name,op1_,des_) Instruction* name = new Instruction();  name->op1 = op1_;    name->des = des_;   name->op = ir::Operator::cvt_i2f;ADD_INST(name)
#define ADD_INST_CVT_F2I(name,op1_,des_) Instruction* name = new Instruction();  name->op1 = op1_;    name->des = des_;   name->op = ir::Operator::cvt_f2i;ADD_INST(name)
#define ADD_INST_RETURN(name,op1_) Instruction* name = new Instruction();  name->op1 = op1_;     name->op = ir::Operator::_return;ADD_INST(name)
#define ADD_INST_GOTO(name,op1_,des_) Instruction* name = new Instruction();  name->op1 = op1_;    name->des = des_;   name->op = ir::Operator::_goto;ADD_INST(name)
#define ADD_INST_UNUSE(name) Instruction* name = new Instruction();     name->op = ir::Operator::unuse;ADD_INST(name)
// #define ADD_INST_CALL(name,op1_,des_,paraVec)  ir::CallInst* name = new ir::CallInst(); name->op1 = op_1; name->des = des_; name->
#define ADD_INST_AUTOTYPE(name,type,inst_name,params) 
#define ANALYZE_CHILD() for(auto child:root->children){analyzeAstNode(child);}
#define cur_node_is(type_) ( (root->children[b_ptr]->type) == type_)
#define cur_termtoken_is(type_) (cur_node_is(NodeType::TERMINAL) && ((Term*)root->children[b_ptr])->token.type==type_)
#define literal_check(type_) (type_ == ir::Type::FloatLiteral || type_ == ir::Type::IntLiteral)
#define ptr_check(type_) (type_ == ir::Type::FloatPtr || type_ == ir::Type::IntPtr)
#define int_check(type_) (type_ == ir::Type::Int || type_ == ir::Type::IntLiteral)
#define float_check(type_) (type_ == ir::Type::Float || type_ == ir::Type::FloatLiteral)
#define root_exp_assign(name) root->is_computable = name->is_computable;root->op = name->op
#define debug_reach() std::cerr<<"successfully reach "<<__LINE__<<"!\n"
#define warning_todo() std::cerr<<"This work is not complete in line "<<__LINE__<<"\n"
#define ptr_to_literval(type) (var_to_literal(ptr_to_var(type)))
#define ASSERT_NULLFUNC() if(this->func==nullptr){assert(0 && "NULL FUNC!");}
#define ASSERT_WRONGRET() if(!(this->func->returnType==ir::Type::Int || this->func->returnType==ir::Type::Float || this->func->returnType==ir::Type::null)){error();}
#define ASSERT_WRONGRET_WITHOUTNULL() if(!(this->func->returnType==ir::Type::Int || this->func->returnType==ir::Type::Float)){error();}
#define PERFROM_OP(node_name,op_name) if(root->is_computable){ root->op = perform_literal(root->op,node_name->op,op_name->token.type);} else{ root->op =  op_to_var(root->op); node_name->op = op_to_var(node_name->op); root->op = perform_var(root->op,node_name->op,op_name->token.type);}

map<std::string,ir::Function*>* frontend::get_lib_funcs() {

    static map<std::string,ir::Function*> lib_funcs = {
        {"getint", new Function("getint", Type::Int)},
        {"getch", new Function("getch", Type::Int)},
        {"getfloat", new Function("getfloat", Type::Float)},
        {"getarray", new Function("getarray", {Operand("arr", Type::IntPtr)}, Type::Int)},
        {"getfarray", new Function("getfarray", {Operand("arr", Type::FloatPtr)}, Type::Int)},
        {"putint", new Function("putint", {Operand("i", Type::Int)}, Type::null)},
        {"putch", new Function("putch", {Operand("i", Type::Int)}, Type::null)},
        {"putfloat", new Function("putfloat", {Operand("f", Type::Float)}, Type::null)},
        {"putarray", new Function("putarray", {Operand("n", Type::Int), Operand("arr", Type::IntPtr)}, Type::null)},
        {"putfarray", new Function("putfarray", {Operand("n", Type::Int), Operand("arr", Type::FloatPtr)}, Type::null)},
    };
    return &lib_funcs;
}

int frontend::Analyzer::intstring_to_int(std::string s){
    return this->Atoi(s);
}
float frontend::Analyzer::floatstring_to_float(std::string s){
    return std::stof(s);
}

//字符串转换
std::string  frontend::Analyzer::floatstring_to_int(std::string s){
    int ftoi = (int)std::stof(s);
    return std::to_string(ftoi);
}
std::string  frontend::Analyzer::intstring_to_float(std::string s){
    float itof = (float)(this->Atoi(s));
    return std::to_string(itof);
}

//同步字面量类型
ir::Operand  frontend::Analyzer::sync_literal_type(Operand op,ir::Type to_type){
    if(!literal_check(op.type) || !literal_check(to_type)){
        error();
    }   
    if(op.type==to_type){
        //DO NOTHING
        return op;
    }
    else{
        if(op.type==ir::Type::FloatLiteral && to_type==ir::Type::IntLiteral){
            op.name = floatstring_to_int(op.name);
        }
        else if(op.type==ir::Type::IntLiteral && to_type==ir::Type::FloatLiteral){
            op.name = intstring_to_float(op.name);
        }
        else{
            error();
        }
        op.type = to_type;
        return op;
    }
    error();
    return {};
}

//类型映射
ir::Type frontend::Analyzer::var_to_literal(ir::Type t){
    if(t==ir::Type::Float){
        return ir::Type::FloatLiteral;
    }
    else if(t==ir::Type::Int){
        return ir::Type::IntLiteral;
    }
    error();
}

ir::Type frontend::Analyzer::literal_to_var(ir::Type t){
    if(t==ir::Type::FloatLiteral){
        return ir::Type::Float;
    }
    else if(t==ir::Type::IntLiteral){
        return ir::Type::Int;
    }
    error();
}

ir::Type frontend::Analyzer::ptr_to_var(ir::Type t){
    if(t==ir::Type::FloatPtr){
        return ir::Type::Float;
    }
    else if(t==ir::Type::IntPtr){
        return ir::Type::Int;
    }
    error();
}

ir::Operand frontend::Analyzer::sync_to_int(ir::Operand op){
    if(int_check(op.type)){
        return op;
    }
    if(op.type==ir::Type::Float){
        return sync_var_type(op,ir::Type::Int);
    }
    else if(op.type==ir::Type::FloatLiteral){
        return sync_literal_type(op,ir::Type::IntLiteral);
    }
    error();
}

//隐式转换int 为 float
void frontend::Analyzer::sync_literalop_type(ir::Operand& op1,ir::Operand& op2){
    if(!literal_check(op1.type) || !literal_check(op2.type)){
        error();
    } 
    if(op1.type==op2.type){
        return;
    }
    else{
        if(op1.type==ir::Type::IntLiteral && op2.type==ir::Type::FloatLiteral){
            op2 = sync_literal_type(op2,ir::Type::FloatLiteral);
            return;
        }
        else if(op1.type==ir::Type::FloatLiteral && op2.type==ir::Type::IntLiteral){
            op1 = sync_literal_type(op1,ir::Type::FloatLiteral);
            return;
        }
        error();
    }
    error();
}


//Operand 运算
//进制转换,仅限整数
int frontend::Analyzer::Atoi(std::string s)
{
    // 先判断进制和符号
    int base = 0;
    int begin = 0;
    if(s[0]=='-'){
        begin++;
    }
    if (s[begin] == '0' && s.size()-begin > 1)
    {
        if (s[begin+1] == 'b'){
            base = 2;
            begin += 2;
        }
        else if (s[begin+1] == 'x'){
            base = 16;
            begin += 2;
        }
        else if (s[begin+1] == 'h'){
            base = 16;
            begin += 2;
        }
            
        else if (s[begin+1] == 'd'){
            base = 10;
            begin +=2;
        }
        else
        {
            base = 8;
            begin++;
        }
    }
    else
    {
        base = 10;
    }
    if (base == 0)
        error();
    int ret = 0;
    auto mapping = [&](char x)
    {
        if (x <= '9' && x >= '0')
        {
            return x - '0';
        }
        else if (x <= 'Z' && x >= 'A')
        {
            return x - 'A' + 10;
        }
        else if (x <= 'z' && x >= 'a')
        {
            return x - 'a' + 10;
        }
        else
        {
            error();
            return 0;
        }
    };
    bool enable = false;
    for (int i = begin; i < s.size(); i++)
    {
        if (enable)
        {
            ret = ret * base;
        }
        else
        {
            enable = true;
        }
        ret += mapping(s[i]);
    }
    if(s[0]=='-'){
        ret = -ret;
    }
    return ret;
}

//任意操作
std::string frontend::Analyzer::perform_string(std::string s1,std::string s2, frontend::TokenType op, ir::Type addtype){
    if(addtype==ir::Type::IntLiteral){
        int i1,i2;
        i1 = Atoi(s1);
        i2 = Atoi(s2);
        if(op==TokenType::PLUS){
            return std::to_string(i1+i2);
        }
        else if(op==TokenType::MINU){
            return std::to_string(i1-i2);
        }
        else if(op==TokenType::MULT){
            return std::to_string(i1*i2);
        }
        else if(op==TokenType::DIV){
            return std::to_string(i1/i2);
        }
        else if(op==TokenType::MOD){
            return std::to_string(i1%i2);
        }
        else if(op==TokenType::LSS){
            return std::to_string(int(i1<i2));
        }
        else if(op==TokenType::GTR){
            return std::to_string(int(i1>i2));
        }
        else if(op==TokenType::LEQ){
            return std::to_string(int(i1<=i2));
        }
        else if(op==TokenType::GEQ){
            return std::to_string(int(i1>=i2));
        }
        else if(op==TokenType::EQL){
            return std::to_string(int(i1==i2));
        }
        else if(op==TokenType::NEQ){
            return std::to_string(int(i1!=i2));
        }
        else if(op==TokenType::AND){
            return std::to_string(int(i1&&i2));
        }
        else if(op==TokenType::OR){
            return std::to_string(int(i1||i2));
        }
        else if(op==TokenType::NOT){
            //第二个操作数不使用
            return std::to_string(int(!i1));
        }
        else{
            error();
        }
    }       
    else if(addtype==ir::Type::FloatLiteral){
        float f1,f2;
        f1 = std::stof(s1);
        f2 = std::stof(s2);
        if(op==TokenType::PLUS){
            return std::to_string(f1+f2);
        }
        else if(op==TokenType::MINU){
            return std::to_string(f1-f2);
        }
        else if(op==TokenType::MULT){
            return std::to_string(f1*f2);
        }
        else if(op==TokenType::DIV){
            return std::to_string(f1/f2);
        }
        else if(op==TokenType::MOD){
           error();
        }
        else if(op==TokenType::LSS){
            return std::to_string(int(f1<f2));
        }
        else if(op==TokenType::GTR){
            return std::to_string(int(f1>f2));
        }
        else if(op==TokenType::LEQ){
            return std::to_string(int(f1<=f2));
        }
        else if(op==TokenType::GEQ){
            return std::to_string(int(f1>=f2));
        }
        else if(op==TokenType::EQL){
            return std::to_string(int(f1==f2));
        }
        else if(op==TokenType::NEQ){
            return std::to_string(int(f1!=f2));
        }
        else if(op==TokenType::AND){
            return std::to_string(int(f1&&f2));
        }
        else if(op==TokenType::OR){
            return std::to_string(int(f1||f2));
        }
        else if(op==TokenType::NOT){
            //第二个操作数不使用
            return std::to_string(int(!f1));
        }
        else{
            error();
        }
    }
    else{
        error();
    }
    error();
    return "";
}

//字面量操作
ir::Operand frontend::Analyzer::perform_literal(Operand op1,Operand op2, frontend::TokenType op){
    if(!literal_check(op1.type) || !literal_check(op2.type)){
        error();
    } 
    sync_literalop_type(op1,op2);
    return {perform_string(op1.name,op2.name,op,op1.type),op1.type};
}

//变量操作
ir::Operand frontend::Analyzer::perform_var(Operand op1, Operand op2, frontend::TokenType op){
    if(literal_check(op1.type) || literal_check(op2.type)){
        error();
    }
    sync_varop_type(op1,op2);
    Operand tmp_op = {get_tmp_name(),op1.type};
    if(op==TokenType::PLUS){
        if(op1.type==ir::Type::Int){
            ADD_INST_ADD(add1,op1,op2,tmp_op);
        }
        else if(op1.type==ir::Type::Float) {
            ADD_INST_FADD(add2,op1,op2,tmp_op);
        }
        else{
            error();
        }
    }
    else if(op==TokenType::MINU){
        if(op1.type==ir::Type::Int){
            ADD_INST_SUB(add1,op1,op2,tmp_op);
        }
        else if(op1.type==ir::Type::Float) {
            ADD_INST_FSUB(add2,op1,op2,tmp_op);
        }
        else{
            error();
        }
    }
    else if(op==TokenType::MULT){
        if(op1.type==ir::Type::Int){
            ADD_INST_MUL(add1,op1,op2,tmp_op);
        }
        else if(op1.type==ir::Type::Float) {
            ADD_INST_FMUL(add2,op1,op2,tmp_op);
        }
        else{
            error();
        }
    }
    else if(op==TokenType::DIV){
        if(op1.type==ir::Type::Int){
            ADD_INST_DIV(add1,op1,op2,tmp_op);
        }
        else if(op1.type==ir::Type::Float) {
            ADD_INST_FDIV(add2,op1,op2,tmp_op);
        }
        else{
            error();
        }
    }
    else if(op==TokenType::MOD){
        if(op1.type==ir::Type::Int){
            ADD_INST_MOD(add1,op1,op2,tmp_op);
        }
        else if(op1.type==ir::Type::Float) {
            error();
        }
        else{
            error();
        }
    }
    else if(op==TokenType::LSS){
        //逻辑运算
        tmp_op.type = ir::Type::Int;
        if(op1.type==ir::Type::Int){
            ADD_INST_LSS(add1,op1,op2,tmp_op);
        }
        else if(op1.type==ir::Type::Float) {
            ADD_INST_FLSS(add1,op1,op2,tmp_op);
        }
        else{
            error();
        }
    }
    else if(op==TokenType::GTR){
        //逻辑运算
        tmp_op.type = ir::Type::Int;
        if(op1.type==ir::Type::Int){
            ADD_INST_GTR(add1,op1,op2,tmp_op);
        }
        else if(op1.type==ir::Type::Float) {
            ADD_INST_FGTR(add1,op1,op2,tmp_op);
        }
        else{
            error();
        }
    }
    else if(op==TokenType::LEQ){
        //逻辑运算
        tmp_op.type = ir::Type::Int;
        if(op1.type==ir::Type::Int){
            ADD_INST_LEQ(add1,op1,op2,tmp_op);
        }
        else if(op1.type==ir::Type::Float) {
            ADD_INST_FLEQ(add1,op1,op2,tmp_op);
        }
        else{
            error();
        }
    }
    else if(op==TokenType::GEQ){
        //逻辑运算
        tmp_op.type = ir::Type::Int;
        if(op1.type==ir::Type::Int){
            ADD_INST_GEQ(add1,op1,op2,tmp_op);
        }
        else if(op1.type==ir::Type::Float) {
            ADD_INST_FGEQ(add1,op1,op2,tmp_op);
        }
        else{
            error();
        }
    }
    else if(op==TokenType::EQL){
        //逻辑运算
        tmp_op.type = ir::Type::Int;
        if(op1.type==ir::Type::Int){
            ADD_INST_EQ(add1,op1,op2,tmp_op);
        }
        else if(op1.type==ir::Type::Float) {
            ADD_INST_FEQ(add1,op1,op2,tmp_op);
        }
        else{
            error();
        }
    }
    else if(op==TokenType::NEQ){
        //逻辑运算
        tmp_op.type = ir::Type::Int;
        if(op1.type==ir::Type::Int){
            ADD_INST_NEQ(add1,op1,op2,tmp_op);
        }
        else if(op1.type==ir::Type::Float) {
            ADD_INST_FNEQ(add1,op1,op2,tmp_op);
        }
        else{
            error();
        }
    }
    else if(op==TokenType::AND){
        //逻辑运算
        tmp_op.type = ir::Type::Int;
        if(op1.type==ir::Type::Int){
            ADD_INST__AND(add1,op1,op2,tmp_op);
        }
        else if(op1.type==ir::Type::Float) {
            warning_todo();
            ADD_INST__AND(add1,op1,op2,tmp_op);
        }
        else{
            error();
        }
    }
    else if(op==TokenType::OR){
        //逻辑运算
        tmp_op.type = ir::Type::Int;
        if(op1.type==ir::Type::Int){
            ADD_INST__OR(add1,op1,op2,tmp_op);
        }
        else if(op1.type==ir::Type::Float) {
            warning_todo();
            ADD_INST__OR(add1,op1,op2,tmp_op);
        }
        else{
            error();
        }
    }
    else if(op==TokenType::NOT){
        //逻辑运算,op2 不使用
        tmp_op.type = ir::Type::Int;
        if(op1.type==ir::Type::Int){
            ADD_INST__NOT(add1,op1,tmp_op);
        }
        else if(op1.type==ir::Type::Float) {
            warning_todo();
            ADD_INST__NOT(add1,op1,tmp_op);
        }
        else{
            error();
        }
    }
    else{
        error();
    }
    return tmp_op;
}



//获取默认Operand
ir::Operand frontend::Analyzer::get_default_opeand(ir::Type t){
    if(t==ir::Type::null){
        return {};
    }
    else if(t == Type::FloatLiteral){
        return {"0.0", Type::FloatLiteral};
    }
    else if(t == Type::IntLiteral){
        return {"0", Type::IntLiteral};
    }
    error();
}

ir::Operand frontend::Analyzer::op_to_var(Operand literal_op){
    if(!literal_check(literal_op.type)){
        //DONOTHING
        return literal_op;
    }
    Operand tmp_op; 
    if(int_check(literal_op.type)){
        tmp_op = Operand(get_tmp_name(),ir::Type::Int);
        ADD_INST_DEF(t,literal_op,tmp_op)
    }
    else if(float_check(literal_op.type)){
        tmp_op = Operand(get_tmp_name(),ir::Type::Float);
        ADD_INST_FDEF(t,literal_op,tmp_op)
    }
    else{
        error();
    }
    return tmp_op;
}

//变量对齐
ir::Operand frontend::Analyzer::sync_var_type(ir::Operand op, ir::Type to_type){
    if(literal_check(op.type)||literal_check(to_type)){
        error();
    }
    if(to_type== op.type){
        //DO NOTHING
        return op;
    }
    Operand tmp_op = Operand(get_tmp_name(),to_type);
    if(to_type==ir::Type::Float){
        ADD_INST_CVT_I2F(cvif,op,tmp_op);
    }
    else if(to_type==ir::Type::Int){
        ADD_INST_CVT_F2I(cvif,op,tmp_op);
    }
    else{
        error();
    }
    return tmp_op;
}

void frontend::Analyzer::sync_varop_type(ir::Operand& op1, ir::Operand& op2){
    if(op1.type !=op2.type){
        Operand tmp_op = Operand(get_tmp_name(),ir::Type::Float);
        if(float_check(op1.type)){
            op2 = sync_var_type(op2,ir::Type::Float);
        }
        else if(float_check(op2.type)){
            op1 = sync_var_type(op1,ir::Type::Float);
        }
        else{
            error();
        }
    }
    else{
        //DoNOTHING
    }
}


//进入新作用域时, 向符号表中添加 ScopeInfo, 相当于压栈
void frontend::SymbolTable::add_scope(Block* node) {
    if(scope_stack.empty()){    
        this->scope_stack.push_back({0});
    }
    else{
        //获取top
        const frontend::ScopeInfo& top_scope_info = this->scope_stack.back();
        ScopeInfo new_scope_info = ScopeInfo(top_scope_info.cnt+1);
        this->scope_stack.push_back(new_scope_info);
    }
    return;
}

//退出时弹栈
void frontend::SymbolTable::exit_scope() {
    if(scope_stack.empty()){
        error();
    }
    scope_stack.pop_back();
}

//输入一个变量名, 返回其在当前作用域下重命名后IR operand的名字 (相当于加后缀)
string frontend::SymbolTable::get_scoped_name(string id) {
    //获取top cnt
    if(scope_stack.empty()){
        return id + "_";//global,加下划线区分临时变量
    }
    const int top_cnt = this->scope_stack.back().cnt;
    return id+"_"+std::to_string(top_cnt);
}

//输入一个变量名, 在符号表中寻找最近的同名变量, 返回对应的 Operand(注意，此 Operand 的 name 是重命名后的)
Operand frontend::SymbolTable::get_operand(string id)  {
    //遍历栈
    for(int i = this->scope_stack.size()-1;i>=0;i--){
        if((scope_stack[i]).table.count(id)!=0){
            return  (scope_stack[i]).table[id].operand;
        }
    }
    if(this->global_symbol.count(id)!=0){
        return global_symbol[id].operand;
    }
    error();
    return {};
}

//输入一个变量名, 在符号表中寻找最近的同名变量, 返回 STE
frontend::STE frontend::SymbolTable::get_ste(string id) {
    //遍历栈
    for(int i = this->scope_stack.size()-1;i>=0;i--){
        if((scope_stack[i]).table.count(id)!=0){
            frontend::map_str_ste tmp_map =  (scope_stack[i]).table;
            return tmp_map[id];
        }
    }
    if(this->global_symbol.count(id)!=0){
        return global_symbol[id];
    }
    error();
    return {};
}

//存入符号
void frontend::SymbolTable::add_ste(std::string id,STE ste){
    //判断是否属于global
    if(this->scope_stack.empty()){
        //global
        this->global_symbol[id] = ste;
    }
    else{
        this->scope_stack.back().table[id] = ste; 
    }
}

frontend::Analyzer::Analyzer():symbol_table() {
    //添加输入输出库函数
    symbol_table.functions["getint"] = new ir::Function("getint",ir::Type::Int);
    symbol_table.functions["putint"] = new ir::Function("putint",{{"a",ir::Type::Int}},ir::Type::null);
    symbol_table.functions["getch"] = new ir::Function("getch",ir::Type::Int);
    symbol_table.functions["putch"] = new ir::Function("putch",{{"a",ir::Type::Int}},ir::Type::null);
    symbol_table.functions["getfloat"] = new ir::Function("getfloat",ir::Type::Float);
    symbol_table.functions["putfloat"] = new ir::Function("putfloat",{{"a",ir::Type::Float}},ir::Type::null);
    symbol_table.functions["getarray"] = new ir::Function("getarray",ir::Type::IntPtr);
    symbol_table.functions["putarray"] = new ir::Function("putarray",{{"n",ir::Type::IntLiteral},{"a",ir::Type::IntPtr}},ir::Type::null);
    symbol_table.functions["getfarray"] =  new ir::Function("getfarray",ir::Type::FloatPtr);
    symbol_table.functions["putfarray"] = new ir::Function("putfarray",{{"n",ir::Type::IntLiteral},{"a",ir::Type::FloatPtr}},ir::Type::null);
}

ir::Program frontend::Analyzer::get_ir_program(CompUnit* root) {
    analyzeCompUnit(root);
    return this->anlyzed_p;
}

std::string frontend::Analyzer::get_tmp_name(){
    return "tmp" + std::to_string(this->tmp_cnt++);
}

int frontend::Analyzer::get_alloc_size(std::vector<int>& dim){
    if(dim.size()==0){
        return 0;
    }
    int ret = 1;
    for(int i = 0;i<dim.size();i++){
        if(dim[i]<=0){
            error();
        }
        ret*=dim[i];
    }
    return ret;
}

//数组运算,计算总偏移量
int frontend::Analyzer::get_offset(std::vector<int>& dim, std::vector<int>& index){
    if(dim.size()!=index.size()){error();}
    for(int i = 0;i<dim.size();i++){
        if(index[i]>=dim[i]){
            error();
        }
    }
    int off_per_dim = 1;
    int ret = index.back() * off_per_dim;
    for(int i = dim.size()-1;i>=1;i--){
        off_per_dim*=dim[i];
        ret+= index[i-1] * off_per_dim;
    }
    return ret;
}

ir::Operand frontend::Analyzer::int_to_literal(int x){
    return {std::to_string(x),ir::Type::IntLiteral};
}

Operand frontend::Analyzer::get_offset_op(std::vector<int>& dim,std::vector<Operand>& ind){
    if(dim.size()!=ind.size()){
        //ind小于dim时返回数组指针
        todo();
    }
    bool is_all_literal = true;
    for(int i = 0;i<ind.size();i++){
        is_all_literal = is_all_literal && literal_check(ind[i].type); 
    }
    vector<int> int_id;
    if(is_all_literal){
        for(int i = 0;i<ind.size();i++){
            int_id.push_back(intstring_to_int(ind[i].name));
        }
        return {std::to_string(get_offset(dim,int_id)),ir::Type::IntLiteral};
    }
    Operand off_op = {get_tmp_name(),ir::Type::Int};
    ADD_INST_DEF(init_off_op,ind.back(),off_op);
    int off_per_dim = 1;
    for(int i  = dim.size()-1;i>=1;i--){
        off_per_dim*= dim[i];
        Operand now_off = {get_tmp_name(),ir::Type::Int};
        ind[i-1] = sync_to_int(ind[i-1]);
        ADD_INST_MUL(mul_, op_to_var(ind[i-1]), op_to_var(int_to_literal(off_per_dim)),now_off);
        ADD_INST_ADD(ad_,off_op,now_off,off_op);
    }
    return off_op;
}

//支持IF-ELSE
int frontend::Analyzer::get_nowins_ind(){
    if(this->func==nullptr){
        return this->g_init_inst.size()-1;
    }
    else{
        return this->func->InstVec.size()-1;
    }
}

//1. CompUnit -> (Decl | FuncDef) [CompUnit]
def_analyze(CompUnit){
    ANALYZE_CHILD()
}

//2. Decl -> ConstDecl | VarDecl
def_analyze(Decl){
    ANALYZE_CHILD()
}

//3. ConstDecl -> 'const' BType ConstDef { ',' ConstDef } ';'
def_analyze(ConstDecl){
    BEGIN_PTR_INIT()
    b_ptr++;//跳过'const'
    parse_bptr(BType,btype);
    TokenType tt = analyzeBType(node_btype);
    parse_bptr(ConstDef,c);
    analyzeConstDef(node_c,tt);
    while(b_ptr<root->children.size() && cur_termtoken_is(TokenType::COMMA)){
        b_ptr++;//跳过','
        parse_bptr(ConstDef,c_sub);
        analyzeConstDef(node_c_sub,tt);
    }
}

//4. BType -> 'int' | 'float'
def_analyze_withret(BType,frontend::TokenType){
    BEGIN_PTR_INIT()
    parse_bptr(Term, t);
    return node_t->token.type;
}

//5. ConstDef -> Ident { '[' ConstExp ']' } '=' ConstInitVal
def_analyze_withparams(ConstDef,TokenType token_type){
    BEGIN_PTR_INIT()
    parse_bptr(Term,ident);
    frontend::STE def_ste;
    //确定operand.name
    def_ste.operand.name = this->symbol_table.get_scoped_name(node_ident->token.value);
    //数组,和VARDEF 一致
    while(b_ptr<root->children.size() && cur_termtoken_is(TokenType::LBRACK)){
        //确定alloc_size和dimension
        b_ptr++;//跳过'['
        parse_bptr(ConstExp,ce);
        analyzeConstExp(node_ce);
        b_ptr++;//跳过']'
        //必须计算出值
        if(!node_ce->is_computable || !literal_check(node_ce->op.type)){
            error();
        }
        Operand dim_op = node_ce->op;
        dim_op = sync_literal_type(dim_op,ir::Type::IntLiteral);
        int dim = intstring_to_int(dim_op.name);
        if(dim<=0){
            error();
        }
        def_ste.dimension.push_back(dim);
    }
    if(b_ptr>root->children.size()){
        error();
    }
    //初始化变量
    if(def_ste.dimension.empty()){
        if(token_type==TokenType::INTTK){
            def_ste.operand.type = ir::Type::Int;
            ADD_INST_DEF(t1,get_default_opeand(ir::Type::IntLiteral),def_ste.operand)
        }
        else if(token_type==TokenType::FLOATTK){
            def_ste.operand.type = ir::Type::Float;
            ADD_INST_FDEF(t1,get_default_opeand(ir::Type::FloatLiteral),def_ste.operand)
        }
        else{
            error();
        }
    }
    else{
        //声明数组，alloc
        Operand sz = {std::to_string(get_alloc_size(def_ste.dimension)),ir::Type::IntLiteral};
        if(token_type==TokenType::INTTK){
            def_ste.operand.type = ir::Type::IntPtr;
        }
        else if(token_type==TokenType::FLOATTK){
            def_ste.operand.type = ir::Type::FloatPtr;
        }
        else{
            error();
        }
        ADD_INST_ALLOC(al,sz,def_ste.operand);
    }
    if(def_ste.dimension.empty()){
        //值在ConstInitVal添加
        def_ste.is_const = true;
    }
    else{
        //DONOTHING
        //数组声明const 无效
    }
    //添加
    if(b_ptr>=root->children.size()){
        error();
    }
    //def和initVal分离
    //赋值语句.
    b_ptr++;//跳过'='
    parse_bptr(ConstInitVal,const_initval);
    analyzeConstInitVal(node_const_initval,def_ste);
    //将符号存入字典中
    this->symbol_table.add_ste(node_ident->token.value,def_ste);
}

//6. ConstInitVal -> ConstExp | '{' [ ConstInitVal { ',' ConstInitVal } ] '}'
def_analyze_withparams(ConstInitVal,frontend::STE& ste){
    BEGIN_PTR_INIT()
    if(cur_termtoken_is(TokenType::LBRACE)){
        if(ste.dimension.empty()){
            error();
        }
        if(root->parent->type==NodeType::CONSTINITVAL){
            //嵌套{{{}}}
            todo();
        }
        //只考虑单行嵌套
        b_ptr++;//跳过'{'
        if(cur_node_is(NodeType::CONSTINITVAL)){
            parse_bptr(ConstInitVal,civ_);
            analyzeConstInitVal(node_civ_,ste);
        }
        while(b_ptr<root->children.size() && cur_termtoken_is(TokenType::COMMA)){
            b_ptr++;//跳过','
            parse_bptr(ConstInitVal,civ);
            analyzeConstInitVal(node_civ,ste);
        }
        //补齐vector;
        while(root->arr_init_ops.size()!=get_alloc_size(ste.dimension)){
            if(ste.operand.type==ir::Type::FloatPtr){
                root->arr_init_ops.push_back(get_default_opeand(ir::Type::FloatLiteral));
            }
            else if(ste.operand.type==ir::Type::IntPtr){
                root->arr_init_ops.push_back(get_default_opeand(ir::Type::IntLiteral));
            }
            else{
                error();
            }
        }
        //添加store指令
        for(int i = 0;i<root->arr_init_ops.size();i++){
            Operand off_op = {std::to_string(i),ir::Type::IntLiteral};
            if(literal_check(root->arr_init_ops[i].type)){
                root->arr_init_ops[i] = op_to_var(root->arr_init_ops[i]);
            }
            ADD_INST_STORE(store_ins,ste.operand,off_op,root->arr_init_ops[i]);
        }
    }
    else if(cur_node_is(NodeType::CONSTEXP)){
        parse_bptr(ConstExp,e);
        analyzeConstExp(node_e);
        if(ste.dimension.empty()){
            if(!node_e->is_computable){
                error();
            }
            if(!ste.is_const){
                error();
            }
            if(int_check(node_e->op.type)){
                ADD_INST_MOV(t,node_e->op,ste.operand)
            }
            else if(float_check(node_e->op.type)){
                ADD_INST_FMOV(t,node_e->op,ste.operand)
            }
            else{
                error();
            }
            //给ste添加值
            ste.constVal.name = node_e->op.name;
            ste.constVal.type = node_e->op.type;
        }
        else if(!ste.dimension.empty()){
            //必须从CONSTINITVal走下来
            if(root->parent->type!=NodeType::CONSTINITVAL){
                error();
            }
            //类型对齐
            Operand ep_op = node_e->op;
            if(ep_op.type==ir::Type::null){
                error();
            }
            //同步类型
            if(int_check(ep_op.type) && ste.operand.type==ir::Type::FloatPtr){
                if(literal_check(ep_op.type)){
                    ep_op = sync_literal_type(ep_op,ir::Type::FloatLiteral);
                }
                else if(!literal_check(ep_op.type)){
                    ep_op = sync_var_type(ep_op,ir::Type::Float);
                }
                else{
                    error();
                }
            }
            else if(float_check(ep_op.type) && ste.operand.type == ir::Type::IntPtr){
                if(literal_check(ep_op.type)){
                    ep_op = sync_literal_type(ep_op,ir::Type::IntLiteral);
                }
                else if(!literal_check(ep_op.type)){
                    ep_op = sync_var_type(ep_op,ir::Type::Int);
                }
                else{
                    error();
                }
            }
            //添加到父亲结点的op list中
            ((ConstInitVal*)root->parent)->arr_init_ops.push_back(ep_op);
        }
        else{   
            error();
        }
    }
    else{
        error();
    }
}

//7. VarDecl -> BType VarDef { ',' VarDef } ';'
def_analyze(VarDecl){
    BEGIN_PTR_INIT()
    parse_bptr(BType,btype);
    TokenType tt = analyzeBType(node_btype);
    parse_bptr(VarDef,v);
    analyzeVarDef(node_v,tt);
    while(b_ptr<root->children.size() && cur_termtoken_is(TokenType::COMMA)){
        b_ptr++;//跳过','
        parse_bptr(VarDef,v_sub);
        analyzeVarDef(node_v_sub,tt);
    }
    
}

//8. VarDef -> Ident { '[' ConstExp ']' } [ '=' InitVal ]
def_analyze_withparams(VarDef,frontend::TokenType token_type){
    BEGIN_PTR_INIT()
    parse_bptr(Term,ident);
    frontend::STE def_ste;
    //确定oprand.name
    def_ste.is_const = false;
    def_ste.operand.name = this->symbol_table.get_scoped_name(node_ident->token.value);
    while(b_ptr<root->children.size() && cur_termtoken_is(TokenType::LBRACK)){
        //确定alloc_size和dimension
        b_ptr++;//跳过'['
        parse_bptr(ConstExp,ce);
        analyzeConstExp(node_ce);
        b_ptr++;//跳过']'
        //必须计算出值
        if(!node_ce->is_computable || !literal_check(node_ce->op.type)){
            error();
        }
        Operand dim_op = node_ce->op;
        dim_op = sync_literal_type(dim_op,ir::Type::IntLiteral);
        int dim = intstring_to_int(dim_op.name);
        if(dim<=0){
            error();
        }
        def_ste.dimension.push_back(dim);
    }
    if(b_ptr>root->children.size()){
        error();
    }
    //初始化变量
    if(def_ste.dimension.empty()){
        if(token_type==TokenType::INTTK){
            def_ste.operand.type = ir::Type::Int;
            ADD_INST_DEF(t1,get_default_opeand(ir::Type::IntLiteral),def_ste.operand)
        }
        else if(token_type==TokenType::FLOATTK){
            def_ste.operand.type = ir::Type::Float;
            ADD_INST_FDEF(t1,get_default_opeand(ir::Type::FloatLiteral),def_ste.operand)
        }
        else{
            error();
        }
    }
    else{
        //声明数组，alloc
        Operand sz = {std::to_string(get_alloc_size(def_ste.dimension)),ir::Type::IntLiteral};
        if(token_type==TokenType::INTTK){
            def_ste.operand.type = ir::Type::IntPtr;
        }
        else if(token_type==TokenType::FLOATTK){
            def_ste.operand.type = ir::Type::FloatPtr;
        }
        else{
            error();
        }
        ADD_INST_ALLOC(al,sz,def_ste.operand);
    }
    //def和initVal分离
    //赋值语句.
    if(b_ptr<root->children.size() && cur_termtoken_is(TokenType::ASSIGN)){
        b_ptr++;//跳过'='
        parse_bptr(InitVal,initval);
        analyzeInitVal(node_initval,def_ste);
    }
    //将符号存入字典中
    this->symbol_table.add_ste(node_ident->token.value,def_ste);
}

//9. InitVal -> Exp | '{' [ InitVal { ',' InitVal } ] '}'
def_analyze_withparams(InitVal,frontend::STE& ste){
    BEGIN_PTR_INIT()
    if(cur_termtoken_is(TokenType::LBRACE)){
        if(ste.dimension.empty()){
            error();
        }
        if(root->parent->type==NodeType::INITVAL){
            //嵌套{{{}}}
            todo();
        }
        //只考虑单行嵌套
        b_ptr++;//跳过'{'
        if(cur_node_is(NodeType::INITVAL)){
            parse_bptr(InitVal,iv_);
            analyzeInitVal(node_iv_,ste);
        }
        while(b_ptr<root->children.size() && cur_termtoken_is(TokenType::COMMA)){
            b_ptr++;//跳过','
            parse_bptr(InitVal,iv);
            analyzeInitVal(node_iv,ste);
        }
        //补齐vector;
        while(root->arr_init_ops.size()!=get_alloc_size(ste.dimension)){
            if(ste.operand.type==ir::Type::FloatPtr){
                root->arr_init_ops.push_back(get_default_opeand(ir::Type::FloatLiteral));
            }
            else if(ste.operand.type==ir::Type::IntPtr){
                root->arr_init_ops.push_back(get_default_opeand(ir::Type::IntLiteral));
            }
            else{
                error();
            }
        }
        //添加store指令
        for(int i = 0;i<root->arr_init_ops.size();i++){
            Operand off_op = {std::to_string(i),ir::Type::IntLiteral};
            if(literal_check(root->arr_init_ops[i].type)){
                root->arr_init_ops[i] = op_to_var(root->arr_init_ops[i]);
            }
            ADD_INST_STORE(store_ins,ste.operand,off_op,root->arr_init_ops[i]);
        }
    }
    else if(cur_node_is(NodeType::EXP)){
        parse_bptr(Exp,e);
        analyzeExp(node_e);
        if(ste.dimension.empty()){
            if(int_check(node_e->op.type)){
                ADD_INST_MOV(t,node_e->op,ste.operand)
            }
            else if(float_check(node_e->op.type)){
                ADD_INST_FMOV(t,node_e->op,ste.operand)
            }
            else{
                error();
            }
        }
        else if(!ste.dimension.empty()){
            //必须从InitVal走下来
            if(root->parent->type!=NodeType::INITVAL){
                error();
            }
            //类型对齐
            Operand ep_op = node_e->op;
            if(ep_op.type==ir::Type::null){
                error();
            }
            //同步类型
            if(int_check(ep_op.type) && ste.operand.type==ir::Type::FloatPtr){
                if(literal_check(ep_op.type)){
                    ep_op = sync_literal_type(ep_op,ir::Type::FloatLiteral);
                }
                else if(!literal_check(ep_op.type)){
                    ep_op = sync_var_type(ep_op,ir::Type::Float);
                }
                else{
                    error();
                }
            }
            else if(float_check(ep_op.type) && ste.operand.type == ir::Type::IntPtr){
                if(literal_check(ep_op.type)){
                    ep_op = sync_literal_type(ep_op,ir::Type::IntLiteral);
                }
                else if(!literal_check(ep_op.type)){
                    ep_op = sync_var_type(ep_op,ir::Type::Int);
                }
                else{
                    error();
                }
            }
            //添加到父亲结点的op list中
            ((InitVal*)root->parent)->arr_init_ops.push_back(ep_op);
        }
        else{   
            error();
        }
    }
    else{
        error();
    }
}

//10. FuncDef -> FuncType Ident '(' [FuncFParams] ')' Block
def_analyze(FuncDef){
    //parse_node
    BEGIN_PTR_INIT()
    parse_bptr(FuncType,functype);
    parse_bptr(Term,funcname);
    b_ptr++;//跳过'('
    FuncFParams* node_funcfparams = nullptr;
    if(cur_node_is(NodeType::FUNCFPARAMS)){
        parse_bptr_declared(FuncFParams,funcfparams);
        analyzeFuncFParams(node_funcfparams);
    }
    b_ptr++;//跳过')'
    parse_bptr(Block,block);
    
    //生成函数定义IR
    new_func();
    ir::Type func_type = analyzeFuncType(node_functype);
    func->returnType = func_type;
    func->name = node_funcname->token.value;
    //存入符号表
    this->symbol_table.functions[func->name] = func;
    //main函数需要执行先加载global
    if(func->name=="main"){
        //添加global Func.
        Function* global = new Function();
        for(auto ins: g_init_inst){
            global->addInst(ins);
        }
        global->name = "global";
        global->returnType = ir::Type::null;
        global->addInst(new Instruction({},{},{},ir::Operator::_return));
        this->anlyzed_p.addFunction(*global);
        ir::CallInst* callins =  new ir::CallInst(ir::Operand("global",ir::Type::null),{});
        func->addInst((Instruction*)callins);
    }
    /**分析函数Block*/
    //生成符号表
    this->symbol_table.add_scope(node_block);
    //生成函数参数
    if(node_funcfparams!=nullptr){
        //添加函数参数中
        for(auto se:node_funcfparams->param_stes){
            std::string ident_name = se.operand.name;
            se.operand.name = this->symbol_table.get_scoped_name(se.operand.name);
            this->func->ParameterList.push_back(se.operand);
            this->symbol_table.add_ste(ident_name,se);
        }
    }
    analyzeBlock(node_block);
    //检测main函数是否有返回值
    if(func->name=="main"){
        if(func->InstVec.empty() || func->InstVec.back()->op!=Operator::_return){
            //添加返回0
            ADD_INST_RETURN(t,get_default_opeand(ir::Type::IntLiteral))
        }
    }
    //将Function添加到program中
    add_func();
}

//11. FuncType -> 'void' | 'int' | 'float'
def_analyze_withret(FuncType,ir::Type){
    if(root->children.size()!=1){
        error();
    }
    Term* child = (Term*)root->children[0];
    if(child->token.type==TokenType::VOIDTK){
        return ir::Type::null;
    }
    else if(child->token.type==TokenType::INTTK){
        return ir::Type::Int;
    }
    else if(child->token.type==TokenType::FLOATTK){
        return ir::Type::Float;
    }
    else{
        error();
    }
    error();
    return {};
}


//12. FuncFParam -> BType Ident ['[' ']' { '[' Exp ']' }]
def_analyze(FuncFParam){
    BEGIN_PTR_INIT()
    root->param.is_const = false;
    parse_bptr(BType,bt);
    parse_bptr(Term,ident);
    if(b_ptr<root->children.size() && cur_termtoken_is(TokenType::LBRACK)){
        //数组
        todo();
    }
    else{
        //变量
        TokenType tt =  analyzeBType(node_bt);
        std::string name = node_ident->token.value;
        if(tt==TokenType::INTTK){
            root->param.operand = {name,ir::Type::Int};
        }
        else if(tt==TokenType::FLOATTK) {
            root->param.operand = {name,ir::Type::Float};
        }
        else{
            error();
        }
    }
}

//13. FuncFParams -> FuncFParam { ',' FuncFParam }
def_analyze(FuncFParams){
    BEGIN_PTR_INIT()
    parse_bptr(FuncFParam,f_);
    analyzeFuncFParam(node_f_);
    root->param_stes.push_back(node_f_->param);
    while(b_ptr<root->children.size() && cur_termtoken_is(TokenType::COMMA)){
        b_ptr++;//跳过','
        parse_bptr(FuncFParam,f);
        analyzeFuncFParam(node_f);
        root->param_stes.push_back(node_f->param);
    }
}

//14. Block -> '{' { BlockItem } '}'
def_analyze(Block){
    //创建符号表 
    ASSERT_NULLFUNC()
    BEGIN_PTR_INIT()  
    b_ptr++;//忽略'{'
    while(cur_node_is(NodeType::BLOCKITEM)){
        parse_bptr(BlockItem,blockitem);
        analyzeBlockItem(node_blockitem);
    }
    //符号表出栈
    this->symbol_table.exit_scope();
}

//15. BlockItem -> Decl | Stmt
def_analyze(BlockItem){
    ASSERT_NULLFUNC()
    ANALYZE_CHILD()
}

//16. Stmt -> LVal '=' Exp ';' | Block | 'if' '(' Cond ')' Stmt [ 'else' Stmt ] | 'while' '(' Cond ')' Stmt | 'break' ';' | 'continue' ';' | 'return' [Exp] ';' | [Exp] ';'
def_analyze_withparams(Stmt,std::vector<gotoInst>* pa_go_ins){
    //FIRST,根据子节点类型分情况讨论
    BEGIN_PTR_INIT()
    ASSERT_NULLFUNC()
    if(cur_termtoken_is(TokenType::RETURNTK)){
        //'return' [Exp] ';'
        ASSERT_WRONGRET()
        //parse
        b_ptr++;//跳过return
        Exp* node_exp = nullptr;
        if(cur_node_is(NodeType::EXP)){
            parse_bptr_declared(Exp,exp);
        }
        //处理return 语句
        ir::Instruction* ret_ins = new Instruction();
        ret_ins->op = ir::Operator::_return;
        if(node_exp==nullptr){
            if(this->func->returnType!=ir::Type::null){
                //自动填充值0
                ret_ins->op1.name = "0";
                ret_ins->op1.type = var_to_literal(this->func->returnType);
            }
        }
        else{
            ASSERT_WRONGRET_WITHOUTNULL()
            analyzeExp(node_exp);
            if(node_exp->is_computable && literal_check(node_exp->op.type)){
                node_exp->op =  sync_literal_type(node_exp->op,var_to_literal(func->returnType));
            }
            else if(!node_exp->is_computable && !literal_check(node_exp->op.type)){
                //同步变量类型
                node_exp->op =  sync_var_type(node_exp->op,func->returnType);
            }
            else{
                error();
            }
            ret_ins->op1 = node_exp->op;
        }
        func->addInst(ret_ins);
    }
    else if(cur_node_is(NodeType::LVAL)){
        //LVal '=' Exp ';'
        parse_bptr(LVal,lv);
        b_ptr++;//跳过 '='
        parse_bptr(Exp,e);
        analyzeLVal(node_lv);
        analyzeExp(node_e);
        if(node_lv->is_computable){
            error();
        }
        if(!ptr_check(node_lv->op.type) && node_lv->ind.empty()){
            //变量赋值
            if(literal_check(node_e->op.type)){
                node_e->op = sync_literal_type(node_e->op,var_to_literal(node_lv->op.type));
            }
            else{
                node_e->op = sync_var_type(node_e->op,node_lv->op.type);
            }
            if(int_check(node_lv->op.type)){
                ADD_INST_MOV(t,node_e->op,node_lv->op);
            }
            else if(float_check(node_lv->op.type)){
                ADD_INST_FMOV(t,node_e->op,node_lv->op);
            }
            else{
                error();
            }
        }
        else if(!(node_lv->ind.empty())){
            //数组赋值
            if(literal_check(node_e->op.type)){
                node_e->op = op_to_var(node_e->op);
            }
            node_e->op = sync_var_type(node_e->op,ptr_to_var(node_lv->arr_ste.operand.type));
            ADD_INST_STORE(st,node_lv->arr_ste.operand,node_lv->off_op,node_e->op);
        }
        else{
            error();
        }
    }
    else if(cur_node_is(NodeType::BLOCK)){
        parse_bptr(Block,b);
        this->symbol_table.add_scope(node_b);
        analyzeBlock(node_b);
    }
    else if(cur_termtoken_is(TokenType::IFTK)){
        // 'if' '(' Cond ')' Stmt [ 'else' Stmt ]
        //处理IF
        //parse
        b_ptr+=2;//跳过 if,'('
        parse_bptr(Cond,cd);
        //跳过')'
        int begin_if_id = -1;
        int begin_else_id = -1;
        int begin_final_id = -1;
        analyzeCond(node_cd);
        begin_if_id = get_nowins_ind()+1;
        b_ptr++;//跳过')'
        parse_bptr(Stmt,ifst);
        analyzeStmt(node_ifst,pa_go_ins);
        //if执行完去final
        ADD_INST_GOTO(if_go_final,get_default_opeand(ir::Type::null),get_default_opeand(ir::Type::null));
        int if_go_final_id = get_nowins_ind();
        begin_else_id = get_nowins_ind()+1;
        if(b_ptr<root->children.size() && cur_termtoken_is(TokenType::ELSETK)){
            b_ptr++;//跳过else;
            parse_bptr(Stmt,elsest);
            analyzeStmt(node_elsest,pa_go_ins);
        }
        begin_final_id = get_nowins_ind()+1;
        //对goto语句进行位置偏移计算
        for(auto g:node_cd->go_ins){
            if(g.go_type==GoToType::AND){
                g.goto_ins->des = int_to_literal(begin_else_id - g.ins_index);
            }
            else if(g.go_type==GoToType::OR){
                g.goto_ins->des = int_to_literal(begin_if_id - g.ins_index);
            }
            else{
                error();
            }
        }
        if_go_final->des = int_to_literal(begin_final_id - if_go_final_id);
    }
    else if(cur_node_is(NodeType::EXP)){
        parse_bptr(Exp,e);
        analyzeExp(node_e);
    }
    else if(cur_termtoken_is(TokenType::SEMICN)){
        //DONOTHING;
    }
    else if(cur_termtoken_is(TokenType::BREAKTK)){
        todo();
    }
    else if(cur_termtoken_is(TokenType::WHILETK)){
        // 'while' '(' Cond ')' Stmt
        b_ptr+=2;//跳过while '('
        parse_bptr(Cond,cd);
        b_ptr++;//跳过')'
        parse_bptr(Stmt,while_st);
        int begin_cd_id = get_nowins_ind()+1;
        int begin_while_id = -1;
        int begin_final_id = -1;
        analyzeCond(node_cd);
        begin_while_id = get_nowins_ind()+1;
        analyzeStmt(node_while_st,&(root->go_ins));
        //Stmt执行完去begin_cd_id
        ADD_INST_GOTO(while_go_begin,get_default_opeand(ir::Type::null),get_default_opeand(ir::Type::null));
        int while_go_begin_id = get_nowins_ind();
        begin_final_id = get_nowins_ind()+1;
        //对cond 进行偏移计算
        for(auto g:node_cd->go_ins){
            if(g.go_type==GoToType::AND){
                g.goto_ins->des = int_to_literal(begin_final_id - g.ins_index);
            }
            else if(g.go_type==GoToType::OR){
                g.goto_ins->des = int_to_literal(begin_while_id - g.ins_index);
            }
            else{
                error();
            }
        }
        while_go_begin->des = int_to_literal(begin_cd_id- while_go_begin_id);
        //对BREAK 和 CONTINUE进行计算
        if(!root->go_ins.empty()){
            todo();
        }
    }
    else if(cur_termtoken_is(TokenType::CONTINUETK)){
        todo();
    }
    else{
        error();
    }
}

//17. Exp -> AddExp
def_analyze(Exp){
    BEGIN_PTR_INIT()
    parse_bptr(AddExp,addexp);
    analyzeAddExp(node_addexp);
    root_exp_assign(node_addexp);
}

//18. Cond -> LOrExp
def_analyze(Cond){
    BEGIN_PTR_INIT()
    parse_bptr(LOrExp,_);
    analyzeLOrExp(node__,root);
    root_exp_assign(node__);
}

//19. LVal -> Ident {'[' Exp ']'}
def_analyze(LVal){
    BEGIN_PTR_INIT()
    parse_bptr(Term,ident);
    root->is_computable = false;
    //从符号表中找到STE和OP
    STE indent_ste = this->symbol_table.get_ste(node_ident->token.value);
    Operand ident_op = this->symbol_table.get_operand(node_ident->token.value);
    if(ptr_check(ident_op.type)){
        while(b_ptr<root->children.size() && cur_termtoken_is(TokenType::LBRACK)){
            //数组寻值,临时变量,计算偏移
            b_ptr++;//跳过'['
            parse_bptr(Exp,ep);
            b_ptr++;//跳过']'
            analyzeExp(node_ep);
            node_ep->op =  sync_to_int(node_ep->op);
            root->ind.push_back(node_ep->op);
        }
        root->off_op = get_offset_op(indent_ste.dimension,root->ind);
        Operand tmp_op = {get_tmp_name(),ptr_to_var(ident_op.type)};
        root->op = tmp_op;
        root->arr_ste = indent_ste;
    }
    else{
        if(indent_ste.is_const==true){
            //CONST
            root->is_computable = true;
            root->op = indent_ste.constVal;
        }
        else{
            if(b_ptr!=root->children.size()){
                error();
            }
            root->op = ident_op;
        }
    }
}

//20. Number -> IntConst | floatConst
def_analyze(Number){
    BEGIN_PTR_INIT()
    parse_bptr(Term,t);
    root->is_computable = true;
    if(node_t->token.type==TokenType::INTLTR){
        root->op =  ir::Operand(node_t->token.value,ir::Type::IntLiteral);
    }   
    else if(node_t->token.type==TokenType::FLOATLTR){
        root->op =  ir::Operand(node_t->token.value,ir::Type::FloatLiteral);
    }
    else{
        error();
    }
}

//21. PrimaryExp -> '(' Exp ')' | LVal | Number
def_analyze(PrimaryExp){
    //parse,First
    BEGIN_PTR_INIT()
    if(cur_node_is(NodeType::TERMINAL)){
        //'(' Exp ')' 
        b_ptr++;
        parse_bptr(Exp,e);
        analyzeExp(node_e);
        root_exp_assign(node_e);
    }
    else if(cur_node_is(NodeType::LVAL)){
        //LVal
        parse_bptr(LVal,lv);
        analyzeLVal(node_lv);
        if(!node_lv->ind.empty()){
            //添加load,创建临时变量
            ADD_INST_LOAD(load,node_lv->arr_ste.operand,node_lv->off_op,node_lv->op);
        }
        root_exp_assign(node_lv);
    }
    else if(cur_node_is(NodeType::NUMBER)){
        //Number
        parse_bptr(Number,n);
        analyzeNumber(node_n);
        root_exp_assign(node_n);
    }
    else{
        error();
    }
}

//22. UnaryExp -> PrimaryExp | Ident '(' [FuncRParams] ')' | UnaryOp UnaryExp
def_analyze(UnaryExp){
    //parse,FIRST
    BEGIN_PTR_INIT()
    if(cur_node_is(NodeType::PRIMARYEXP)){
        //PrimaryExp
        parse_bptr(PrimaryExp,p);
        analyzePrimaryExp(node_p);
        root_exp_assign(node_p);
    }
    else if(cur_node_is(NodeType::TERMINAL)){
        //Ident '(' [FuncRParams] ')'
        //parse
        parse_bptr(Term,func_name);
        b_ptr++;//跳过'('
        Function* func_tmp = this->symbol_table.functions[node_func_name->token.value];
        Operand func_op = {node_func_name->token.value,func_tmp->returnType};
        Operand ret_op = {get_tmp_name(),func_tmp->returnType};
        ir::CallInst* call_ins = new ir::CallInst(func_op,ret_op);
        std::vector<Operand>&paraVec = call_ins->argumentList;
        if(b_ptr<root->children.size() && cur_node_is(NodeType::FUNCRPARAMS)){
            parse_bptr(FuncRParams,fr);
            analyzeFuncRParams(node_fr);
            //添加到paraVec中
            for(auto op:node_fr->params){
                paraVec.push_back(op);
            }
        }
        root->op = ret_op;
        root->is_computable = false;
        ADD_INST(call_ins);
    }
    else if(cur_node_is(NodeType::UNARYOP)){
        //UnaryOp UnaryExp
        parse_bptr(UnaryOp,uop);
        parse_bptr(UnaryExp,ue);
        analyzeUnaryOp(node_uop);
        analyzeUnaryExp(node_ue);
        root->is_computable = node_ue->is_computable;
        if(node_uop->op==TokenType::MINU){
            Operand tmp_op = {"-1",ir::Type::IntLiteral};
            if(root->is_computable){ 
                root->op = perform_literal(node_ue->op,tmp_op,TokenType::MULT);
            } 
            else{ 
                tmp_op =  op_to_var(tmp_op); 
                node_ue->op = op_to_var(node_ue->op); 
                root->op = perform_var(tmp_op,node_ue->op,TokenType::MULT);
            }
        }
        else if(node_uop->op==TokenType::NOT){
           if(root->is_computable){
                root->op = perform_literal(node_ue->op,get_default_opeand(ir::Type::IntLiteral),TokenType::NOT);
           }
           else{
                Operand tmp_op =  op_to_var(get_default_opeand(ir::Type::IntLiteral)); 
                node_ue->op = op_to_var(node_ue->op); 
                root->op = perform_var(node_ue->op,tmp_op,TokenType::NOT);
           }
        }
        else if(node_uop->op==TokenType::PLUS){
            root->op = node_ue->op;
        }
        else{
            error(); 
        }
    }
    else{
        error();
    }
}

//23. UnaryOp -> '+' | '-' | '!'
def_analyze(UnaryOp){
    BEGIN_PTR_INIT()
    parse_bptr(Term,uop);
    root->op = node_uop->token.type;
}

//24. FuncRParams -> Exp { ',' Exp }
def_analyze(FuncRParams){
    BEGIN_PTR_INIT()
    parse_bptr(Exp,e_);
    analyzeExp(node_e_);
    root->params.push_back(node_e_->op);
    while(b_ptr<root->children.size() && cur_termtoken_is(TokenType::COMMA)){
        b_ptr++;
        parse_bptr(Exp,e);
        analyzeExp(node_e);
        root->params.push_back(node_e->op);
    }
}

//25. MulExp -> UnaryExp { ('*' | '/' | '%') UnaryExp }
def_analyze(MulExp){
    //parse
    BEGIN_PTR_INIT()
    parse_bptr(UnaryExp,unaryexp_);
    analyzeUnaryExp(node_unaryexp_);
    root_exp_assign(node_unaryexp_);
    while(b_ptr<root->children.size()){
        parse_bptr(Term,mulop);
        if( (node_mulop->token.type!=TokenType::MULT) && (node_mulop->token.type!=TokenType::DIV) && (node_mulop->token.type!=TokenType::MOD)){
            error();
        }
        parse_bptr(UnaryExp,ue_sub);
        analyzeUnaryExp(node_ue_sub);
        root->is_computable = root->is_computable && node_ue_sub->is_computable;
        PERFROM_OP(node_ue_sub,node_mulop)
    }
}

//26. AddExp -> MulExp { ('+' | '-') MulExp }
def_analyze(AddExp){
    //parse
    BEGIN_PTR_INIT()
    parse_bptr(MulExp,mulexp_);
    analyzeMulExp(node_mulexp_);
    root_exp_assign(node_mulexp_);
    while(b_ptr<root->children.size()){
        parse_bptr(Term,addop);
        if( (node_addop->token.type!=TokenType::PLUS) && (node_addop->token.type!=TokenType::MINU)){
            error();
        }
        parse_bptr(MulExp,mulexp_sub);
        analyzeMulExp(node_mulexp_sub);
        PERFROM_OP(node_mulexp_sub,node_addop)
    }
}

//27. RelExp -> AddExp { ('<' | '>' | '<=' | '>=') AddExp }
def_analyze(RelExp){
    BEGIN_PTR_INIT()
    parse_bptr(AddExp,ad);
    analyzeAddExp(node_ad);
    root_exp_assign(node_ad);
    while(b_ptr<root->children.size()){
        parse_bptr(Term,rlop);
        if((node_rlop->token.type!=TokenType::LSS) && (node_rlop->token.type!=TokenType::LEQ) && (node_rlop->token.type!=TokenType::GTR) && (node_rlop->token.type!=TokenType::GEQ)){
            error();
        }
        parse_bptr(AddExp,ad_1);
        analyzeAddExp(node_ad_1);
        PERFROM_OP(node_ad_1,node_rlop);
    }
    root->op = sync_to_int(root->op);
}

//28. EqExp -> RelExp { ('==' | '!=') RelExp }
def_analyze(EqExp){
    BEGIN_PTR_INIT()
    parse_bptr(RelExp,rl);
    analyzeRelExp(node_rl);
    root_exp_assign(node_rl);
    while(b_ptr<root->children.size()){
        parse_bptr(Term,eqop);
        if( (node_eqop->token.type!=TokenType::EQL) && (node_eqop->token.type!=TokenType::NEQ)){
            error();
        }
        parse_bptr(RelExp,rl_1);
        analyzeRelExp(node_rl_1);
        PERFROM_OP(node_rl_1,node_eqop)
    }
    root->op = sync_to_int(root->op);
}

//29. LAndExp -> EqExp [ '&&' LAndExp ]
def_analyze_withparams(LAndExp,frontend::Cond* cond_father){
    BEGIN_PTR_INIT()
    parse_bptr(EqExp,eq);
    analyzeEqExp(node_eq);
    root_exp_assign(node_eq);
    //生成GOTO
    root->op = sync_to_int(root->op);
    Operand not_op = {get_tmp_name(),ir::Type::Int};
    ADD_INST__NOT(no_ins,node_eq->op,not_op)
    ADD_INST_GOTO(goto_ins,not_op,get_default_opeand(ir::Type::null))
    cond_father->go_ins.push_back({goto_ins,get_nowins_ind(),GoToType::AND});
    while(b_ptr<root->children.size() && cur_termtoken_is(TokenType::AND)){
        parse_bptr(Term,andop);
        parse_bptr(LAndExp,_);
        analyzeLAndExp(node__,cond_father);
        PERFROM_OP(node__,node_andop);
    }
}

//30. LOrExp -> LAndExp [ '||' LOrExp ]
//结尾的事情交给父亲结点处理
def_analyze_withparams(LOrExp,frontend::Cond* cond_father){
    BEGIN_PTR_INIT()
    parse_bptr(LAndExp,la);
    analyzeLAndExp(node_la,cond_father);
    root_exp_assign(node_la);
    //生成GOTO
    root->op = sync_to_int(root->op);//必须是int
    ADD_INST_GOTO(goto_ins,op_to_var(root->op),get_default_opeand(ir::Type::null))//偏移量先留空
    cond_father->go_ins.push_back({goto_ins,get_nowins_ind(),GoToType::OR});
    while(b_ptr<root->children.size() && cur_termtoken_is(TokenType::OR)){
        parse_bptr(Term,orop);
        parse_bptr(LOrExp,_);
        analyzeLOrExp(node__,cond_father);
        PERFROM_OP(node__,node_orop)
    }
}

//31. ConstExp -> AddExp
def_analyze(ConstExp){
    BEGIN_PTR_INIT()
    parse_bptr(AddExp, ad);
    analyzeAddExp(node_ad);
    root_exp_assign(node_ad);
}

//根据AstNodeType 选择分析函数
def_analyze(AstNode){
    if(root==nullptr){
        error();
    }
    else if(root->type==NodeType::COMPUINT){
        analyzeCompUnit((CompUnit*)root);
    }
    else if(root->type==NodeType::DECL){
        analyzeDecl((Decl*)root);
    }
    else if(root->type==NodeType::FUNCDEF){
        analyzeFuncDef((FuncDef*)root);
    }
    else if(root->type==NodeType::CONSTDECL){
        analyzeConstDecl((ConstDecl*)root);
    }
    else if(root->type==NodeType::BTYPE){
        analyzeBType((BType*)root);
    }
    else if(root->type==NodeType::CONSTDEF){
        error();
    }
    else if(root->type==NodeType::CONSTINITVAL){
        error();
    }
    else if(root->type==NodeType::VARDECL){
        analyzeVarDecl((VarDecl*)root);
    }
    else if(root->type==NodeType::VARDEF){
        error();
    }
    else if(root->type==NodeType::INITVAL){
        error();
    }
    else if(root->type==NodeType::FUNCTYPE){
        analyzeFuncType((FuncType*)root);
    }
    else if(root->type==NodeType::FUNCFPARAM){
        analyzeFuncFParam((FuncFParam*)root);
    }
    else if(root->type==NodeType::FUNCFPARAMS){
        analyzeFuncFParams((FuncFParams*)root);
    }
    else if(root->type==NodeType::BLOCK){
        analyzeBlock((Block*)root);
    }
    else if(root->type==NodeType::BLOCKITEM){
        analyzeBlockItem((BlockItem*)root);
    }
    else if(root->type==NodeType::STMT){
        analyzeStmt((Stmt*)root, nullptr);
    }
    else if(root->type==NodeType::EXP){
        analyzeExp((Exp*)root);
    }
    else if(root->type==NodeType::COND){
        analyzeCond((Cond*)root);
    }
    else if(root->type==NodeType::LVAL){
        analyzeLVal((LVal*)root);
    }
    else if(root->type==NodeType::NUMBER){
        analyzeNumber((Number*)root);
    }
    else if(root->type==NodeType::PRIMARYEXP){
        analyzePrimaryExp((PrimaryExp*)root);
    }
    else if(root->type==NodeType::UNARYEXP){
        analyzeUnaryExp((UnaryExp*)root);
    }
    else if(root->type==NodeType::UNARYOP){
        analyzeUnaryOp((UnaryOp*)root);
    }
    else if(root->type==NodeType::FUNCRPARAMS){
        analyzeFuncRParams((FuncRParams*)root);
    }
    else if(root->type==NodeType::MULEXP){
        analyzeMulExp((MulExp*)root);
    }
    else if(root->type==NodeType::ADDEXP){
        analyzeAddExp((AddExp*)root);
    }
    else if(root->type==NodeType::RELEXP){
        analyzeRelExp((RelExp*)root);
    }
    else if(root->type==NodeType::EQEXP){
        analyzeEqExp((EqExp*)root);
    }
    else if(root->type==NodeType::LANDEXP){
        error();
    }
    else if(root->type==NodeType::LOREXP){
        error();
    }
    else if(root->type==NodeType::CONSTEXP){
        analyzeConstExp((ConstExp*)root);
    }
    else{
        error();
    }
}