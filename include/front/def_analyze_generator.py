string = """
struct CompUnit: AstNode {
    /**
     * @brief constructor
     */
    CompUnit(AstNode* p = nullptr);
};

struct Decl: AstNode{
    /**
     * @brief constructor
     */
    Decl(AstNode* p = nullptr);
};

struct FuncDef: AstNode{
    string n;
    Type t;
    
    /**
     * @brief constructor
     */
    FuncDef(AstNode* p = nullptr);
};

struct ConstDecl: AstNode {
    Type t;

    /**
     * @brief constructor
     */
    ConstDecl(AstNode* p = nullptr);        
};

struct BType: AstNode {
    Type t;

    /**
     * @brief constructor
     */
    BType(AstNode* p = nullptr);
};

struct ConstDef: AstNode{
    /**
     * @brief constructor
     */
    ConstDef(AstNode* p = nullptr);
};

struct ConstInitVal: AstNode{
    /**
     * @brief constructor
     */
    ConstInitVal(AstNode* p = nullptr);
};

struct VarDecl: AstNode{
    /**
     * @brief constructor
     */
    VarDecl(AstNode* p = nullptr);
};

struct VarDef: AstNode{
    /**
     * @brief constructor
     */
    VarDef(AstNode* p = nullptr);
};

struct InitVal: AstNode{
    /**
     * @brief constructor
     */
    InitVal(AstNode* p = nullptr);
};

struct FuncType: AstNode{
    /**
     * @brief constructor
     */
    FuncType(AstNode* p = nullptr);
};

struct FuncFParam: AstNode{
    /**
     * @brief constructor
     */
    FuncFParam(AstNode* p = nullptr);
};

struct FuncFParams: AstNode{
    /**
     * @brief constructor
     */
    FuncFParams(AstNode* p = nullptr);
};

struct Block: AstNode{
    /**
     * @brief constructor
     */
    Block(AstNode* p = nullptr);
};

struct BlockItem: AstNode{
    /**
     * @brief constructor
     */
    BlockItem(AstNode* p = nullptr);
};

struct Stmt: AstNode{
    /**
     * @brief constructor
     */
    Stmt(AstNode* p = nullptr);
};

struct Exp: AstNode{
    /**
     * @brief constructor
     */
    Exp(AstNode* p = nullptr);
};

struct Cond: AstNode{
    /**
     * @brief constructor
     */
    Cond(AstNode* p = nullptr);
};

struct LVal: AstNode{
    /**
     * @brief constructor
     */
    LVal(AstNode* p = nullptr);
};

struct Number: AstNode{
    /**
     * @brief constructor
     */
    Number(AstNode* p = nullptr);
};

struct PrimaryExp: AstNode{
    /**
     * @brief constructor
     */
    PrimaryExp(AstNode* p = nullptr);
};

struct UnaryExp: AstNode{
    /**
     * @brief constructor
     */
    UnaryExp(AstNode* p = nullptr);
};

struct UnaryOp: AstNode{
    /**
     * @brief constructor
     */
    UnaryOp(AstNode* p = nullptr);
};

struct FuncRParams: AstNode{
    /**
     * @brief constructor
     */
    FuncRParams(AstNode* p = nullptr);
};

struct MulExp: AstNode{
    /**
     * @brief constructor
     */
    MulExp(AstNode* p = nullptr);
};

struct AddExp: AstNode{
    /**
     * @brief constructor
     */
    AddExp(AstNode* p = nullptr);
};

struct RelExp: AstNode{
    /**
     * @brief constructor
     */
    RelExp(AstNode* p = nullptr);
};

struct EqExp: AstNode{
    /**
     * @brief constructor
     */
    EqExp(AstNode* p = nullptr);
};

struct LAndExp: AstNode{
    /**
     * @brief constructor
     */
    LAndExp(AstNode* p = nullptr);
};

struct LOrExp: AstNode{
    /**
     * @brief constructor
     */
    LOrExp(AstNode* p = nullptr);
};

struct ConstExp: AstNode{
    /**
     * @brief constructor
     */
    ConstExp(AstNode* p = nullptr);
};
"""

import re
parttern = re.compile(r"struct (?P<name>.*?):(.*?)\n");
alis = []
for match in  parttern.finditer(string):
    alis.append(match.group('name'))

for x in alis:
    # print("\{\}".format(1))
    print("\telse if(root->type==NodeType::{0}){{\n\t        analyze{1}(({1}*)root);\n\t}}".format(x.upper(),x))