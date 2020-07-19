/*
 * @Author: your name
 * @Date: 2020-07-18 10:01:34
 * @LastEditTime: 2020-07-18 13:53:36
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \code\BST_rl.h
 */
#ifndef BST_RL_H
#define BST_RL_H
#include "BST.h"
#include "BinTree.h"
template <typename T> //在以v为根的（AVL,SPLAY,rbTree 等）BST 子树中查找关键码
    static BinNodePosi(T) & searchIn(BinNodePosi(T) & v, const T &e, BinNodePosi(T) & hot)
{
    if (!v || (e == v->data))
        return v;                                             //递归基：在节点v(或假想的通配节点)命中
    hot = v;                                                  //一般情况：先记下当前节点，然后再
    return searchIn(((e < v->data) ? v->lc : v->rc), e, hot); //深入一层，递归查找
} //返回时，返回值指向命中节点（或假想的通配哨兵),hot指向其父亲

template <typename T>
    BinNodePosi(T) & BST<T>::search(const T &e)
{                                           //在BST查找关键码
    return searchIn(_root, e, _hot = NULL); //返回目标节点位置的引用，以便后续插入、删除操作
}

template <typename T>
BinNodePosi(T) BST<T>::insert(const T &e)
{ //将关键码e插入BST树中
    BinNodePosi(T) &x = search(e);
    if (x)
        return x;                //确认目标不存在（留意对_hot的设置)
    x = new BinNode<T>(e, _hot); //创建新节点x:以e为关键码，以_hot为父
    _size++;                     //更新全树规模
    using BinTree<T>::updateHeightAbove;
    updateHeightAbove(x); //更新x及其历代祖先的高度
    return x;             //新插入节点，必为叶子
} //无论e是否存在于原树中，返回时总有x->data==e

template <typename T>
bool BST<T>::remove(const T &e)
{ //从BST树中删除关键码
    BinNodePosi(T) &x = search(e);
    if (!x)
        return false; //确认目标存在
    removeAt(x, _hot);
    _size--; //实施删除
    usint BinTree<T>::updateHeightAbove;
    updateHeightAbove(_hot); //更新_hot及其历代祖先的高度
    return true;
} //删除成功与否，由返回值指示
//p190  第253话[2.4.2]--07B3-2
template <typename T>
static BinNodePosi(T) removeAt(BinNodePosi(T) & x, BinNodePosi(T) & hot)
{
    BinNodePosi(T) w = x;          //实际被摘除的节点，初值同x
    BinNodePosi(T) succ = nullptr; //实际被删除节点的接替者
    if (!HasLChild(*x))            //若*x的左子树为空，则
        succ = x = x->rc;          //直接将*x替换为其右子树
    else if (!HasRChild(*x))
        succ = x = x->lc; //对称的处理
    else
    {                                 //若左右子树均存在，则选择在x的直接后继作为实际被摘除节点，为此需要
        w = w->succ();                //在（右子树中）找到*x的直接后继*w
        swap(x->data, w->data);       //交换*x和*w的数据元素
        BinNodePosi(T) u = w->parent; //u是w的父亲，
        //这里让w的父亲u和其右孩子完成双向连接
        ((u == x) ? u->rc : u->lc) = succ = w->rc; //隔离节点*w
    }
    //与其祖父完成连接
    hot = w->parent; //记录实际被删除节点的父亲
    if (succ)
        succ->parent = hot; //并将被删除节点的接替者与hot相连
    release(w->data);
    release(w);
    return succ; //释放被删除节点，返回接替者
}
#endif
