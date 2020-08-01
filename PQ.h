/*
 * @Author: your name
 * @Date: 2020-08-01 10:55:22
 * @LastEditTime: 2020-08-01 10:55:23
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \code\PQ.h
 */
#ifndef PQ_H
#define PQ_H

template <typename T>
struct PQ
{
    /* data */
    virtual void insert(T) = 0; //按照比较器确定的优先级次序插入词条
    virtual T getMax() = 0;     //取出优先级最高的词条
    virtual T delMax() = 0;     //删除优先级最高的词条
};

#endif