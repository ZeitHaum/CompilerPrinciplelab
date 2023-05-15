/**
 * @file syntax.h
 * @author Yuntao Dai (d1581209858@live.com)
 * @brief 
 * in the second part, we already has a token stream, now we should analysis it and result in a syntax tree, 
 * which we also called it AST(abstract syntax tree)
 * @version 0.1
 * @date 2022-12-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef SYNTAX_H
#define SYNTAX_H

#include"front/abstract_syntax_tree.h"
#include"front/token.h"

#include<vector>

namespace frontend {

// definition of Parser
// a parser should take a token stream as input, then parsing it, output a AST
struct Parser {
    uint32_t index; // current token index
    const std::vector<Token>& token_stream;

    /**
     * @brief constructor
     * @param tokens: the input token_stream
     */
    Parser(const std::vector<Token>& tokens);

    /**
     * @brief destructor
     */
    ~Parser();
    
    /**
     * @brief creat the abstract syntax tree
     * @return the root of abstract syntax tree
     */
    CompUnit* get_abstract_syntax_tree();
    
    /**删除某指针指向的结点*/
    void deleteAstNode(AstNode* todelete);
    void deleteAstNodes(std::vector<AstNode*> todeletes);
    void deleteChildrens(AstNode* root);

    /***********处理文法***********/
    bool parseCompUnit(frontend::CompUnit* root);
    bool parseDecl(frontend::Decl* root);
    bool parseFuncDef(frontend::FuncDef* root);    
    bool parseConstDecl(frontend::ConstDecl* root);
    bool parseBType(frontend::BType* root);        
    bool parseConstDef(frontend::ConstDef* root);  
    bool parseConstInitVal(frontend::ConstInitVal* root);
    bool parseVarDecl(frontend::VarDecl* root);
    bool parseVarDef(frontend::VarDef* root);
    bool parseInitVal(frontend::InitVal* root);
    bool parseFuncType(frontend::FuncType* root);
    bool parseFuncFParam(frontend::FuncFParam* root);
    bool parseFuncFParams(frontend::FuncFParams* root);
    bool parseBlock(frontend::Block* root);
    bool parseBlockItem(frontend::BlockItem* root);
    bool parseStmt(frontend::Stmt* root);
    bool parseExp(frontend::Exp* root);
    bool parseCond(frontend::Cond* root);
    bool parseLVal(frontend::LVal* root);
    bool parseNumber(frontend::Number* root);
    bool parsePrimaryExp(frontend::PrimaryExp* root);
    bool parseUnaryExp(frontend::UnaryExp* root);
    bool parseUnaryOp(frontend::UnaryOp* root);
    bool parseFuncRParams(frontend::FuncRParams* root);
    bool parseMulExp(frontend::MulExp* root);
    bool parseAddExp(frontend::AddExp* root);
    bool parseRelExp(frontend::RelExp* root);
    bool parseEqExp(frontend::EqExp* root);
    bool parseLAndExp(frontend::LAndExp* root);
    bool parseLOrExp(frontend::LOrExp* root);
    bool parseConstExp(frontend::ConstExp* root);
    
    /**
     * @brief for debug, should be called in the beginning of recursive descent functions 
     * @param node: current parsing node 
     */
    void log(AstNode* node);
};

} // namespace frontend

#endif