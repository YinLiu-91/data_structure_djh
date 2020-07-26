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

template<typename T>BinNodePosi(T)
#endif