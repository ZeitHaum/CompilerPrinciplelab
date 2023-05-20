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
    std::cout<<get_offset({1,2,3},{0,1,2});
    return 0;
}