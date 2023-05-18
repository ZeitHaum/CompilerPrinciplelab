#include"front/semantic.h"

#include<cassert>
#include<typeinfo>
#include<iostream>

using ir::Instruction;
using ir::Function;
using ir::Operand;
using ir::Operator;


#define TODO() assert(0 && "TODO");
// #define GET_CHILD_PTR(node, type, index) auto node = dynamic_cast<type*>(root->children[index]); assert(node); 
// #define ANALYSIS(node, type, index) auto node = dynamic_cast<type*>(root->children[index]); assert(node); analysis##type(node, buffer);
// #define COPY_EXP_NODE(from, to) to->is_computable = from->is_computable; to->v = from->v; to->t = from->t;
#define error() assert(0 && "Semantic anlysis error!")
#define def_analyze(type) void frontend::Analyzer::analyze##type(type* root)
#define def_analyze_withret(type,returntype) returntype frontend::Analyzer::analyze##type(type* root)
#define def_analyze_withfunc(type) void frontend::Analyzer::analyze##type(type* root,ir::Function& func)
#define def_analyze_withfunc_withret(type,returntype) returntype frontend::Analyzer::analyze##type(type* root,ir::Function& func)
#define BEGIN_PTR_INIT() int b_ptr = 0;//begin_ptr
#define parse_bptr(type,name) if(b_ptr>=root->children.size()){error();} type* node_##name = (type*)root->children[b_ptr++]
#define parse_bptr_declared(type,name) if(b_ptr>=root->children.size()){error();} node_##name = (type*)root->children[b_ptr++]
#define cur_node_is(type_) ( (root->children[b_ptr]->type) == type_)
#define debug_reach() std::cerr<<"successfully reach "<<__LINE__<<"!\n"
#define warning_todo() std::cerr<<"This work is not complete in line "<<__LINE__<<"\n"

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
string frontend::SymbolTable::get_scoped_name(string id) const {
    //获取top cnt
    if(scope_stack.empty()){
        error();
    }
    const int top_cnt = this->scope_stack.back().cnt;
    return id+"_"+std::to_string(top_cnt);
}

//输入一个变量名, 在符号表中寻找最近的同名变量, 返回对应的 Operand(注意，此 Operand 的 name 是重命名后的)
Operand frontend::SymbolTable::get_operand(string id) const {
    //遍历栈
    for(int i = this->scope_stack.size()-1;i>=0;i--){
        if((scope_stack[i]).table.count(id)!=0){
            frontend::map_str_ste tmp_map =  (scope_stack[i]).table;
            return tmp_map[id].operand;
        }
    }
    error();
    return {};
}

//输入一个变量名, 在符号表中寻找最近的同名变量, 返回 STE
frontend::STE frontend::SymbolTable::get_ste(string id) const {
    //遍历栈
    for(int i = this->scope_stack.size()-1;i>=0;i--){
        if((scope_stack[i]).table.count(id)!=0){
            frontend::map_str_ste tmp_map =  (scope_stack[i]).table;
            return tmp_map[id];
        }
    }
    error();
    return {};
}

frontend::Analyzer::Analyzer():symbol_table() {
    
}

ir::Program frontend::Analyzer::get_ir_program(CompUnit* root) {
    // if(typeid(root)==typeid(CompUnit)){
    //     debug_reach();
    // }
    analyzeCompUnit(root);
    return this->anlyzed_p;
}


//1. CompUnit -> (Decl | FuncDef) [CompUnit]
def_analyze(CompUnit){
    for(auto child:root->children){analyzeAstNode(child);}
}

//2. Decl -> ConstDecl | VarDecl
def_analyze(Decl){
    for(auto child:root->children){analyzeAstNode(child);}
}

//3. ConstDecl -> 'const' BType ConstDef { ',' ConstDef } ';'
def_analyze(ConstDecl){
    TODO()
}

//4. BType -> 'int' | 'float'
def_analyze(BType){
    TODO()
}

//5. ConstDef -> Ident { '[' ConstExp ']' } '=' ConstInitVal
def_analyze(ConstDef){
    TODO()
}

//6. ConstInitVal -> ConstExp | '{' [ ConstInitVal { ',' ConstInitVal } ] '}'
def_analyze(ConstInitVal){
    TODO()
}

//7. VarDecl -> BType VarDef { ',' VarDef } ';'
def_analyze(VarDecl){
    TODO()
}

//8. VarDef -> Ident { '[' ConstExp ']' } [ '=' InitVal ]
def_analyze(VarDef){
    TODO()
}

