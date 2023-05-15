#include"front/lexical.h"

#include<map>
#include<cassert>
#include<string>

#define TODO assert(0 && "todo")
#define error() assert(0 && "Lexical anlysis error!")
// #define DEBUG_DFA
// #define DEBUG_SCANNER

std::string frontend::toString(State s) {
    switch (s) {
    case State::Empty: return "Empty";
    case State::Ident: return "Ident";
    case State::IntLiteral: return "IntLiteral";
    case State::FloatLiteral: return "FloatLiteral";
    case State::op: return "op";
    default:
        assert(0 && "invalid State");
    }
    return "";
}

std::set<std::string> frontend::keywords= {
    "const", "int", "float", "if", "else", "while", "continue", "break", "return", "void"
};

frontend::DFA::DFA(): cur_state(frontend::State::Empty), cur_str() {}

frontend::DFA::~DFA() {}


//判断是否为op
bool frontend::DFA::isoperator(char c){
    if(isOp.empty()){
        initIsOp();
    }
    return isOp.count(c)!=0;
}

//初始化isOp
void frontend::DFA::initIsOp(){
    isOp['+'] = true;
    isOp['-'] = true;
    isOp['*'] = true;
    isOp['/'] = true;
    isOp['%'] = true;
    isOp['<'] = true;
    isOp['>'] = true;
    isOp[':'] = true;
    isOp['='] = true;
    isOp[';'] = true;
    isOp[','] = true;
    isOp['('] = true;
    isOp[')'] = true;
    isOp['['] = true;
    isOp[']'] = true;
    isOp['{'] = true;
    isOp['}'] = true;
    isOp['!'] = true;
    isOp['&'] = true;
    isOp['|'] = true;
}

//判断是否为字母
bool frontend::DFA::isalpha(char c){
    return (c>='a' && c<='z') || (c>='A' && c<='Z');
}

//判断是否为数字
bool frontend::DFA::isdigit(char c){
    return c>='0' && c<='9';
}

//判断是否含某字母(大小写)
bool frontend::DFA::iscontain(const std::string& s,char lower_char){
    char upper_char = char(lower_char - 'a' + 'A');
    return (s.find(lower_char)!= s.npos) || (s.find(upper_char)!=s.npos); 
}
        
//获取状态
std::string frontend::DFA::get_cur_state(){
    return toString(this->cur_state);
}

//获取cur_str
std::string frontend::DFA::get_cur_str(){
    return this->cur_str;
}

//根据字符串获取Tokentype
frontend::TokenType frontend::DFA::get_op_type(std::string s){
    /***********先判断关键词**************/
    if(s=="const"){
        return frontend::TokenType::CONSTTK;
    }
    else if(s=="void"){
        return frontend::TokenType::VOIDTK;
    }
    else if(s=="int"){
        return frontend::TokenType::INTTK;
    }
    else if(s=="float"){
        return frontend::TokenType::FLOATTK;
    }
    else if(s=="if"){
        return frontend::TokenType::IFTK;
    }
    else if(s=="else"){
        return frontend::TokenType::ELSETK;
    }
    else if(s=="while"){
        return frontend::TokenType::WHILETK;
    }
    else if(s=="continue"){
        return frontend::TokenType::CONTINUETK;
    }
    else if(s=="break"){
        return frontend::TokenType::BREAKTK;
    }
    else if(s=="return"){
        return frontend::TokenType::RETURNTK;
    }
    /**********判断关键词结束*************/
    //标识符第一个char必须为字母或下划线
    else if(isalpha(s[0]) || s[0]=='_'){
        return frontend::TokenType::IDENFR;
    }
    //数字以数字开头,不含'.' 且 (不含e,E 或 同时含x或h和e或E)
    else if(
            isdigit(s[0]) && 
            (s.find(".")==s.npos) && 
            ((!iscontain(s,'e')) || /**/(iscontain(s,'e') && (iscontain(s,'x') || iscontain(s,'h')))/**/)
        ){
        return frontend::TokenType::INTLTR;
    }
    //浮点数包含'.'或('e'或'E' 且不含'x','h'),且 非标识符
    else if( 
            (s.find(".")!=s.npos) || 
            (iscontain(s,'e') && !iscontain(s,'x') && !iscontain(s,'h'))
        ){
        return frontend::TokenType::FLOATLTR;
    }
    /**********判断op*************/
    else if(s=="+"){
        return frontend::TokenType::PLUS;
    }
    else if(s=="-"){
        return frontend::TokenType::MINU;
    }
    else if(s=="*"){
        return frontend::TokenType::MULT;
    }
    else if(s=="/"){
        return frontend::TokenType::DIV;
    }
    else if(s=="%"){
        return frontend::TokenType::MOD;
    }
    else if(s=="<"){
        return frontend::TokenType::LSS;
    }
    else if(s==">"){
        return frontend::TokenType::GTR;
    }
    else if(s==":"){
        return frontend::TokenType::COLON;
    }
    else if(s=="="){
        return frontend::TokenType::ASSIGN;
    }
    else if(s==";"){
        return frontend::TokenType::SEMICN;
    }
    else if(s==","){
        return frontend::TokenType::COMMA;
    }
    else if(s=="("){
        return frontend::TokenType::LPARENT;
    }
    else if(s==")"){
        return frontend::TokenType::RPARENT;
    }else if(s=="["){
        return frontend::TokenType::LBRACK;
    }
    else if(s=="]"){
        return frontend::TokenType::RBRACK;
    }
    else if(s=="{"){
        return frontend::TokenType::LBRACE;
    }
    else if(s=="}"){
        return frontend::TokenType::RBRACE;
    }
    else if(s=="!"){
        return frontend::TokenType::NOT;
    }
    else if(s=="<="){
        return frontend::TokenType::LEQ;
    }
    else if(s==">="){
        return frontend::TokenType::GEQ;
    }
    else if(s=="=="){
        return frontend::TokenType::EQL;
    }
    else if(s=="!="){
        return frontend::TokenType::NEQ;
    }
    else if(s=="&&"){
        return frontend::TokenType::AND;
    }
    else if(s=="||"){
        return frontend::TokenType::OR;
    }
    else{
        error();
        return frontend::TokenType::NOT;
    }
}

