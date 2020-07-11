/*
 * @Author: your name
 * @Date: 2020-07-11 11:12:16
 * @LastEditTime: 2020-07-11 11:27:48
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \code\Queue.h
 */
#ifndef _QUEUE_H_
#define _QUEUE_H_
#include "List.h"
#include "listNode.h"
template <typename T>
class Queue : public List<T>
{ //队列模板
    using ListNode<T>::insertAslast;
    using List<T>::remove;
    using List<T>::first;
    //using List<T>::data;
    void enqueue(T const &e) { insertAsLast(e); } //入队：尾部插入
    T dequeue() { return remove(first()); }       //出对；首部删除
    T &front() { return first()->data; }//对首
};
#endif