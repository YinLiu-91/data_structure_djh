/*
 * @Author: your name
 * @Date: 2020-07-11 14:34:58
 * @LastEditTime: 2020-07-11 14:56:51
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \code\BinNode.h
 */
#ifndef _BINNODE_H_
#define _BINNODE_H_
#define BinNodePosi(T) BinNode<T> *        //节点位置
#define stature((p) ? (p)->height : -1)    //节点高度（与“空树高度为-1”的约定相统一
#typedef enum {RB_RED, RB_BLACK } RBColor; //节点颜色

template <typename T>
struct BinNode
{ //二叉树节点模板类
    //成员（为简化描述起见统一开放
    T data; //数值
    BinNodePosi(T) parent;
    BinNodePosi(T) lc;
    BinNodePosi(T) rc; //父亲节点及左、右孩子
    int height;        //高度（通用）
    int npl;           //null path length（左式堆，也可直接用height代替
    RBColor color;     //颜色（红黑树）
    //构造函数
    BinNode() : parent(nullptr), lc(nullptr), rc(nullptr), height(0), npl(1), color(RB_RED) {}
    BinNode(T e, BinNodePosi(T) p = nullptr, BinNodePosi(T) lc = nullptr, BinNodePosi(T) rc = nullptr, int h = 0, int l = 1, RBColor c = RB_RED) : data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c) {}
    //操作接口
    int size();                           //统计当前节点后代总数，亦即以其为根的子树规模
    BinNodePosi(T) insertAsLC(T const &); //作为当前节点的左孩子插入新节点
    BinNodePosi(T) insertAsRC(T const &); //作为当前节点的右孩子插入新节点
    BinNodePosi(T) succ();                //取当前节点的直接后继
    template <typename VST>
    void travLevel(VST &); //子树层次遍历
    template <typename VST>
    void travPre(VST &); //子树先序遍历
    template <typename VST>
    void travIn(VST &); //子树中序遍历
    template <typename VST>
    void travPost(VST &); //子树后序遍历
    //比较器、判等器（各列其一，其余自行补充)
    bool operator<(BinNode const &bn) { return data < bn.data; };
    bool operator=(BinNode const &bn) { return data == bn.data; }
};
#include "BinNode_rl.h"
#include "BinNode_macro_BASIC.h"
#endif