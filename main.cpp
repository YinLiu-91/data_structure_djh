/*
 * @Author: your name
 * @Date: 2020-06-29 20:28:10
 * @LastEditTime: 2020-07-05 14:39:59
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \code\main.cpp
 */

#include <iostream>
#include "Vector.h"
#include "Stack.h"
#include "List.h"
#include <vector>

void convert(Stack<char> &S,  __int64 n, int base)
{                       //十进制正整数n到base进制的转换
    static char digit[] //0<n,1<base<=16，新进制下的数位符号，可视base取值范围适当扩充
        = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    if (0 < n)
    {                               //在尚有余数之前，反复的
        S.push(digit[n % base]);    //逆向记录当前最低位，再

        convert(S, n / base, base); //通过递归得到所有更高位
    }
} //新进制下由高到低位，自顶向下保存于栈中
int main()
{
    Vector<int> ivec(10, 9, 1);
    Vector<int> ivec1(9, 5, 1);
    int i = 0;
    i = i;
 
    //ivec1 = ivec;
     Stack<char> s;
     
     convert(s,1000,3);

    return 0;
    std::cout << "Hello, world!\n";
}
