/*
 * @Author: your name
 * @Date: 2020-07-05 16:08:02
 * @LastEditTime: 2020-07-05 16:10:01
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \code\Vector1.H
 */ 
#ifndef _VECTORRL_H_
#define _VECTORRL_H_
#include <numeric>
//using namespace self;
template <typename T>
Vector<T> &Vector<T>::operator=(Vector<T> const &V)
{ //重载赋值运算
    if (_elem)
        delete[] _elem;              //释放原有内容
    copyFrom(V._elem, 0, V._size()); //整体复制，这样是自己赋值安全的吗？不能的
    return *this;
}
template <typename T>
void Vector<T>::copyFrom(T const *A, Rank lo, Rank hi)
{
    //以数组区间A[lo,hi]复制向量
    _elem = new T[_capacity = 2 * (hi - lo)];
    _size = 0;                    //分配空间，规模清零
    while (lo < hi)               //A中元素逐个
        _elem[_size++] = A[lo++]; //复制到elem[0,hi-lo]
}

//扩容单词操作分摊时间为O(1)
template <typename T>
void Vector<T>::expand() //在每次insert前都要调用该算法
{
    if (_size < _capacity) //_size小于_capacity不增大容量
        return;
    if (_capacity < DEFAULT_CAPACITY)
        _capacity = DEFAULT_CAPACITY;
    T *oldElem = _elem;             //保留旧向量的指针
    _elem = new T[_capacity <<= 1]; //容量加倍
    for (int i = 0; i < _size; ++i)
        _elem[i] = oldElem[i];
    delete[] oldElem;
}

//缩容
template <typename T>
void Vector<T>::shrink()
{
    //装填因子过小时压缩向量所占用空间
    if (_capacity < DEFAULT_CAPACITY << 1)
        return;                     //不致收缩到默认值以下
    if (_size << 2 > _capacity)     //左移动两位相当于*4
        return;                     //以%25为界
    T *oldElem = _elem;             //此时容量必然小于25%，容量可以减半
    _elem = new T[_capacity >>= 1]; //容量减半
    for (int i = 0; i < _size; ++i)
        _elem[i] = oldElem[i]; //复制原向量内容
    delete[] oldElem;
}

//重载下标运算符
template <typename T> //下表不对下表越界检查，越界检查是程序员自己的事情
T &Vector<T>::operator[](Rank r) const
{
    return _elem[r];
}

// //置乱
// template <typename T>
// void permute(Vector<T> &V)
// {
//     for (int i = V.size(); i > 0; i--)           //自后向前
//         std::swap(V[i - 1], V[std::rand() % i]); //随机交换
// }

template <typename T>
void Vector<T>::unsort(Rank lo, Rank hi)
{                      //等概率随机置乱区间
    T *V = _elem + lo; //将子向量_elem[lo,hi]视作另一向量
    for (Rank i = hi - lo; i > 0; i--)
        std::swap(V[i - 1], V[std::rand() % i]);
}

//比对与比较
template <typename T>
static bool lt(T *a, T *b) { return lt(*a, *b); } //less than
template <typename T>
static bool lt(T &a, T &b) { return a < b; }; //less than
template <typename T>
static bool eq(T *a, T *b) { return eq(*a, *b); } //equal
template <typename T>
static bool eq(T &a, T &b) { return a == b; } //equal

//2020.07.01 无序查找
template <typename T>
Rank Vector<T>::find(T const &e, Rank lo, Rank hi) const
{                                          //assert :0<=lo<hi<=_size
    while ((lo < hi--) && (e != _elem[hi])) //这里是<不是<=
        ;                                  //从后向前，顺序查找
    //比如2<3--,此时hi=2；比对不成功，判断2<2不成功，但是hi仍会--，此时hi比lo小1
    return hi;
}

template <typename T>
Rank Vector<T>::insert(Rank r, T const &e)
{
    expand(); //若有必要，扩容
    for (int i = _size; i > r; i--)
        _elem[i] = _elem[i - 1]; //自后向前，后继元素顺次移位
    _elem[r] = e;
    return r;
}

