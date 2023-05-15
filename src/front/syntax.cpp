#include"front/syntax.h"

#include<iostream>
#include<cassert>
#include<algorithm>

using frontend::Parser;

// #define DEBUG_PARSER
#define TODO assert(0 && "todo")
// #define CUR_TOKEN_IS(tk_type) (token_stream[index].type == TokenType::tk_type)
// #define PARSE_TOKEN(tk_type) root->children.push_back(parseTerm(root, TokenType::tk_type))
// #define PARSE(name, type) auto name = new type(root); assert(parse##type(name)); root->children.push_back(name); 
#define error() assert(0 && "Syntax anlysis error!")
#define backtrack() index = tmp_index;deleteChildrens(root);return false
#define indexcopy() int tmp_index = index
#define PARSE_BEGIN_CHECK() if(index>=token_stream.size()){return false;}
#define newNode(type,name) auto name = new type(root); root->children.push_back(name)
#define PARSE_BACKTRACK(type,name) newNode(type,name); if(!parse##type(name)) {backtrack();}
#define cur_token_is(tk_type) (token_stream[index].type == tk_type)
#define parse_term(name) Term* name = new Term(token_stream[index],root); root->children.push_back(name); index++
#define parse_begin() PARSE_BEGIN_CHECK() indexcopy()
#define CHECKINDEX_BACKTRACK() if(index>=token_stream.size()){backtrack();}
#define parse_error() error(); return false
#define PARSE_TERM_BACKTRACK(tk_type,name) if(!cur_token_is(tk_type)){backtrack();} else{parse_term(name);}
#define def_parse(type) bool frontend::Parser::parse##type(frontend::type* root)
#define debug_reach() std::cerr<<"successfully reach "<<__LINE__<<"!\n"
#define debug_index() std::cerr<<" cur index:"<<index<<" "
#define debug_token() std::cerr<<" cur token:"<<toString(token_stream[index].type)<<" "<<token_stream[index].value<<" "
#define debug_newline() std::cerr<<"\n"
#define debug_index_token() debug_index(); debug_token(); debug_newline()

Parser::Parser(const std::vector<frontend::Token>& tokens): index(0), token_stream(tokens) {}

Parser::~Parser() {}

frontend::CompUnit* Parser::get_abstract_syntax_tree(){
    //起始函数
    CompUnit* root = new CompUnit();
    if(parseCompUnit(root)==false){
        //DEBUG
        std::cerr<<"final index:"<<index<<"\n";
        error();
    }
    return root;
}

/**删除某指针指向的结点*/
void frontend::Parser::deleteAstNode(AstNode* todelete){
    if(todelete==nullptr){
        return;
    }
    if(todelete->parent==nullptr){
        error();
    }
    if(todelete->parent->children.back()==todelete){
        todelete->parent->children.pop_back();
    }
    else{
        //保险起见检查所有儿子
        std::vector<frontend::AstNode*>& children = todelete->parent->children;
        children.erase(std::remove(children.begin(),children.end(),todelete),children.end());
    }
    delete todelete;
    todelete = nullptr;//避免成为野指针
}

/**删除一系列指针指向的结点*/
void frontend::Parser::deleteAstNodes(std::vector<frontend::AstNode*> todeletes){
    for(auto todelete : todeletes){
        frontend::Parser::deleteAstNode(todelete);
    }
}

/**删除父亲结点的所有儿子*/
void frontend::Parser::deleteChildrens(frontend::AstNode* root){
    for(auto iter = root->children.rbegin();iter!=root->children.rend();iter++){
        frontend::Parser::deleteAstNode(*iter);
    }
}

//1 处理CompUnit -> (Decl | FuncDef) [CompUnit]
def_parse(CompUnit){
    parse_begin();
    newNode(Decl,decl_child);
    if(parseDecl(decl_child)==false){//parse失败
        //回溯
        index = tmp_index;
        deleteAstNode(decl_child);
        PARSE_BACKTRACK(FuncDef,c1)
    }
    //处理可能的CompUnit
    if(this->index<this->token_stream.size()){//缺少LL(k)
        PARSE_BACKTRACK(CompUnit,c2)
        return true;
    }
    else if(this->index==token_stream.size()){
        return true;
    }
    else{
        backtrack();
    }
    parse_error();
}