bool frontend::DFA::next(char input, Token& buf) {
#ifdef DEBUG_DFA
#include<iostream>
    std::cout << "in state [" << toString(cur_state) << "], input = \'" << input << "\', str = " << cur_str << "\t";
#endif
    // TODO;//完成状态机的转移图
    if(cur_state==frontend::State::Empty){
        if(isoperator(input)){
            cur_state = frontend::State::op;
            cur_str = "";
            cur_str.push_back(input);
        }
        else if(input<='9' && input>='0'){
            cur_state = frontend::State::IntLiteral;
            cur_str = "";
            cur_str.push_back(input);
        }
        else if(isalpha(input) || input=='_'){
            cur_state = frontend::State::Ident;
            cur_str = "";
            cur_str.push_back(input);
        }
        else if(input=='.'){
            cur_state = frontend::State::FloatLiteral;
            cur_str = "";
            cur_str.push_back(input);
        }
        else{
            cur_state = frontend::State::Empty;
        }
        return false;
    }
    else if(cur_state==frontend::State::IntLiteral){
        bool is_out = false;
        if(isoperator(input)){
            cur_state = frontend::State::op;
            is_out = true;
        }   
        else if((input>='0' && input<='9') ||input=='b' || input == 'x' || input == 'h' || input == 'd' || (input>='a' && input<='f') || (input>='A' && input<='F')){
            is_out = false;
            cur_str.push_back(input);
            if(input=='e' || input=='E'){
                if(iscontain(cur_str,'x') || iscontain(cur_str,'h')){
                    cur_state = frontend::State::IntLiteral;
                }
                else{
                    cur_state = frontend::State::FloatLiteral;
                }
            }
            else{
                cur_state = frontend::State::IntLiteral;
            }
        }
        else if(input=='.'){
            cur_state = frontend::State::FloatLiteral;
            cur_str.push_back(input);
            is_out = false;
        }
        else{
            cur_state = frontend::State::Empty;
            is_out = true;
        }
        //out
        if(is_out){
            buf.value = cur_str;
            buf.type = get_op_type(buf.value);
            cur_str = "";
            cur_str.push_back(input);
            return true;
        }
        else{
            return false;
        }
        error();return false;
    }
    else if(cur_state==frontend::State::Ident){
        bool is_out = true;
        if(isoperator(input)){
            cur_state = frontend::State::op;
            is_out = true;
        }
        else if(isalpha(input) || isdigit(input) || input=='_'){
            cur_state = frontend::State::Ident;
            cur_str.push_back(input);
            is_out = false;
        }
        else{
            cur_state = frontend::State::Empty;
            is_out = true;
        }
        //out
        if(is_out){
            buf.value = cur_str;
            buf.type = get_op_type(buf.value);
            cur_str = "";
            cur_str.push_back(input);
            return true;
        }
        else{
            return false;
        }
        error();return false;
    }
    else if(cur_state==frontend::State::FloatLiteral){
        bool is_out = false;
        if(isoperator(input)){
            if(cur_str.size()>0 && (cur_str[cur_str.size()-1]=='e' || cur_str[cur_str.size()-1]=='E')){
                cur_state = frontend::State::FloatLiteral;
                cur_str.push_back(input);
                is_out = false;
            }
            else{
                cur_state=frontend::State::op;
                is_out = true;
            }
        }
        else if(input<='9' && input>='0'){
            cur_state=frontend::State::FloatLiteral;
            cur_str.push_back(input);
            is_out = false;
        }
        else{
            cur_state = frontend::State::Empty;
            is_out = true;
        }
        //out
        if(is_out){
            buf.value = cur_str;
            buf.type = get_op_type(buf.value);
            cur_str = "";
            cur_str.push_back(input);
            return true;
        }
        else{
            return false;
        }
        error();return false;
    }
    else if(cur_state==frontend::State::op){
        bool is_out = false;
        if(isoperator(input)){
            if(input=='=' && (cur_str==">" || cur_str=="<" || cur_str=="!" || cur_str=="=")){
                is_out = false;
                cur_state = frontend::State::op;
                cur_str.push_back(input);
            }
            else if(input=='&' && cur_str=="&"){
                is_out = false;
                cur_state = frontend::State::op;
                cur_str.push_back(input);
            }
            else if(input=='|' && cur_str =="|"){
                is_out = false;
                cur_state = frontend::State::op;
                cur_str.push_back(input);
            }
            else{
                is_out = true;
                cur_state = frontend::State::op;
            }
        } 
        else if(input>='0'  && input<='9'){
            cur_state = frontend::State::IntLiteral;
            is_out = true;
        }
        else if(isalpha(input) || input=='_'){
            cur_state = frontend::State::Ident;
            is_out = true;
        }
        else if(input=='.'){
            cur_state = frontend::State::FloatLiteral;
            is_out = true;
        }
        else{
            cur_state = frontend::State::Empty;
            is_out = true;
        }
        //out
        if(is_out){
            buf.value = cur_str;
            buf.type = get_op_type(buf.value);
            cur_str = "";
            cur_str.push_back(input);
            return true;
        }
        else{
            return false;
        }
        error();return false;
    }
    else {
        error();
        return false;
    }
    error();return false;
#ifdef DEBUG_DFA
    std::cout << "next state is [" << toString(cur_state) << "], next str = " << cur_str << "\t, ret = " << ret << std::endl;
#endif
}

