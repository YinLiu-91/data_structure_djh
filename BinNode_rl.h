/*
 * @Author: your name
 * @Date: 2020-07-11 15:24:39
 * @LastEditTime: 2020-07-11 15:29:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \code\BinNode_rl.h
 */
#ifndef _BINNODE_RL_H_
#define _BINNODE_RL_H_
#include "BinNode.h"
template <typename T>
BinNodePosi(T) BinNode<T>::insertAsLC(T const &e)
{
    return lc = new BinNode(e, this); //lc为当前对象的左孩子
} //将e作为当前节点的左孩子插入二叉树
template <typename T>
BinNodePosi(T) BinNode<T>::insertAsRC(T const &e)
{
    return rc = new BinNode(e, this); //rc为当前对象的右孩子
}

// template <typename T>
// template <typename VST> //元素类型、操作器
// void BinNode<T>::travIn(VST &visit)
// { //二叉树中序遍历算法统一入口
//     switch (rand() % 5)
//     { //此处随机选择以做测试，五种

//     case 1:
//         travIn_I1(this, visit);
//         break; //迭代版本1
//     case 2:
//         travIn_I2(this, visit);
//         break; //迭代版本1
//     case 3:
//         travIn_I3(this, visit);
//         break; //迭代版本1
//     case 4:
//         travIn_I4(this, visit);
//         break; //迭代版本1
//     default:
//         travIn_R(this, visit);
//         break; //迭代版本1
//     }
// }
#endif
