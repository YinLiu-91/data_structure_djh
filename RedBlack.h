/*
 * @Author: your name
 * @Date: 2020-07-26 17:46:45
 * @LastEditTime: 2020-07-26 17:59:16
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \code\RedBlack.h
 */
#ifndef REDBLACK_H
#define REDBLACK_H
#include "BST.h"
template <typename T>
class RedBlack : public BST<T>
{ //redblack树模板类
protected:
    void solveDoubleRed(BinNodePosi(T) x);   //双红修正
    void solveDoubleBlack(BinNodePosi(T) x); //双黑修正
    int updateHeight(BinNodePosi(T) x);      //更新节点x的高度
public:
    BinNodePosi(T) insert(const T &e); //插入（重写）
    bool remove(const T &e);           //删除（重写）
    //BST：search()等其余接口可直接沿用
}

#define IsBlack(p) (!(p) || (RB_BLACK == (p)->color)) //外部节点也视作黑节点
#define IsRed(p) (!IsBlack(p))                        //非黑即红
#define BlackHeightUpdated(x) (                       /*红黑高度更新条件*/ \
                               (stature((x).lc) == stature((x).rc)) &&             \
                               ((x).height == (IsRed(&x) ? stature((x).lc) : stature((x).lc) + 1)))

template <typename T>
int RedBlack<T>::updateHeight(BinNodePosi(T) x)
{                                                     //更新节点高度
    x->height = max(stature(x->lc), stature(x - rc)); //孩子一般黑高度相等，除非出现双黑
    return IsBlack(x) ? x->height++ : x->height;      //若当前节点为黑，则计入黑深度

} //因同一定义stature(NULL) =-1;故height比黑高度少一，好在不致影响到各种算法中的比较判断

//p231
template <typename T>
BinNodePosi(T)
    RedBlack<T>::insert(const T &e)
{ //将e插入红黑树
    BinNodePosi(T) &x = search(e);
    if (x)
        return x; //确认目标不存在，（留意对_hotd的设置
    x = new BinNode<T>(e, _hot, nullptr, nullptr, -1);
    _size++; //创建红节点：以_hot为父，黑高度-1
    solveDoubleRed(x);
    return x ? x : _hot->parent; //经双红修正后，即可返回
} //无论e是否存在于原树中，返回时总有x->data==e

//p233
//双红修正,待补充
template <typename T>
void RedBlack<T>::solvDoubleRed(BinNodePosi(T) x)
{ //x当前必为红
}

//节点删除p234
template <typename T>
bool RedBlack<T>::remove(const T &E)
{
}
//p238
//双黑修正
template <typename T>
void RedBlack<T>::solveDoubleBlack(BinNodePosi(T) r)
{
}
#endif
