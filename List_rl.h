
/*
 * @Author: LY
 * @Date: 2020-07-04 08:23:01
 * @LastEditTime: 2020-07-05 16:48:05
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \code\List.cpp
 */
#ifndef _LIST_RL_H_
#define _LIST_RL_H_
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
template <typename T> //p89画 03-B-3插入与复制
ListNodePosi(T) List<T>::insertB(ListNodePosi(T) p, T const &e)
{
    _size++;
    return p->insertAsPred(e);
} //e当做前驱插入

template <typename T> //p89画 03-B-3插入与复制
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
    //拓扑关系调整，将p前驱的后继，p后继的前驱调整，以求跳过节点p
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
    return oldSize;
}

//唯一化
template <typename T>
int List<T>::deduplicate() //p115画，--03B-5
{                          //剔除无序列表中的重复节点
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
    return oldSize - _size;                      //列表规模变化量，即被删除的元素数目
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
    ListNodePosi(T) q;               //使用listnode的默认构造函数               //p为各区段起点，q为其后继
    while (trailer != (q = p->succ)) //反复考察紧邻节点对(p,q)
        if (p->data != q->data)
            p = q;
        else
            remove(q);      //否则（雷同），删除后者
    return oldSize - _size; //列表规模变化量，即被删除的元素数量
}

template <typename T> //在有序列表内节点p（可能是trailer）的n个（真）前驱中，找到不大于e的最后者
ListNodePosi(T) List<T>::search(T const &e, int n, ListNodePosi(T) p) const
{
    //assert :0<=n<=rank(p)<_size
    while (0 <= n--) //对于p的最近的n个前驱，从右向左逐个比较
        if (((p = p->pred)->data) <= e)
            break; //直至命中，数值越界或范围越界
    //assert:至此为止p必符合输出语义约定-尽管此前最后一次关键码可能没有意义（等效于与-inf比较)
    return p; //返回查找终止的位置
} //失败时，返回区间左边界的前驱（可能是header）-调用者通过valid（）判断成功与否

//排序器
template <typename T>
void List<T>::sort(ListNodePosi(T) p, int n)
{ //列表区间排序
    switch (rand() % 3)
    { //随机选择排序方法
    case 1:
        insertionSort(p, n);
        break;
    case 2:
        selectionSort(p, n);
        break;
    case 3:
        mergeSort(p, n);
        break;
    }
}

//插入排序
template <typename T> //列表插入排序算法，对起始于位置p的n个元素排序
void List<T>::insertsionSort(ListNodePosi(T) p, int n)
{                                                //valid(0)&&rank(p)+n<=size()
    for (int r = 0; r < n; r++)                  //r即有序区间长度
    {                                            //逐一节点
        insertA(search(p->data, r, p), p->data); //查找适当的位置并插入
        p = p->succ;
        remove(p->pred); //转向下一节点
    }
}

template <typename T> //列表的选择排序算法,:对于起始位置p的n个元素排序
void List<T>::selectionSort(ListNodePosi(T) p, int n)
{ //valid(p)&&rank(p)+n<=size
    ListNodePosi(T) head = p->pred;
    ListNodePosi(T) tail = p; //

    for (itn i = 0; i < n; ++i)
        tail = tail->succ; //待排序区间为(head,tail)
    while (1 < n)
    {                                                   //至少还剩下两个节点之前，在待排序区间内
        ListNodePosi(T) max = selectMax(head->succ, n); //找出最大者（歧义时后者优先
        insertB(tail, remove(max));                     //将其移动到无序区间末尾（作为有序区间的新的首元素
        tail = tail->pred;
        n--;
    }
}

template <typename T> //起始位置于位置p的n个元素中选出最大者
ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n)
{
    ListNodePosi(T) max = p;                         //最大者暂定为首节点
    for (ListNodePosi(T) cur = p; 1 < n; n--)        //从首节点出发，将后续节点逐一于max比较
        if (!lt((cur = cur->succ)->data, max->data)) //若当前元素不小于max，则
            max = cur;                               //更新最大元素记录
    return max;
}

template <typename T> //有序列表的归并排序：当前列表中自p起的n个元素，与列表L中自q起的m个元素合并
void List<T>::merge(ListNodePosi(T) & p, int n, List<T> &L, ListNodePosi(T) q, int m)
{
    //assert: this.valid(p)&&rank(p)+n<=size&&this.sorted(p,n)
    //L.valid(q)&& rank(q)+m<=L._size && L.sorted(q,m)
    //注意：归并排序之类的场合，有可能 this=L && rank(p)+n=rank(q)
    ListNodePosi(T) pp = p->pred;            //借助前驱，以便返回前...
    while (0 < m)                            //在q尚未移出区间之前
        if ((0 < n) && (p->data <= q->data)) //若p仍在区间内且v(p)<=v(q)，则
        {
            if (q == (p = p->succ))
                break;
            n--;
        }    //p归入合并的列表，并替换为其直接后继
        else //若p已超出右边界或v(q)<v(p)，则
        {
            insertB(p, L.remove((q = q->succ)->pred));
            m--;
        }         //将q转移至p之前
    p = pp->succ; //确定归并后区间的（新）起点
}

/**
 * @description: ds
 * @param {sdfdsddd} 
 * @return: sfdsd
 */
template <typename T> //列表的归并排序算法；对起始于位置p的n个元素排序
void List<T>::mergeSort(ListNodePosi(T) & p, int n)
{
    if (n < 2)
        return;
    int m = n >> 1; //以中点为界
    ListNodePosi(T) q = p;
    for (int i = 0; i < m; ++i)
        q = q->succ; //均分列表
    mergeSort(p, m);
    mergeSort(q, n - m);          //对前后子序列分别排序
    merge(p, m, *this, q, n - m); //归并

} //注意：排序后，p依然指向归并后区间的（新）的起点


 #endif //_LIST_RL_H_