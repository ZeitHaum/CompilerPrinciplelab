//测试各种辅助函数
#include <bits/stdc++.h>
// #include <typeinfo>
#define error() assert(0 && "Semantic anlysis error!")
#define gen_global(name) ("\t.global " + std::string(name) + " \n") //name必须是string 类型



int main(){
    std::string s = "main";
    std::cout<<gen_global(s);
    return 0;
}