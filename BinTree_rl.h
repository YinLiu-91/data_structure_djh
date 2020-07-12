/*
 * @Author: your name
 * @Date: 2020-07-12 08:40:34
 * @LastEditTime: 2020-07-12 13:08:02
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \code\BinTree_rl.h
 */
#ifndef _BINTREE_RL_H_
#define _BINTREE_RL_H_
#include "BinTree.h"
#include <stdlib.h>
#include "release.h"
#include "Stack.h"
#include "Queue.h"
template <typename T> //p121
int BinTree<T>::updateHeight(BinNodePosi(T) x)
{

    return x->height = 1 + std::max(stature(x->lc), stature(x->rc));
} //具体原则，因树而异
template <typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x) //更新高度
{
    while (x)
    {
        updateHeight(x);
        x = x->parent;
    }
} //从x出发，覆盖历代祖先(还可优化)

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRoot(T const &e) //p121
{
    _size = 1;
    return _root = new BinNode<T>(e);
} //将e当做根节点插入空的二叉树

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const &e)
{
    _size++;
    x->insertAsLC(e);
    updateHeightAbove(x);
    return x->lc;
} //e插入为x的左孩子
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const &e)
{
    _size++;
    x->insertAsRC(e);
    updateHeightAbove(x);
    return x->rc;
}

//p122 子树接入
template <typename T> //二叉树子树接入算法：将S当做节点x的左子树接入，S本身为空
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T> *&S)
{ //x->lc==NULL
    if (x->lc = S->_root)
        x->lc->parent = x; //接入
    _size += S->_size;
    updateHeightAbove(x); //更新全树的规模与x所有祖先的高度
    S->_root = nullptr;
    S->_size = 0;
    release(S);
    S = nullptr;
    return x;
}

template <typename T> //二叉树子树接入算法：将S当做节点x的右子树接入，S本身置空
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T> *&S)
{                          //x->rc==NULL
    if (x->rc = S->_root)  //将S的根节点作为x的右孩子
        x->rc->parent = x; //接入、令x作为该根节点的父亲
    _size += S->_size;
    updateHeightAbove(x); //更新全树的规模与x所有祖先的高度
    S->_root = nullptr;
    S->_size = 0;
    release(S);
    S = nullptr;
    return x;
}

//p123 子树删除
template <typename T> //删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
int BinTree<T>::remove(BinNodePosi(T) x)
{                                 //assert:x为二叉树中合法位置
    FromParentTo(*x) = nullptr;   //切断来自父节点的指针
    updateHeightAbove(x->parent); //更新祖先高度
    int n = removeAt(x);
    _size -= n;
    return n; //删除子树x，更新规模，返回删除节点总数
}

template <typename T> //删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
static int removeAt(BinNodePosi(T) x)
{ //assert:x为二叉树中的合法位置
    if (!x)
        return 0;                                  //递归基：空树
    int n = 1 + removeAt(x->lc) + removeAt(x->rc); //递归释放左右子树
    release(x->data);
    release(x);
    return n; //释放被摘除节点，并返回删除节点总数
}

//子树分离
template <typename T> //二叉树子树分离算法：将子树x从当前树中删除，将其封装为一棵独立子树
BinTree<T> *BinTree<T>::secede(BinNodePosi(T) x)
{                                 //assert:x为二叉树中的合法位置
    FromParentTo(*x) = nullptr;   //切断来自父节点的指针
    updateHeightAbove(x->parent); //更新原树中所有祖先的高度
    BinTree<T> *S = new BinTree<T>;
    S->_root = x;
    x->parent = nullptr; //新树以x为根
    S->_size = x->size();
    _size -= S->_size;
    return S; //更新规模，返回分离出来的子树
}

//递归版本遍历
//p124先序遍历
template <typename T, typename VST> //元素类型，操作器
void travPre_R(BinNodePosi(T) x, VST &visit)
{ //二叉树先序遍历算法（递归版）
    if (!x)
        return;
    visit(x->data);
    travPre_R(x->lc, visit);
    travPre_R(x->rc, visit);
}

//中序遍历
template <typename T, typename VST> //元素类型，操作器
void travPre_R(BinNodePosi(T) x, VST &visit)
{ //二叉树先序遍历算法（递归版）
    if (!x)
        return;
    travPre_R(x->lc, visit);
    visit(x->data);
    travPre_R(x->rc, visit);
}
//后序遍历
template <typename T, typename VST> //元素类型，操作器
void travPre_R(BinNodePosi(T) x, VST &visit)
{ //二叉树先序遍历算法（递归版）
    if (!x)
        return;
    travPre_R(x->lc, visit);
    travPre_R(x->rc, visit);
    visit(x->data);
}

//p127迭代版本遍历
//先序遍历：思路先序遍历分为两个阶段：1、沿着最左通路自顶向下访问各个节点，以及自底而上
//的遍历的对应右子树。

