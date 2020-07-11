/*
 * @Author: your name
 * @Date: 2020-07-11 09:28:36
 * @LastEditTime: 2020-07-11 10:49:37
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \code\Stack.cpp
 */
#include <Stack.h>
#include <cstdlib>
#include"Stack_rl.h"
// void convert(Stack<char> &S, __int64 n, int base)
// {                       //十进制正整数n到base进制的转换
//     static char digit[] //0<n,1<base<=16，新进制下的数位符号，可视base取值范围适当扩充
//         = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
//     if (0 < n)
//     {                            //在尚有余数之前，反复的
//         S.push(digit[n % base]); //逆向记录当前最低位，再

//         convert(S, n / base, base); //通过递归得到所有更高位
//     }
// } //新进制下由高到低位，自顶向下保存于栈中

//版本二
void convert(Stack<char> &S, __int64 n, int base)
{                       //十进制数n到base进制的转换（迭代版
    static char digit[] //0<n,1<base<=16，新进制下的数位符号，可视base取值范围适当扩充
        = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    while (n > 0)
    { //由低到高，逐一计算出新进制下的各位数
        int remainder = (int)(n % base);
        S.push(digit[remainder]); //余数（当前位）入栈
        n /= base;                //n更新为其对base的除商
    }
} //新进制下由高到低的各位数，自顶而下保存于栈S中

//括号匹配
bool paren(const char exp[], int lo, int hi)
{                                 //表达式括号匹配检查，可兼顾三种括号
    Stack<char> S;                //使用栈记录已发现但尚未匹配的左括号
    for (int i = 0; i <= hi; ++i) //逐个检查当前字符
        switch (exp[i])
        { //左括号之间进栈；右括号若与栈顶失配，则表达式必然不匹配
        case '(':
        case '[':
        case '{':
            S.push(exp[i]);
            break;
        case ')':
            if ((S.empty()) || ('(' != S.pop()))
                return false;
            break;
        case ']':
            if ((S.empty()) || ('[' != S.pop()))
                return false;
            break;
        case '}':
            if ((S.empty()) || ('{' != S.pop()))
                return false;
            break;
        default:
            break;
        }
    return S.empty(); //整个表达式扫描过后，栈中若仍残留（左）括号，则不匹配；否则（栈空）匹配
}