//2 处理 Decl -> ConstDecl | VarDecl
def_parse(Decl){
    parse_begin();
    newNode(ConstDecl,constdecl_child);
    if(parseConstDecl(constdecl_child)==false){
        //回溯
        index = tmp_index;
        deleteAstNode(constdecl_child);
        PARSE_BACKTRACK(VarDecl,c1)
        return true;
    }
    return true;
}

//3 处理 ConstDecl -> 'const' BType ConstDef { ',' ConstDef } ';'
def_parse(ConstDecl){
    parse_begin();
    PARSE_TERM_BACKTRACK(TokenType::CONSTTK,tb1)
    PARSE_BACKTRACK(BType, c1)
    PARSE_BACKTRACK(ConstDef,c2)
    while(index<token_stream.size() && cur_token_is(TokenType::COMMA)){
        if(token_stream[index].type!=frontend::TokenType::COMMA){
            backtrack();
        }
        parse_term(t2);
        //尝试ConstDef
        PARSE_BACKTRACK(ConstDef,c3)
    }
    CHECKINDEX_BACKTRACK()
    //尝试分号
    PARSE_TERM_BACKTRACK(TokenType::SEMICN,tb2)
    return true;
}

//4 处理 BType -> 'int' | 'float'
def_parse(BType){
    parse_begin();
    //根据FIRST选择parse
    if(cur_token_is(TokenType::INTTK)){
        parse_term(t1);
        return true;
    }
    else if(cur_token_is(TokenType::FLOATTK)){
        parse_term(t2);
        return true;
    }
    else{
        backtrack();
        return false;
    }
    parse_error();
}

//5 处理 ConstDef -> Ident { '[' ConstExp ']' } '=' ConstInitVal
def_parse(ConstDef){
    parse_begin();
    //尝试Ident
    PARSE_TERM_BACKTRACK(TokenType::IDENFR,tb1)
    //尝试{ '[' ConstExp ']' }
    CHECKINDEX_BACKTRACK()
    while(index<token_stream.size() && cur_token_is(TokenType::LBRACK)){
        //尝试'['
        parse_term(t1);
        PARSE_BACKTRACK(ConstExp,c1)
        CHECKINDEX_BACKTRACK()
        //尝试']'
        PARSE_TERM_BACKTRACK(TokenType::RBRACK,tb2)
    }
    CHECKINDEX_BACKTRACK()
    /*尝试{ '[' ConstExp ']' } 结束 */
    //尝试'='
    PARSE_TERM_BACKTRACK(TokenType::ASSIGN,tb3)
    CHECKINDEX_BACKTRACK()
    //尝试ConstInitVal
    PARSE_BACKTRACK(ConstInitVal,c2)
    else{
        return true;
    }
    parse_error();
}

//6 处理ConstInitVal -> ConstExp | '{' [ ConstInitVal { ',' ConstInitVal } ] '}'
def_parse(ConstInitVal){
    parse_begin();
    //根据FIRST选择路径
    //ConstExp绝不可能生成LBRACE
    if(cur_token_is(TokenType::LBRACE)){
        //尝试'{'
        parse_term(t1);
        //根据FIRST选择路径
        CHECKINDEX_BACKTRACK()
        if(cur_token_is(TokenType::RBRACE)){
            //尝试'}'
            parse_term(t2);
            return true;
        }
        else{
            //尝试[ ConstInitVal { ',' ConstInitVal } ] '}'
            PARSE_BACKTRACK(ConstInitVal,c1)
            CHECKINDEX_BACKTRACK()
            //尝试{ ',' ConstInitVal }
            while(index<token_stream.size() && !cur_token_is(TokenType::RBRACE)){
                //尝试','
                PARSE_TERM_BACKTRACK(TokenType::COMMA,tb1)
                CHECKINDEX_BACKTRACK()
                PARSE_BACKTRACK(ConstInitVal,c2)
            }
            CHECKINDEX_BACKTRACK()
            PARSE_TERM_BACKTRACK(TokenType::RBRACE, tb2);
            return true;
        }
        parse_error();
    }
    else{
        //尝试 ConstExp
        PARSE_BACKTRACK(ConstExp,c3);
        return true;  
    }
    parse_error();
}

