/*
 * @Author: your name
 * @Date: 2020-07-04 08:22:50
 * @LastEditTime: 2020-07-05 11:21:38
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \code\List.h
 */

#ifndef _LISTNODE_H_
#define _LISTNODE_H_
typedef int Rank;                     //秩
#define ListNodePosi(T) ListNode<T>* //列表节点位置

template <typename T>
struct ListNode
{ //列表节点模板类，（以双向链表形式实现)
    //成员
    T data;
    ListNodePosi(T) pred;
    ListNodePosi(T) succ; //数值，前驱，后继
    //构造函数
    ListNode() {} //针对header和trailer的构造
    ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
        : data(e), pred(p), succ(s) {} //默认构造器

    //操作接口
    ListNodePosi(T) insertAsPred(T const &e); //紧靠当前节点之前插入新节点
    ListNodePosi(T) insertAsSucc(T const &e); //紧靠当前节点之后插入新节点
};
#endif //_LISTNODE_H_
