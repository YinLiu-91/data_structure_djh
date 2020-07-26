/*
 * @Author: your name
 * @Date: 2020-07-26 12:37:29
 * @LastEditTime: 2020-07-26 12:37:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \code\BTNode.h
 */
#ifndef BTNODE_H
#define BTNODE_H
#include "Vector.h"
#define BTNodePosi(T) BTNode<T> * //B-树节点位置

template <typename T>
struct BTNode
{
    /* data */
    BTNodePosi(T) parent;        //父节点
    Vector<T> key;               //关键码向量
    Vector<BTNodePosi(t)> child; //孩子向量（其长度总比key多一
    //构造函数（注意：BTNode只能作为根节点创建，而且初始时有0个关键码和1个空格孩子指针
    BTNode()
    {
        parent = nullptr;
        child.insert(0, nullptr);
    }
    BTNode(T e, BTNodePosi(T) lc = nullptr, BTNodePosi(T) rc = nullptr)
    {
        parent = nullptr; //作为根节点，而且初始时
        key.insert(0, e); //只有一个关键码，以及
        child.insert(0, lc);
        child.insert(1, rc);
        if (lc)
            lc->parent = this;
        if (rc)
            rc->parent = this;
    }
};

#endif