//7 处理 VarDecl -> BType VarDef { ',' VarDef } ';'
def_parse(VarDecl){
    parse_begin();
    PARSE_BACKTRACK(BType,c1)
    PARSE_BACKTRACK(VarDef,c2)
    while(index<token_stream.size() && cur_token_is(TokenType::COMMA)){
        PARSE_TERM_BACKTRACK(TokenType::COMMA,tb1)
        CHECKINDEX_BACKTRACK()
        PARSE_BACKTRACK(VarDef,c3)
    }
    CHECKINDEX_BACKTRACK()
    PARSE_TERM_BACKTRACK(TokenType::SEMICN,tb2)
    return true;
}

//8 处理 VarDef -> Ident { '[' ConstExp ']' } [ '=' InitVal ]
def_parse(VarDef){
    parse_begin();
    PARSE_TERM_BACKTRACK(TokenType::IDENFR,tb1)
    while(index<token_stream.size() && cur_token_is(TokenType::LBRACK)){
        PARSE_TERM_BACKTRACK(TokenType::LBRACK,tb2)
        CHECKINDEX_BACKTRACK()
        PARSE_BACKTRACK(ConstExp,c1)
        CHECKINDEX_BACKTRACK()
        PARSE_TERM_BACKTRACK(TokenType::RBRACK,tb3)
    }
    if(cur_token_is(TokenType::ASSIGN)){
        parse_term(t1);
        PARSE_BACKTRACK(InitVal,c2)
    }
    return true;
}

//9 处理 InitVal -> Exp | '{' [ InitVal { ',' InitVal } ] '}'
def_parse(InitVal){
    parse_begin();
    //根据FIRST选择路径
    //Exp不可能生成'{'
    if(!cur_token_is(TokenType::LBRACE)){
        PARSE_BACKTRACK(Exp,c1)
        return true;
    }
    else{   
        PARSE_TERM_BACKTRACK(TokenType::LBRACE,tb1)
        CHECKINDEX_BACKTRACK()
        //根据FIRST选择路径
        if(cur_token_is(TokenType::RBRACE)){
            parse_term(t1);
            return true;
        }
        //处理[ InitVal { ',' InitVal } ] '}'
        else{
            PARSE_BACKTRACK(InitVal,c2)
            while(index<token_stream.size() && !cur_token_is(TokenType::RBRACE)){
                PARSE_TERM_BACKTRACK(TokenType::COMMA,tb2)
                CHECKINDEX_BACKTRACK()
                PARSE_BACKTRACK(InitVal,c3)
            }
            CHECKINDEX_BACKTRACK()
            PARSE_TERM_BACKTRACK(TokenType::RBRACE,tb3);
            return true;
        }
    }
    parse_error();
}

//10 FuncDef -> FuncType Ident '(' [FuncFParams] ')' Block
def_parse(FuncDef){
    parse_begin();
    PARSE_BACKTRACK(FuncType,c1)
    PARSE_TERM_BACKTRACK(TokenType::IDENFR,tb1)
    CHECKINDEX_BACKTRACK()
    PARSE_TERM_BACKTRACK(TokenType::LPARENT,tb2)
    //FIRST选择
    if(cur_token_is(TokenType::RPARENT)){
        CHECKINDEX_BACKTRACK()
        PARSE_TERM_BACKTRACK(TokenType::RPARENT,tb3)
    }
    else{
        PARSE_BACKTRACK(FuncFParams,c2)
        CHECKINDEX_BACKTRACK()
        PARSE_TERM_BACKTRACK(TokenType::RPARENT,tb4)
    }
    PARSE_BACKTRACK(Block,c3)
    return true;
}

//11. FuncType -> 'void' | 'int' | 'float'
def_parse(FuncType){
    parse_begin();
    if(cur_token_is(TokenType::VOIDTK)){
        parse_term(t1);
        return true;
    }
    else if(cur_token_is(TokenType::INTTK)){
        parse_term(t2);
        return true;
    }
    else if(cur_token_is(TokenType::FLOATTK)){
        parse_term(t3);
        return true;
    }
    else{
        backtrack();
    }
    parse_error();
}

