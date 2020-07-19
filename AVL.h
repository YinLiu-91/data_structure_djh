/*
 * @Author: your name
 * @Date: 2020-07-18 13:54:02
 * @LastEditTime: 2020-07-19 09:51:22
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \code\AVL.h
 * 
 */
#ifndef AVL_H
#define AVL_H
#include "BST.h"
#include "BinNode_macro_AVL.h"

/******************************************************************************************
 * 在左、右孩子中取更高者
 * 在AVL平衡调整前，借此确定重构方案
 ******************************************************************************************/
#define tallerChild(x) (                                                                                                                                                                               \
    stature((x)->lc) > stature((x)->rc) ? (x)->lc : (                                                                                    /*左高*/                                                    \
                                                     stature((x)->lc) < stature((x)->rc) ? (x)->rc : (                                   /*右高*/                                                    \
                                                                                                      IsLChild(*(x)) ? (x)->lc : (x)->rc /*等高：与父亲x同侧者（zIg-zIg或zAg-zAg）优先*/ \
                                                                                                      )))

template <typename T>
class AVL : public BST<T>
{ //由BST派生AVL树模板类
public:
    BinNodePosi(T) insert(const T &e); //插入（重写）
    bool remove(const T &e);           //删除，重写
    //BST::search()等其余接口可直接沿用
}

template <typename T>
BinNodePosi(T) AVL<T>::insert(const T &e)
{
    using BST<T>::search;
    BinNodePosi(T) &x = search(e);
    if (x)
        return x; //确认目标节点不存在
    BinNodePosi(T) xx = x = new BinNode<T>(e, _hot);
    _size++; //创建新的节点
    //此时，x的父亲节点_hot若增高，则其祖父有可能失衡
    for (BinNodePosi(T) g = _hot; g; g = g->parent)
    {
        //从x的父亲出发，逐层检查各代祖先g
        if (!AvlBalanced(*g))
        {                                                             //一旦发现g失衡，则（采用“3+4”算法）使之复衡，并将子树
            FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g))); //重新接入原树
            break;                                                    //g复衡后，局部子树高度必然复原；其祖先也必如此，故调整随即结束
        }
        else                 //否则（g依然平衡），只需简单的
            updateHeight(g); //更新其高度（注意:即使g未失衡，高度亦可能增加

    }          //至多需要一次调整，若果真需要调整，则全树高度必然复原
    return xx; //返回新节点位置
} //无论e是否存在于原树中，总有AVL::insert(e)->data==e;

//p199 d第271话
template <typename T>
bool AVL<T>::remove(const T &e)
{ //从AVL树中删除关键码e
    BinNodePosi(T) &x = search(e);
    if (!x)
        return false; //确认目标存在
    removeAt(x, _hot);
    _size--;
    for (BinNodePosi(T) g = _hot; g; g = g->parent)
    {                                                                     //从_hot 出发，向上逐层检查各代祖先
        if (!AvlBalanced(*g))                                             //一旦发现失衡，则采用"3+4"方法使之复衡，并将该子树联至
            g = FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g))); //原父亲
        updateHeight(g);
    } //可能需要做omega(logn)此调整-无论是否做过调整，全树高度均可能降低
    return true;
} //若目标节点存在且被删除，返回true，否则返回false

//p200 3+4重构算法
template <typename T>
BinNodePosi(T) BST<T>::connect34(
    BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c, //
    BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2,
    BinNodePosi(T) T3)
{
    a->lc = T0;
    if (T0)
        T0->parent = a;
    a->rc = T1;
    if (T1)
        T1->parent = a;
    updateHeight(a);
    a->lc = T2;
    if (T2)
        T2->parent = c;
    c->rc = T3;
    if (T3)
        T3->parent = c;
    updateHeight(c);
    b->lc = a;
    a->parent = b;
    b->rc = c;
    c->parent = b;
    updateHeight(b);
    return 0; //该子树的根节点
}

template <typename T>
BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v)
{ //v为非空孙辈节点
    BinNodePosi(T) p = v->parent;
    BinNodePosi(T) g = p->parent; //视v、p和g相对位置分四种情况
    if (IsLChild(*p))             /*zig*/
        if (IsLChild(*v))
        {                          /*zig-zig*/
            p->parent = g->parent; //向上连接
            reutrn conect34(p, v, g, p->lc, v->lc, v->rc, g->rc);
        }
        else
        {                          /*zig-zag*/
            v->parent = g->parent; //向上连接
            return connect34(p, v, g, p->lc, v->lc, v->rc, g->rc);
        }

    /*zag*/
    else if (IsRChild(*v))
    {                          /*zag-zag*/
        p->parent = g->parent; //向上连接
        return connect34(g, p, v, g->lc, p->lc, v->lc, v->rc);
    }
    else
    {                          /*zag-zig*/
        v->parent = g->parent; //向上连接
        return connect34(g, v, p, g->lc, v->lc, v->rc, p->rc);
    }
}
#endif