//从当前节点出发，沿左分支不断深入，直至没有左分支的节点；沿途节点遇到后立即访问
template <typename T, typename VST> //元素类型、操作器
static void visitAlongLeftBranch(BinNodePosi(T) x, VST &visit, Stack<BinNodePosi(T)> &S)
{
    while (x)
    {
        visit(x->data); //访问当前节点
        S.push(x->rc);  //右孩子入栈暂存（可优化：通过判断，避免空的右孩子入栈)
        x = x->lc;      //沿左分支深入一层
    }
}
template <typename T, typename VST> //元素类型、操作器
void travPre_I2(BinNodePosi(T) x, VST &visit)
{                            //二叉树先序遍历算法（迭代版本2
    Stack<BinNodePosi(T)> S; //辅助栈
    while (true)
    {
        visitAlongLeftBranch(x, visit, S); //从当前节点出发，逐批访问
        if (S.empty())
            break;   //直到栈空
        x = S.pop(); //弹出下一批的起点
    }
}

//迭代版中序遍历
//思路：沿最左侧通路自底而上，以沿途各节点为界，中序遍历序列可分解为d+1段。各段彼此独立，
//且均包括来自最左侧通路的某****一节点****L_k ,以及遍历其对应的****右子树(不是节点)*****
template <typename T> //从当前节点出发，沿左分支不断深入，直至没有左分支的节点
static void goAlongLeftBranch(BinNodePosi(T) x, Stack<BinNodePosi(T)> &S)
{
    while (x) //这里能看出节点入栈，最多也就有n个
    {
        S.push(x);
        x = x->lc;
    } //当前节点(非上面的右孩子)入栈后随即访问左侧分支深入，迭代直到无左孩子位置
}

template <typename T, typename VST> //元素类型、操作器
void travIn_I1(BinNodePosi(T) x, VST &visit)
{                            //二叉树中序遍历算法
    Stack<BinNodePosi(T)> S; //辅助栈
    while (true)
    {
        goAlongLeftBranch(x, S); //从当前节点出发，逐批入栈
        if (S.empty())
            break; //直至所有节点处理完毕
        x = S.pop();
        visit(x->data); //弹出栈顶节点并访问之
        x = x->rc;      //转向右子树
    }
}

template <typename T>
BinNodePosi(T) BinNode<T>::succ()
{                            //定位节点v的直接后继
    BinNodePosi(T) s = this; //记录后继的临时遍量
    if (rc)
    {
        //若有右孩子，则直接后继必在右子树中，具体地就是
        s = rc; //右子树中
        while (HasLChild(*s))
            s = s->lc; //最靠左（最小）的节点
    }
    else
    { //否则，直接后继应是“将当前节点包含于其左子树中的最低祖先”，具体地就是
        while (IsRChild(*s))
            s = s->parent; //逆向地沿右向分支，不断朝左上方移动
        s = s->parent;     //最后在朝右上方移动一步，即抵达直接后继（如果存在）
    }
    return s;
}

//迭代版本的中序遍历2
template <typename T, typename VST> //元素类型，操作器
void travIn_I2(BinNodePosi(T) x, VST &visit)
{                            //二叉树中序遍历算法
    Stack<BinNodePosi(T)> S; //辅助栈
    while (true)
        if (x)
        {
            S.push(x); //根节点入栈
            x = x->lc; //深入遍历左子树
        }
        else if (!S.empty())
        {
            x = S.pop();    //尚未访问的最低祖先节点退栈
            visit(x->data); //访问该祖先节点
            x = x->rc;      //遍历祖先的右子树
        }
        else
            break; //遍历完成
}

//后序遍历的迭代版本
template <typename T> //在以S栈顶节点为根的子树中，找到最高左侧可见叶节点
static void gotoHLVFL(Stack<BinNodePosi(T)> &S)
{                                      //沿途所遇节点依次入栈
    while (BinNodePosi(T) x = S.top()) //自顶而下，反复检查当前节点
        if (HasLChild(*x))
        { //尽可能向左
            if (HasRChild(*x))
                S.push(x->rc); //若有右孩子，优先入栈(最后访问)
            S.push(x->lc);     //然后才转至左孩子
        }
        else               //实在不得已
            S.push(x->rc); //才到右
    S.pop();               //返回之前，弹出栈顶的空节点
}
template <typename T, typename VST>
void travPost_I(BinNodePosi(T) x, VST &visit)
{                            //二叉树的后序遍历
    Stack<BinNodePosi(T)> S; //辅助栈
    if (x)
        S.push(x); //根节点入栈
    while (!S.empty())
    {
        if (S.top() != x->parent) //若栈顶非当前节点之父（则必为其右兄),此时需要
            gotoHLVFL(S);         //在以其右兄为根的子树中，找到HLVFL（相当于递归深入其中)
        x = S.pop();
        visit(x->data); //弹出栈顶（即前一节点之后继），并访问之
    }
}

//层次遍历 p134
template <typename T>
template <typename VST> //元素类型，操作器
void BinNode<T>::travLevel(VST &visit)
{                            //二叉树层次遍历方法
    Queue<BinNodePosi(T)> Q; //辅助队列
    Q.enqueue(this);         //根节点入列
    wile(!Q.empty())         //在队列再次变空之前反复迭代
    {
        BinNodePosi(T) x = Q.dequeue;
        visit(x->data); //取出队首节点并访问之
        if (HasLChild(*x))
            Q.enqueue(x->lc); //左孩子入队
        if (HasRChild(*x))
            Q.enqueue(x->rc); //右孩子入队
    }
}
#endif