//12. FuncFParam -> BType Ident ['[' ']' { '[' Exp ']' }]
def_parse(FuncFParam){
    parse_begin();
    PARSE_BACKTRACK(BType,c1)
    CHECKINDEX_BACKTRACK()
    PARSE_TERM_BACKTRACK(TokenType::IDENFR,tb1)
    //LL(2)
    if(index+1<token_stream.size() && cur_token_is(TokenType::LBRACK) && token_stream[index+1].type==TokenType::RBRACK){
        parse_term(t1);
        parse_term(t2);
        while(index<token_stream.size() &&  cur_token_is(TokenType::LBRACK)){
            parse_term(t3);
            PARSE_BACKTRACK(Exp,c2)
            CHECKINDEX_BACKTRACK()
            PARSE_TERM_BACKTRACK(TokenType::RBRACK,tb2);
        }
        if(index>token_stream.size()){
            backtrack();
        }
    }
    return true;
}

//13. FuncFParams -> FuncFParam { ',' FuncFParam }
def_parse(FuncFParams){
    parse_begin();
    PARSE_BACKTRACK(FuncFParam,c1)
    while(index<token_stream.size() && cur_token_is(TokenType::COMMA)){
        parse_term(t1);
        PARSE_BACKTRACK(FuncFParam,c2);
    }
    return true;
}

//14. Block -> '{' { BlockItem } '}'
def_parse(Block){
    parse_begin();
    PARSE_TERM_BACKTRACK(TokenType::LBRACE,tb1)
    while(index<token_stream.size() && !cur_token_is(TokenType::RBRACE)){
        PARSE_BACKTRACK(BlockItem,c1)
    }
    CHECKINDEX_BACKTRACK()
    PARSE_TERM_BACKTRACK(TokenType::RBRACE,tb2)
    return true;
}

//15. BlockItem -> Decl | Stmt
def_parse(BlockItem){
    parse_begin();
    newNode(Decl,decl_child);
    if(parseDecl(decl_child)==false){
        //回溯
        index = tmp_index;
        deleteAstNode(decl_child);
        PARSE_BACKTRACK(Stmt,c1);
    }
    return true;
}

//16. Stmt -> LVal '=' Exp ';' | Block | 'if' '(' Cond ')' Stmt [ 'else' Stmt ] | 'while' '(' Cond ')' Stmt | 'break' ';' | 'continue' ';' | 'return' [Exp] ';' | [Exp] ';'
def_parse(Stmt){
    parse_begin();
    //FIRST
    //LVal ->Ident
    if(cur_token_is(TokenType::IDENFR)){
        //LL(2), debug修正
        if(index+1<token_stream.size() && token_stream[index+1].type==TokenType::LPARENT){
            //走[Exp] ';'
            PARSE_BACKTRACK(Exp,debug_c1)
            CHECKINDEX_BACKTRACK()
            PARSE_TERM_BACKTRACK(TokenType::SEMICN,tb11)
            return true;
        }
        else{
            PARSE_BACKTRACK(LVal,c1)
            CHECKINDEX_BACKTRACK()
            PARSE_TERM_BACKTRACK(TokenType::ASSIGN,tb1)
            PARSE_BACKTRACK(Exp,c2)
            CHECKINDEX_BACKTRACK()
            PARSE_TERM_BACKTRACK(TokenType::SEMICN,tb2)
            return true;
        }
    }
    //Block -> '{'
    else if(cur_token_is(TokenType::LBRACE)){
        PARSE_BACKTRACK(Block,c3);
        return true;
    }
    //'if' '(' Cond ')' Stmt [ 'else' Stmt ] 
    else if(cur_token_is(TokenType::IFTK)){
        parse_term(t1);
        CHECKINDEX_BACKTRACK()
        PARSE_TERM_BACKTRACK(TokenType::LPARENT,tb3)
        PARSE_BACKTRACK(Cond,c4)
        CHECKINDEX_BACKTRACK()
        PARSE_TERM_BACKTRACK(TokenType::RPARENT,tb4)
        PARSE_BACKTRACK(Stmt,c5)
        if(index<token_stream.size() && cur_token_is(TokenType::ELSETK)){
            PARSE_TERM_BACKTRACK(TokenType::ELSETK,tb5)
            PARSE_BACKTRACK(Stmt,c6)
        }
        return true;
    }
    //'while' '(' Cond ')' Stmt 
    else if(cur_token_is(TokenType::WHILETK)){
        parse_term(t2);
        CHECKINDEX_BACKTRACK()
        PARSE_TERM_BACKTRACK(TokenType::LPARENT,tb6)
        CHECKINDEX_BACKTRACK()
        PARSE_BACKTRACK(Cond,c7)
        CHECKINDEX_BACKTRACK()
        PARSE_TERM_BACKTRACK(TokenType::RPARENT,tb7)
        PARSE_BACKTRACK(Stmt,c8)
        return true;
    }
    //'break' ';'
    else if(cur_token_is(TokenType::BREAKTK)){
        parse_term(t3);
        CHECKINDEX_BACKTRACK()
        PARSE_TERM_BACKTRACK(TokenType::SEMICN,tb8)
        return true;
    }
    //'continue' ';'
    else if(cur_token_is(TokenType::CONTINUETK)){
        parse_term(t4);
        CHECKINDEX_BACKTRACK()
        PARSE_TERM_BACKTRACK(TokenType::SEMICN,tb9)
        return true;
    }
    //'return' [Exp] ';' 
    else if(cur_token_is(TokenType::RETURNTK)){
        parse_term(t5);
        CHECKINDEX_BACKTRACK();
        //FIRST
        if(cur_token_is(TokenType::SEMICN)){
            parse_term(t6);
            return true;
        }
        PARSE_BACKTRACK(Exp,c9);
        CHECKINDEX_BACKTRACK()
        PARSE_TERM_BACKTRACK(TokenType::SEMICN,tb10)
        return true;
    }
    //[Exp] ';'
    else{
        if(cur_token_is(TokenType::SEMICN)){
            parse_term(t7);
            return true;
        }
        PARSE_BACKTRACK(Exp,c10)
        CHECKINDEX_BACKTRACK()
        PARSE_TERM_BACKTRACK(TokenType::SEMICN,tb11)
        return true;
    }
    parse_error();
}

