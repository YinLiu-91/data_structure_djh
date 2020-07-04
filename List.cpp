/*
 * @Author: your name
 * @Date: 2020-07-04 08:23:01
 * @LastEditTime: 2020-07-04 12:45:33
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \code\List.cpp
 */
#include "List.h"
template <typename T>
void List<T>::init()
{                              //列表初始化，在创建列表对象时统一调用
    header = new ListNode<T>;  //创建头哨兵节点
    trailer = new ListNode<T>; //创建尾哨兵节点
    header->succ = trailer;
    header->pred = NULL;
    trailer->succ = NULL;
    trailer->pred = header;
    _size = 0; //记录规模
}

//由秩到位置的转换
template <typename T>
//重载下标运算符，以通过秩直接访问列表节点（虽方便，效率低，需慎用)
T &List<T>::operator[](Rank r) const
{                                //assert :0<=r<size
    ListNodePosi(T) p = first(); //从首节点出发
    while (0 < r--)
        p = p->succ; //顺序数第r个节点即是
    return p->data;  //目标节点，返回其中所存元素
}

//查找
template <typename T>
//在无序列表节点内查找，从后往前查找
ListNodePosi(T) List<T>::find(T const &e, int n, ListNodePosi(T) p) const
{
    while (0 < n--) //(0<=n<=rank(p)<_size)对于p的最近的n个前驱，从右向左
        if (e == (p = p->pred)->data)
            return p; //逐个对比，直至命中或范围越界
    return NULL;      //p越出左边界意味着区间内不含e，查找失败
} //失败时返回NULL

template <typename T>
ListNodePosi(T) List<T>::insertAsFirst(T const &e)
{
    _size++;
    return header->insertAsSucc(e);
} //e当做首节点插入
template <typename T>
ListNodePosi(T) List<T>::insertAsLast(T const &e)
{
    _size++;
    return trailer->insertAsPred(e);
} //e当做末节点插入

template <typename T>
ListNodePosi(T) List<T>::insertA(ListNodePosi(T) p, T const &e)
{
    _size++;
    return p->insertAsSucc(e);
} //将e当做p的后继插入
template <typename T>
ListNodePosi(T) List<T>::insertB(ListNodePosi(T) p, T const &e)
{
    _size++;
    return p->insertAsPred(e);
} //e当做前驱插入

template <typename T>
//将e紧靠当前节点之前插入于当前节点所属列表(设有哨兵头节点header)
ListNodePosi(T) insertAsPred(T const &e)
{
    ListNodePosi(T) x = new ListNode(e, pred, this); //创建新节点
    pred->succ = x;
    pred = x; //设置正向链接
    return x; //返回新节点的位置
}

//后插入
template <typename T> //将e紧随当前节点之后插入于当前节点所属列表
ListNodePosi(T) ListNode<T>::insertAsSucc(T const &e)
{
    ListNodePosi(T) x = new ListNode(e, this, succ); //创建新节点
    succ->pred = x;
    succ = x; //设置逆向链接
    return x; //返回新节点的位置
}

template <typename T> //列表内部方法：复制列表中自位置p起的n项
void List<T>::copyNodes(ListNodePosi(T) p, int n)
{           //p合法，且至少n-1个真后继节点
    init(); //创建头，尾哨兵节点并做初始化
    while (n--)
    {
        insertAsLast(p->data) p = p->succ;
    }
}

template <typename T> //复制列表中自位置p起的n项（assert：p为合法位置，且至少有n-1个后继节点)
List<T>::List(ListNodePosi(T) p, int n)
{
    copyNodes(p, n);
}

template <typename T> //整体复制列表L
List<T>::List(List<T> const &L)
{
    copyNodes(L.first(), L._size());
}

template <typename T> //复制L中自第r项起的n项
List<T>::List(List<T> const &L, int r, int n)
{
    copyNodes(L[r], n);
}
//删除，常数时间内即可完成
template <typename T>
T List<T>::remove(ListNodePosi(T) p)
{                  //删除合法节点p,返回其数值
    T e = p->data; //备份待删除节点
    p->pred->succ = p->succ;
    p->succ->pred = p->pred; //后继，前驱
    delete p;
    _size--;  //释放节点，更新规模
    return e; //返回备份数值
}

//析构
//1.列表的析构首先调用clear接口删除并释放所有对外部有效的节点;
//2.释放内部头，尾哨兵节点
template <typename T>
List<T>::~List() //列表析构
{
    clear();
    delete header;
    delete trailer;
} //清空列表，释放头，尾哨兵节点

template <typename T>
int List<T>::clear()
{ //清空列表
    int oldSize = _size();
    while (0 < _size)
        remove(header->succ); //反复删除首节点，直至列表变空
    reutrn oldSize;
}

//唯一化
template <typename T>
int List<T>::deduplicate()
{ //剔除无序列表中的重复节点
    if (_size < 2)
        return 0;        //平凡列表自然无重复
    int oldSize = _size; //记录原规模
    ListNodePosi(T) p = header;
    Rank r = 0; //p从首节点开始
    while (trailer != (p = p->succ))
    {                                            //依次知道末节点
        ListNodePosi(T) q = find(p->data, r, p); //在p的r个（真）前驱中查找雷同者
        q ? remove(q) : r++;                     //若的确存在，则删除之，否则加1
    }                                            //assert：循环过程中的任意时刻，p的所有前驱互不相同
    reutrn oldSize - _size;                      //列表规模变化量，即被删除的元素数目
}

//遍历
template <typename T>
void List<T>::traverse(void (*visit)(T &)) //借助函数指针机制遍历
{
    for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ)
        visit(p->data);
}

template <typename T>
template <typename VST>
void List<T>::traverse(VST &visit)
{
    for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ)
        visit(p->data);
}

template <typename T>
int List<T>::uniquify()
{ //成批删除重复元素，效率更高
    if (_size < 2)
        return 0; //
    int oldSize = _size;
    ListNodePosi(T) p = first();
    ListNodePosi(T) q;               //p为各区段起点，q为其后继
    while (trailer != (q = p->succ)) //反复考察紧邻节点对(p,q)
        if (p->data != q->data)
            p = q;
        else
            remove(q);      //否则（雷同），删除后者
    return oldSize - _size; //列表规模变化量，即被删除的元素数量
}