void frontend::DFA::reset() {
    cur_state = State::Empty;
    cur_str = "";
}

frontend::Scanner::Scanner(std::string filename): fin(filename) {
    if(!fin.is_open()) {
        assert(0 && "in Scanner constructor, input file cannot open");
    }
}

frontend::Scanner::~Scanner() {
    fin.close();
}

//删除注释
//接收一个字符串，删除所有注释
std::string frontend::Scanner::delete_annotation(std::string s){
    enum State{
        NOA, //非注释
        P, //注释前缀
        AN1, // ‘//’型注释
        AN2,S2 // ‘/**/’型注释
    };
    State state = State::NOA;
    std::string ret = "";//删除注释后的字符串
    for(char c : s){
        if(state==State::NOA){
            if(c=='/'){
                state = State::P;
            }
            else{
                state = State::NOA;
                ret.push_back(c);
            }
        }
        else if(state == State::P){//已经匹配了'/'
            if(c=='/'){
                state = State::AN1;
            }
            else if (c=='*'){
                state = State::AN2;
            }
            else{
                ret.push_back('/');
                ret.push_back(c);
                state = State::NOA;
            }
        }
        else if(state == State::AN1){
            if(c=='\n'){
                state = State::NOA; 
            }
            else{
                state = State::AN1;
            }
        }
        else if(state == State::AN2){
            if(c=='*'){
                state = State::S2;
            }
            else{
                state = State::AN2;
            }
        }
        else if(state == State::S2){
            if(c=='/'){
                state = State::NOA;
            }
            else{
                state = State::AN2;
            }
        }
        else{
            error();
        }
    }
    return ret;
}

//预处理函数
std::string frontend::Scanner::preproccess(){
    //将文件流转换为string
    std::string ret,reti;
    ret = "";
    while(std::getline(fin,reti)){
        ret+= reti;
        ret+= "\n"; 
    }
    //删除注释
    ret = delete_annotation(ret);
    return ret;
}

std::vector<frontend::Token> frontend::Scanner::run() {
    std::vector<frontend::Token> ret;
    frontend::Token tk;
    frontend::DFA dfa;
    std::string s = preproccess();    // 删除注释
    // std::cout<<s<<"\n";
    for(auto c: s) {
        if(dfa.next(c, tk)==true){
            ret.push_back(tk);
            #ifdef DEBUG_SCANNER
                #include<iostream>
                std::cout << dfa.get_cur_state()<< "token: " << toString(tk.type) << "\t" << tk.value << std::endl;
            #endif
        }
    }
    return ret;
}