//17. Exp -> AddExp
def_parse(Exp){
    parse_begin();
    PARSE_BACKTRACK(AddExp,c1)
    return true;
}

//18. Cond -> LOrExp
def_parse(Cond){
    parse_begin();
    PARSE_BACKTRACK(LOrExp,c1);
    return true;
}

//19. LVal -> Ident {'[' Exp ']'}
def_parse(LVal){
    parse_begin();
    PARSE_TERM_BACKTRACK(TokenType::IDENFR,tb1)
    //FIRST
    while(index<token_stream.size() && cur_token_is(TokenType::LBRACK)){
        parse_term(t1);
        PARSE_BACKTRACK(Exp,c1)
        CHECKINDEX_BACKTRACK()
        PARSE_TERM_BACKTRACK(TokenType::RBRACK,tb2)
    }
    return true;
}

//20. Number -> IntConst | floatConst
def_parse(Number){
    parse_begin();
    //FIRST
    if(cur_token_is(TokenType::INTLTR)){
        parse_term(t1);
        return true;
    }
    else if(cur_token_is(TokenType::FLOATLTR)){
        parse_term(t2);
        return true;
    }
    else{
        return false;
    }
    parse_error();
}

//21. PrimaryExp -> '(' Exp ')' | LVal | Number
def_parse(PrimaryExp){
    parse_begin();
    //FIRST
    if(cur_token_is(TokenType::LPARENT)){
        parse_term(t1);
        PARSE_BACKTRACK(Exp,c1)
        CHECKINDEX_BACKTRACK()
        PARSE_TERM_BACKTRACK(TokenType::RPARENT,tb1)
        return true;
    }
    else if(cur_token_is(TokenType::IDENFR)){
        PARSE_BACKTRACK(LVal,c1)
        return true;
    }
    else if(cur_token_is(TokenType::INTLTR) || cur_token_is(TokenType::FLOATLTR)){
        PARSE_BACKTRACK(Number,c2)
        return true;
    }
    else{
        return false;
    }
    parse_error();
}

//22. UnaryExp -> PrimaryExp | Ident '(' [FuncRParams] ')' | UnaryOp UnaryExp
def_parse(UnaryExp){
    parse_begin();
    //LL(k)
    //FIRST
    if(cur_token_is(TokenType::PLUS) || cur_token_is(TokenType::MINU) || cur_token_is(TokenType::NOT)){
        PARSE_BACKTRACK(UnaryOp,c1)
        PARSE_BACKTRACK(UnaryExp,c2)
        return true;
    }
    //ll(2)
    else if(index+1<token_stream.size() && cur_token_is(TokenType::IDENFR) && token_stream[index+1].type==TokenType::LPARENT){
        parse_term(t1);
        parse_term(t2);
        CHECKINDEX_BACKTRACK()
        if(cur_token_is(TokenType::RPARENT)){
            parse_term(t3);
            return true;
        }
        PARSE_BACKTRACK(FuncRParams,c3)
        CHECKINDEX_BACKTRACK()
        PARSE_TERM_BACKTRACK(TokenType::RPARENT,tb1)
        return true;
    }
    else{
        PARSE_BACKTRACK(PrimaryExp,c4)
        return true;
    }
    parse_error();
}

