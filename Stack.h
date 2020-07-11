/*
 * @Author: your name
 * @Date: 2020-07-05 09:15:04
 * @LastEditTime: 2020-07-11 08:48:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \code\Stack.h
 */

#ifndef _STACK_H_
#define _STACK_H_
#include "Vector.h" //以向量为基类，派生出栈模板类
#include "Vector_rl.h"
//#include"Vector.cpp"
//#include"Vector.h"
template <typename T>
class Stack : public Vector<T>
{                                            //将向量的首/末端作为栈底/顶
public:  
                                    //size()、empty()以及其它开放接口，均可直接沿用
    using Vector<T>::empty;
    using Vector<T>::size;
    using Vector<T>::remove;
    using Vector<T>::insert;

   
    void push(T const &e) { insert(size(),e); }     //入栈：等效于将新元素作为向量的末元素插入
    T pop() { return remove(size() - 1); }   //出栈：等效于删除向量的末元素
   
    T &top() { return (*this)[size() - 1]; } //取顶：直接返回向量的末元素
};
#include"Stack_rl.h"
#endif //_STACK_H_


