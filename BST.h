/*
 * @Author: your name
 * @Date: 2020-07-18 09:24:28
 * @LastEditTime: 2020-07-18 09:34:54
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \code\BST.h
 */
#ifndef BST_H
#define BST_H
#include "BinTree.h"

template <typename T>
class BST : public BinTree<T>
{ //由BinTree派生模板类
protected:
    BinNodePosi(T) _hot;                                //“命中”节点的父亲
    BinNodePosi(T) connect34(                           //按照“3+4”结构，连接3个节点及其4棵子树
        BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), //
        BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));
    BinNodePosi(T) rotateAt(BinNodePosi(T) x); //对x及其父亲、祖父做统一旋转调整
public:
    virtual BinNodePosi(T) & search(const T &e); //查找
    virtual BinNodePosi(T) insert(const T &e);   //插入
    virtual bool remove(const T &e);             //删除
}
#endif