//23. UnaryOp -> '+' | '-' | '!'
def_parse(UnaryOp){
    parse_begin();
    if(cur_token_is(TokenType::PLUS) || cur_token_is(TokenType::MINU) || cur_token_is(TokenType::NOT)){
        parse_term(t1);
        return true;
    }
    else{
        return false;
    }
    parse_error();
}

//24. FuncRParams -> Exp { ',' Exp }
def_parse(FuncRParams){
    parse_begin();
    PARSE_BACKTRACK(Exp,c1)
    while(index<token_stream.size() && cur_token_is(TokenType::COMMA)){
        parse_term(t1);
        PARSE_BACKTRACK(Exp,c2);
    }
    return true;
}

//25. MulExp -> UnaryExp { ('*' | '/' | '%') UnaryExp }
def_parse(MulExp){
    parse_begin();
    PARSE_BACKTRACK(UnaryExp,c1)
    CHECKINDEX_BACKTRACK()
    //FIRST
    while(index<token_stream.size() && (cur_token_is(TokenType::MULT) || cur_token_is(TokenType::MOD) || cur_token_is(TokenType::DIV))){
        parse_term(t1);
        PARSE_BACKTRACK(UnaryExp,c2)
    }
    return true;
}


//26. AddExp -> MulExp { ('+' | '-') MulExp }
def_parse(AddExp){
    parse_begin();
    PARSE_BACKTRACK(MulExp,c1)
    while(index<token_stream.size() && (cur_token_is(TokenType::PLUS) || cur_token_is(TokenType::MINU))){
        parse_term(t1);
        PARSE_BACKTRACK(MulExp, c2)
    }
    return true;
}

//27. RelExp -> AddExp { ('<' | '>' | '<=' | '>=') AddExp }
def_parse(RelExp){
    parse_begin();
    PARSE_BACKTRACK(AddExp,c1)
    while(index<token_stream.size() && (cur_token_is(TokenType::LSS) || cur_token_is(TokenType::GTR) || cur_token_is(TokenType::LEQ) || cur_token_is(TokenType::GEQ))){
        parse_term(t1);
        PARSE_BACKTRACK(AddExp,c2)
    }
    return true;
}


//28. EqExp -> RelExp { ('==' | '!=') RelExp }
def_parse(EqExp){
    parse_begin();
    PARSE_BACKTRACK(RelExp,c1)
    while(index<token_stream.size() && (cur_token_is(TokenType::NEQ) || cur_token_is(TokenType::EQL))){
        parse_term(t1);
        PARSE_BACKTRACK(RelExp,c2)
    }
    return true;
}


//29. LAndExp -> EqExp [ '&&' LAndExp ]
def_parse(LAndExp){
    parse_begin();
    PARSE_BACKTRACK(EqExp,c1)
    if(index<token_stream.size() && cur_token_is(TokenType::AND)){
        parse_term(t1);
        PARSE_BACKTRACK(LAndExp,c2)
    }
    return true;
}

//30. LOrExp -> LAndExp [ '||' LOrExp ]
def_parse(LOrExp){
    parse_begin();
    PARSE_BACKTRACK(LAndExp,c1)
    if(index<token_stream.size() && cur_token_is(TokenType::OR)){
        parse_term(t1);
        PARSE_BACKTRACK(LOrExp,c2)
    }
    return true;
}

//31. ConstExp -> AddExp
def_parse(ConstExp){
    parse_begin();
    PARSE_BACKTRACK(AddExp,c1);
    return true;
}

void Parser::log(AstNode* node){
#ifdef DEBUG_PARSER
        std::cout << "in parse" << toString(node->type) << ", cur_token_type::" << toString(token_stream[index].type) << ", token_val::" << token_stream[index].value << '\n';
#endif
}
