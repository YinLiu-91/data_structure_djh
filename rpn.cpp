/*
 * @Author: your name
 * @Date: 2020-07-11 09:52:33
 * @LastEditTime: 2020-07-11 10:44:02
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \code\rpn.cpp
 */
#include "priority_stack.h"
#include "Stack.h"
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstring>
void readNumber(char *&p, Stack<double> &stk)
{                                              //将起始于p的子串解析为数值，并存入操作数栈
    stk.push((double)(*p - '0'));              //当前数位对应的数值进栈
    while (isdigit(*(++p)))                    //只要后续还有紧邻的数字（即多位整数的情况），则
        stk.push(stk.pop() * 10 + (*p - '0')); //弹出原操作数并追加新数位后，新数值重新入栈
    if ('.' != *p)
        return;                                              //此后非小数点，则意味着当前操作数解析完成
    float fraction = 1;                                      //否则，意味着还有小数部分
    while (isdigit(*(++p)))                                  //逐位加入
        stk.push(stk.pop() + (*p - '0') * (fraction /= 10)); //小数部分
}
void append(char *&rpn, double opnd)
{ //将操作数接至RPN末尾
    char buf[64];
    if (0.0 < opnd - (int)opnd)
        sprintf(buf, "%f \0", opnd); //浮点格式，或
    else
        sprintf(buf, "%d \0", (int)opnd);                                       //整数格式
    rpn = (char *)realloc(rpn, sizeof(char) * (strlen(rpn) + strlen(buf) + 1)); //扩展空间
    std::strcat(rpn, buf);                                                      //RPN加长
}

void append(char *&rpn, char optr)
{                                                       //将运算符接至RPN末尾
    int n = strlen(rpn);                                //RPN当前长度（以'\0'结尾，长度n + 1）
    rpn = (char *)realloc(rpn, sizeof(char) * (n + 3)); //扩展空间
    sprintf(rpn + n, "%c ", optr);
    rpn[n + 2] = '\0'; //接入指定的运算符
}
Operator optr2rank(char op)
{ //由运算符转译出编号
    switch (op)
    {
    case '+':
        return ADD; //加
    case '-':
        return SUB; //减
    case '*':
        return MUL; //乘
    case '/':
        return DIV; //除
    case '^':
        return POW; //乘方
    case '!':
        return FAC; //阶乘
    case '(':
        return L_P; //左括号
    case ')':
        return R_P; //右括号
    case '\0':
        return EOE; //起始符与终止符
    default:
        exit(-1); //未知运算符
    }
}
char orderBetween(char op1, char op2) //比较两个运算符之间的优先级
{
    return pri[optr2rank(op1)][optr2rank(op2)];
}

double calcu ( char op, double b ) { //执行一元运算
   switch ( op ) {
      case '!' : return ( double ) facI ( ( int ) b ); //目前仅有阶乘，可照此方式添加
      default  : exit ( -1 );
   }
}

double calcu ( double a, char op, double b ) { //执行二元运算
   switch ( op ) {
      case '+' : return a + b;
      case '-' : return a - b;
      case '*' : return a * b;
      case '/' : if ( 0==b ) exit ( -1 ); return a/b; //注意：如此判浮点数为零可能不安全
      case '^' : return pow ( a, b );
      default  : exit ( -1 );
   }
}

float evaluate(char *S, char *&RPN)
{                       //对（已剔除白空格的）表达式S求值，并转换为逆波兰式RPN
    Stack<double> opnd; //运算数栈
    Stack<char> optr;   //运算符栈
    optr.push('\0');    //尾哨兵'\0'也作为头哨兵首先入栈
    while (!optr.empty())
    { //在运算符栈非空之前，逐个处理表达式中各个字符
        if (std::isdigit(*S))
        { //若当前字符为操作数，则
            readNumber(S, opnd);
            append(RPN, opnd.top()); //读入操作数，并将其接至rpn末尾
        }
        else //若当前字符为运算符
            switch (orderBetween(optr.top(), *S))
            {
            case '<': //栈顶运算符更低时
                optr.push(*S);
                S++; //计算推迟，当前运算符进栈
                /* code */
                break;
            case '=': //优先级相等（当前运算符为右括号或者尾部哨兵时
                optr.pop();
                S++; //脱括号并接受下一个字符
                break;
            case '>':
            { //栈顶运算符优先级更高时，可实施相应的计算，并将结果重新入栈
                char op = optr.pop();
                append(RPN, op); //栈顶运算符出栈并续接至RPN末尾
                if ('!' == op)
                {                                //若属于一元运算符
                    float p0pnd = opnd.pop();    //只需要取出一个操作数，并
                    opnd.push(calcu(op, p0pnd)); //实施一元计算，结果入栈
                }
                else
                {
                    float p0pnd2 = opnd.pop(), p0pnd1 = opnd.pop(); //取出后前操作数
                    opnd.push(calcu(p0pnd1, op, p0pnd2));           //实施二元计算，结果入栈
                }
                break;
            }

            default:
                exit(-1); //逢语法错误，不做处理直接退出
            }             //switch
    }                     //while
    return opnd.pop();
}