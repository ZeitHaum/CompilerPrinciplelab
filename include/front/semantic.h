/**
 * @file semantic.h
 * @author Yuntao Dai (d1581209858@live.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-06
 * 
 * a Analyzer should 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include"ir/ir.h"
#include"front/abstract_syntax_tree.h"

#include<map>
#include<string>
#include<vector>
using std::map;
using std::string;
using std::vector;

namespace frontend {

using map_str_ste = map<string, frontend::STE>;
// definition of scope infomation
struct ScopeInfo {//作用域
    int cnt;
    // string name; 暂时无用，舍弃.
    map_str_ste table;
    ScopeInfo(int c):cnt(c){}
};

// surpport lib functions
map<std::string,ir::Function*>* get_lib_funcs();

// definition of symbol table
struct SymbolTable{//符号表
    std::vector<ScopeInfo> scope_stack;
    map<std::string,ir::Function*> functions;
    map<std::string,frontend::STE> global_symbol;

    /**
     * @brief enter a new scope, record the infomation in scope stacks
     * @param node: a Block node, entering a new Block means a new name scope
     * 进入新作用域时, 向符号表中添加 ScopeInfo, 相当于压栈
     */
    void add_scope(Block*);

    /**
     * @brief exit a scope, pop out infomations
     */
    void exit_scope();

    /**
     * @brief Get the scoped name, to deal the same name in different scopes, we change origin id to a new one with scope infomation,
     * for example, we have these code:
     * "     
     * int a;
     * {
     *      int a; ....
     * }
     * "
     * in this case, we have two variable both name 'a', after change they will be 'a' and 'a_block'
     * @param id: origin id 
     * @return string: new name with scope infomations
     */
    string get_scoped_name(string id) ;

    /**
     * @brief get the right operand with the input name
     * @param id identifier name
     * @return Operand 
     */
    ir::Operand get_operand(string id) ;

    /**
     * @brief get the right ste with the input name
     * @param id identifier name
     * @return STE 
     */
    STE get_ste(string id) ;

    /*
    添加符号
    */
    void add_ste(std::string id,STE ste);
};

// singleton class
struct Analyzer {
    int tmp_cnt = 0; //临时变量_编号
    vector<ir::Instruction*> g_init_inst;//存储global函数语句的集合
    SymbolTable symbol_table;//符号表
    ir::Program anlyzed_p;
    ir::Function* func = nullptr;//空指针表示global

    /**
     * @brief constructor
     */
    Analyzer();

    // analysis functions
    ir::Program get_ir_program(CompUnit*);

    // reject copy & assignment
    Analyzer(const Analyzer&) = delete;
    Analyzer& operator=(const Analyzer&) = delete;
    
    //获取临时变量名称
    std::string get_tmp_name();

    //类型转换类函数
    ir::Type var_to_literal(ir::Type t);
    ir::Type literal_to_var(ir::Type t);
    ir::Type ptr_to_var(ir::Type t);
    int intstring_to_int(std::string s);
    float floatstring_to_float(std::string s);
    std::string floatstring_to_int(std::string s);
    std::string intstring_to_float(std::string s);
    ir::Operand sync_literal_type(ir::Operand op,ir::Type to_type);
    void sync_literalop_type(ir::Operand& op1,ir::Operand& op2);//float隐式转换为int
    int Atoi(std::string s);//进制转换,仅限整数

    //Operand运算
    std::string add_string(std::string s1,std::string s2, frontend::TokenType addop, ir::Type addtype);
    ir::Operand add_literal(ir::Operand op1,ir::Operand op2, frontend::TokenType addop);
    ir::Operand add_var(ir::Operand op1,ir::Operand op2,frontend::TokenType addop);
    ir::Operand get_default_opeand(ir::Type t);
    ir::Operand op_to_var(ir::Operand liter_op);
    ir::Operand sync_var_type(ir::Operand op, ir::Type to_type);
    void sync_varop_type(ir::Operand& op1, ir::Operand& op2);

    //数组类运算
    int get_offset(std::vector<int>& dim, std::vector<int>& index);
    int get_alloc_size(std::vector<int>&dim);

    //def anlyzd function
    void analyzeAstNode(frontend::AstNode* root);
    void analyzeCompUnit(frontend::CompUnit* root);
    void analyzeDecl(frontend::Decl* root);
    void analyzeFuncDef(frontend::FuncDef* root);    
    void analyzeConstDecl(frontend::ConstDecl* root);
    frontend::TokenType analyzeBType(frontend::BType* root);
    void analyzeConstDef(frontend::ConstDef* root);
    void analyzeConstInitVal(frontend::ConstInitVal* root);
    void analyzeVarDecl(frontend::VarDecl* root);
    void analyzeVarDef(frontend::VarDef* root,TokenType token_type);
    void analyzeInitVal(frontend::InitVal* root, frontend::STE& ste);
    ir::Type analyzeFuncType(frontend::FuncType* root);
    void analyzeFuncFParam(frontend::FuncFParam* root);
    void analyzeFuncFParams(frontend::FuncFParams* root);
    void analyzeBlock(frontend::Block* root);
    void analyzeBlockItem(frontend::BlockItem* root);
    void analyzeStmt(frontend::Stmt* root);
    void analyzeExp(frontend::Exp* root);
    void analyzeCond(frontend::Cond* root);
    void analyzeLVal(frontend::LVal* root);
    void analyzeNumber(frontend::Number* root);
    void analyzePrimaryExp(frontend::PrimaryExp* root);
    void analyzeUnaryExp(frontend::UnaryExp* root);
    void analyzeUnaryOp(frontend::UnaryOp* root);
    void analyzeFuncRParams(frontend::FuncRParams* root);
    void analyzeMulExp(frontend::MulExp* root);
    void analyzeAddExp(frontend::AddExp* root);
    void analyzeRelExp(frontend::RelExp* root);
    void analyzeEqExp(frontend::EqExp* root);
    void analyzeLAndExp(frontend::LAndExp* root);
    void analyzeLOrExp(frontend::LOrExp* root);
    void analyzeConstExp(frontend::ConstExp* root);
};

} // namespace frontend

#endif