template <typename T>
int Vector<T>::remove(Rank lo, Rank hi)
{
    if (lo == hi)
        return 0; //出于效率考虑，单独处理退化情况，比如remove(0,0)
    while (hi < _size)
        _elem[lo++] = _elem[hi++]; //[hi,_size]顺次前移ni-lo个元素
    _size = lo;                   //更新规模，丢弃[lo,_size=hi]的区间
    shrink();                     //若有必要，则缩小容
    return hi - lo;               //lo大小虽变，但是与hi同变
}
template <typename T>
T Vector<T>::remove(Rank r)
{                     //删除向量中秩为r的元素，0<=r<size
    T e = _elem[r];   //备份被删除元素
    remove(r, r + 1); //调用删除区间算法
    return e;
}

template <typename T>
int Vector<T>::deduplicate()
{
    //删除无序向量中重复的元素(高效版)
    int oldSize = _size;                              //记录原始规模
    Rank i = 1;                                       //从——elem[1]开始
    while (i < _size)                                 //自前向后逐一考察各元素                                              //在其前缀中寻找与之雷同者（至多一个)
        (find(_elem[i], 0, i) < 0) ? i++ : remove(i); //若无雷同则继续考察其后继，否则删除雷同者
    return oldSize - _size;                           //向量规模变化量，即被删除元素总数
}

//遍历
template <typename T>
void Vector<T>::traverse(void (*visit)(T &)) //借助函数指针机制
{
    for (int i = 0; i < _size; i++)
        visit(_elem[i]);
}
template <typename T>
template<typename VST> void Vector<T>::traverse(VST &visit) //借助函数对象机制
{
    for (int i = 0; i < _size; i++)
        visit(_elem[i]);
} //遍历向量

template <typename T>
int Vector<T>::disordered() const
{                                   //返回向量中逆序相邻元素对的总数
    int n = 0;                      //计数器
    for (int i = 1; i < _size; ++i) //逐一检查_size-1对相邻元素
        if (_elem[i - 1] > _elem[i])
            n++; //逆序则计数
    return n;
}

/////低效版本有序唯一化方法
// template <typename T>
// int Vector<T>::uniquify()
// { //有序向量重复元素的删除方法
//     int oldSize = _size;
//     int i = 1;                                     //当前比对元素的秩,起始于首元素
//     while (i < _size)                              //从前向后，逐一比对各对相邻元素
//         _elm[i - 1] == _elem[i] ? remove(i) : i++; //若雷同，则删除后者；否则，转至后一元素
//     return oldSize - _size;
// }

////高效版本有序数列唯一化
template <typename T>
int Vector<T>::uniquify()
{                                  //有序向量重复元素剔除算法
    Rank i = 0, j = 0;             //各对互异“相邻”的元素
    while (++j < _size)            //逐一扫描，直至末元素
        if (_elem[i] != _elem[j])  //跳过雷同者
            _elem[++i] = _elem[j]; //发现不同元素时，向前移至紧邻于前者右侧
    _size = ++i;
    shrink();     //直接删除尾部多余元素
    return j - i; //向量规模变化量，即被删除的元素数量
}

//2020.07.02
template <typename T> //在有序向量的区间[lo,hi]内，确定不大于e的最后一个结点的秩
Rank Vector<T>::search(T const &e, Rank lo, Rank hi) const
{                         //assert:0<=lo<hi<=_size
    return (rand() % 2) ? //按各50%的概率随机使用二分查找或fibonacci查找
               binSearch(_elem, e, lo, hi)
                        : fibSearch(_elem, e, lo, hi);
}

