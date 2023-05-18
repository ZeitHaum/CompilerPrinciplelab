//测试各种辅助函数
#include <bits/stdc++.h>
// #include <typeinfo>
#define error() assert(0 && "Semantic anlysis error!")

namespace semantic{

};
#define f(type) f##type()

void f2(){
    std::cout<<"2\n";
}

void global(){
    std::cout<<"3\n";
}

int main(){
    std::vector<int> a{1,2,3};
    int ap = 0;
    std::cout<<a[ap++];
    return 0;
}