/*
 * @Author: your name
 * @Date: 2020-06-29 20:28:10
 * @LastEditTime: 2020-07-11 10:56:00
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \code\main.cpp
 */

#include <iostream>
#include "Vector.h"
#include "Stack.h"
#include "List.h"
#include <vector>
#include "Stack_rl.h"

int main()
{
    Vector<int> ivec(10, 9, 1);
    Vector<int> ivec1(9, 5, 1);
    int i = 0;
    i = i;

    //ivec1 = ivec;
    Stack<char> s;
   // convert(s, 1000, 3);

    for (int i = 0; i < s.size(); ++i)
        std::cout << s[i] << std::endl;
    return 0;
    std::cout << "Hello, world!\n";
}
