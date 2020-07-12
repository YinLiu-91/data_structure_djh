/*
 * @Author: your name
 * @Date: 2020-07-11 15:47:21
 * @LastEditTime: 2020-07-11 16:14:06
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \code\BinTree.h
 */
#ifndef _BINTREE_H_
#define _BINTREE_H_
#include "BinNode.h"
template <typename T>
class BinTree
{ //二叉树模板类
protected:
    int _size;
    BinNodePosi(T) _root;                       //规模，根节点
    virtual int updateHeight(BinNodePosi(T) x); //更新节点x的高度
    void updateHeightAbove(BinNodePosi(T) x);   //更新节点及其祖先的高度
public:
    BinTree() : _size(0), _root(nullptr) {} //构造函数
    ~BinTree()
    {
        if (0 < _size)
            remove(_root);
    }                                                            //析构函数
    int size() const { return _size; }                           //规模
    bool empty() const { return !_root; }                        //判空
    BinNodePosi(T) root() const { return _root; }                //树根
    BinNodePosi(T) insertAsRoot(T const &e);                     //插入根节点
    BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const &e);     //作为左孩子插入插入
    BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const &e);     //作为右孩子插入
    BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T> *&T); //T作为左子树插入
    BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T> *&T); //T作为右子树插入
    int remove(BinNodePosi(T) x);                                //删除位置x处节点为根的子树，返回该子树原先的规模
    BinTree<T> *secede(BinNodePosi(T) x);                        //将子树x从当前树中摘除，并将其转换为一颗独立子树
    template <typename VST>                                      //操作器
    void travLevel(VST &visit)
    {
        if (_root)
            _root->travLevel(visit);
    } //层次遍历
    template <typename VST>
    void travPre(VST &visit)
    {
        if (_root)
            _root->travPre(visit);
    }                       //先序遍历
    template <typename VST> //操作器
    void travIn(VST &visit)
    {
        if (_root)
            _root->travIn(visit);
    } //中序遍历
    template <typename VST>
    void travPost(VST &visit)
    {
        if (_root)
            _root->travPost(visit);
    }                                    //后序遍历
    bool oerator < (BinTree<T> const &t) //
    {
        return _root && t._root && lt(_root, t._root);
    }
    bool operator=(BinTree<T> cosnt &t)
    {
        return _root && t._root && (_root == t._root);
    }
};
#include"BinTree_rl.h"
#endif