// //二分查找版本A：在有序向量的区间[lo,hi]内查找元素e,0<=lo<=hi<=_size
// template <typename T>
// static Rank binSearch(T *A, T const &e, Rank lo, Rank hi)
// {
//     while (lo < hi) //判断条件
//     {
//         //每步迭代可能要做两次比较判断，有三个分支
//         Rank mi = (lo + hi) >> 1; //以中点为轴点
//         if (e < A[mi])
//             hi = mi; //深入前半段[lo,mi)
//         else if (A[mi] < e)
//             lo = mi + 1; //深入后半段(mi,hi)继续查找
//         else
//             return mi; //在mi处命中
//     }
//     return -1;
// }

#include "Fib.h"
//Fibnacci查找算法：在有序向量的区间[lo,hi]内查找元素e,0<=lo<=hi<=_size
template <typename T>
static Rank fibSearch(T *A, T const &e, Rank lo, Rank hi)
{
    Fib fib(hi - lo); //用O(log_phi(n=hi-lo))创建Fib数列
    while (lo < hi)
    {
        //每步迭代可能要做两次比较判断，有三个分支
        while(hi - lo < fib.get()) fib.prev(); //通过向前顺序查找（分摊o(1))-至多
        Rank mi = lo + fib.get() - 1;          //确定形如Fib(k)-1的轴点
        if (e < A[mi])
            hi = mi; //深入前半段[lo,mi)继续查找
        else if (A[mi] < e)
            lo = mi + 1; //深入到后半段查找
        else
            return mi; //在mi处命中
    }
    return -1; //查找失败
}

// //二分查找版本B,从三分支到二分支
// template <typename T>
// static Rank binSearch(T *A, T const &e, Rank lo, Rank hi)
// {
//     while (1 < hi - lo)
//     {                                    //每次迭代仅需要一次比较判断，有两个分支；成功查找***不能***提前终止
//         Rank mi = (lo + hi) >> 1;        //以中点为轴点
//         (e < A[mi]) ? hi = mi : lo = mi; //经过比较后确定深入[lo,mi)或[mi,hi)
//     }                                    //出口时hi=lo+1;查找区间仅含有一个元素A[lo]
//     return (e == A[lo]) ? lo : -1;       //查找成功返回对应的秩，否则统一返回-1
// } //有多个元素命中时，不能保证返回秩最大者；查找失败时，简单地返回-1，而不能指示失败位置

//二分查找版本c
//二分查找版本c
template <typename T>
static Rank binSearch(T *A, T const &e, Rank lo, Rank hi)
{
    while (lo < hi)
    {                                        //每步迭代仅需要做一次判断，有两个分支
        Rank mi = (lo + hi) >> 1;            //以中点为轴点
        (e < A[mi]) ? hi = mi : lo = mi + 1; //
    }                                        //成功查找前不能提前终止
    return --lo;                             //循环结束时，lo为大于e的最小的秩，故lo-1即不大于e的元素的最大秩
} //有多个元素命中时，总能保证返回秩最大者；查找失败时，能够返回失败的位置

template <typename T> //向量归并排序
void Vector<T>::mergeSort(Rank lo, Rank hi)
{
    if (hi - lo < 2)
        return;             //单元素区间自然有序
    int mi = (lo + hi) / 2; //以中点为界
    mergeSort(lo, mi);
    mergeSort(mi, hi); //分别排序
    merge(lo, mi, hi);
}
template <typename T> //
void Vector<T>::merge(Rank lo, Rank mi, Rank hi)
{
    T *A = _elem + lo;
    int lb = mi - lo;
    T *B = new T[lb]; //前子向量B[0, lb) = _elem[lo, mi)
    for (Rank i = 0; i < lb; B[i] = A[i++])
        ;
    int lc = hi - mi;
    T *C = _elem + mi; //后子向量C[0, lc) = _elem[mi, hi)
    for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);)
    { //将B[j]和C[k]中癿小者续至A末尾
        if ((j < lb) && (!(k < lc) || (B[j] <= C[k])))
            A[i++] = B[j++];
        if ((k < lc) && (!(j < lb) || (C[k] < B[j])))
            A[i++] = C[k++];
    }
    delete[] B; //释放临时空间B
} //归并后得刡完整癿有序向量[lo, hi)
#endif //_VECTORRL_H_