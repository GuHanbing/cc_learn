//
// Created by 顾涵彬 on 2019-08-19.
//

#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
using namespace std;

int main()
{
    vector<int> numbers(40);
    generate(numbers.begin(),numbers.end(),rand);
    //自动类型推断
    int c = count_if(numbers.begin(),numbers.end(),[](int x){return x%3==0;});
    cout<<"numbers中能被3整除的个数有"<<c<<"个"<<endl;
    //非自动
    int a = count_if(numbers.begin(),numbers.end(),[](int x)->int{int y=3; return x%y==0;});

    //给lambda命名
    auto fun = [](int x){return x%3==0;};

    //lambda访问作用域变量
    int key = 3;
    auto fun2 = [&key](int x){return x%key==0;};
    int c2 = count_if(numbers.begin(),numbers.end(),fun2);
    cout<<"法2：numbers中能被3整除的个数有"<<c2<<"个"<<endl;
}