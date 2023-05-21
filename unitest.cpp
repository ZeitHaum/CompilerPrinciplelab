//测试各种辅助函数
#include <bits/stdc++.h>
// #include <typeinfo>
#define error() assert(0 && "Semantic anlysis error!")

namespace semantic{
int Atoi(std::string s)
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
};
#define f(type) f##type()

void f2(){
    std::cout<<"2\n";
}

void global(){
    std::cout<<"3\n";
}

std::string floatstring_to_int(std::string s){
    int ftoi = (int)std::stof(s);
    return std::to_string(ftoi);
}

std::string intstring_to_float(std::string s){
    float itof = (float)std::stoi(s);
    return std::to_string(itof);
}

//数组运算,计算总偏移量
int get_offset(std::vector<int> dim, std::vector<int> index){
    if(dim.size()!=index.size()){error();}
    int off_per_dim = 1;
    int ret = index.back() * off_per_dim;
    for(int i = dim.size()-1;i>=1;i--){
        off_per_dim*=dim[i];
        ret+= index[i-1] * off_per_dim;
    }
    return ret;
}

int main(){
    // std::vector<int> a{1,2,3};
    // int ap = 0;
    // std::cout<<a[ap++];
    // int a = 1;
    // int ar[2]{2,4};
    // ar[1] = 3;
    // int arr[ar[1]]{0,1,2};
    // std::cout<<arr[2];
    std::cout<<semantic::Atoi("-0xff");
    return 0;
}