//9. InitVal -> Exp | '{' [ InitVal { ',' InitVal } ] '}'
def_analyze(InitVal){
    TODO()
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
    }
    b_ptr++;//跳过')'
    parse_bptr(Block,block);
    
    //生成函数定义IR
    ir::Function func = ir::Function();
    ir::Type func_type = analyzeFuncType(node_functype);
    func.returnType = func_type;
    func.name = node_funcname->token.value;
    //生成函数参数
    if(node_funcfparams!=nullptr){
        TODO()
    }
    //main函数需要执行先加载global
    if(func.name=="main"){
        //TODO
        warning_todo();
    }
    //生成函数语句
    analyzeBlock(node_block,func);
    //检测main函数是否有返回值
    if(func.name=="main"){
        if(func.InstVec.empty() || func.InstVec.back()->op!=Operator::_return){
            //添加返回0
            Instruction* ret0_ins = new Instruction(Operand("0",ir::Type::IntLiteral),Operand(),Operand(),ir::Operator::_return);
            func.addInst(ret0_ins);
        }
    }
    //将Function添加到program中
    this->anlyzed_p.addFunction(func);
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
    TODO()
}

//13. FuncFParams -> FuncFParam { ',' FuncFParam }
def_analyze(FuncFParams){
    TODO()
}

//14. Block -> '{' { BlockItem } '}'
def_analyze_withfunc(Block){
    BEGIN_PTR_INIT()   
    b_ptr++;//忽略'{'
    while(cur_node_is(NodeType::BLOCKITEM)){
        parse_bptr(BlockItem,blockitem);
        analyzeBlockItem(node_blockitem,func);
    }
}

//15. BlockItem -> Decl | Stmt
def_analyze_withfunc(BlockItem){
    TODO()
}

//16. Stmt -> LVal '=' Exp ';' | Block | 'if' '(' Cond ')' Stmt [ 'else' Stmt ] | 'while' '(' Cond ')' Stmt | 'break' ';' | 'continue' ';' | 'return' [Exp] ';' | [Exp] ';'
def_analyze(Stmt){
    TODO()
}

//17. Exp -> AddExp
def_analyze(Exp){
    TODO()
}

//18. Cond -> LOrExp
def_analyze(Cond){
    TODO()
}

//19. LVal -> Ident {'[' Exp ']'}
def_analyze(LVal){
    TODO()
}

//20. Number -> IntConst | floatConst
def_analyze(Number){
    TODO()
}

//21. PrimaryExp -> '(' Exp ')' | LVal | Number
def_analyze(PrimaryExp){
    TODO()
}

//22. UnaryExp -> PrimaryExp | Ident '(' [FuncRParams] ')' | UnaryOp UnaryExp
def_analyze(UnaryExp){
    TODO()
}

//23. UnaryOp -> '+' | '-' | '!'
def_analyze(UnaryOp){
    TODO()
}

//24. FuncRParams -> Exp { ',' Exp }
def_analyze(FuncRParams){
    TODO()
}

//25. MulExp -> UnaryExp { ('*' | '/' | '%') UnaryExp }
def_analyze(MulExp){
    TODO()
}

//26. AddExp -> MulExp { ('+' | '-') MulExp }
def_analyze(AddExp){
    TODO()
}

//27. RelExp -> AddExp { ('<' | '>' | '<=' | '>=') AddExp }
def_analyze(RelExp){
    TODO()
}

//28. EqExp -> RelExp { ('==' | '!=') RelExp }
def_analyze(EqExp){
    TODO()
}

//29. LAndExp -> EqExp [ '&&' LAndExp ]
def_analyze(LAndExp){
    TODO()
}

//30. LOrExp -> LAndExp [ '||' LOrExp ]
def_analyze(LOrExp){
    TODO()
}

//31. ConstExp -> AddExp
def_analyze(ConstExp){
    TODO()
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
        analyzeConstDef((ConstDef*)root);
    }
    else if(root->type==NodeType::CONSTINITVAL){
        analyzeConstInitVal((ConstInitVal*)root);
    }
    else if(root->type==NodeType::VARDECL){
        analyzeVarDecl((VarDecl*)root);
    }
    else if(root->type==NodeType::VARDEF){
        analyzeVarDef((VarDef*)root);
    }
    else if(root->type==NodeType::INITVAL){
        analyzeInitVal((InitVal*)root);
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
        error();
    }
    else if(root->type==NodeType::BLOCKITEM){
        error();
    }
    else if(root->type==NodeType::STMT){
        analyzeStmt((Stmt*)root);
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
        analyzeLAndExp((LAndExp*)root);
    }
    else if(root->type==NodeType::LOREXP){
        analyzeLOrExp((LOrExp*)root);
    }
    else if(root->type==NodeType::CONSTEXP){
        analyzeConstExp((ConstExp*)root);
    }
    else{
        error();
    }
}