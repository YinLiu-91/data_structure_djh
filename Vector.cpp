#include "Vector.h"
#include <numeric>
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
    if (_size < _capacity)
        return;
    if (_capacity < DEFAULT_CAPACITY)
        _capacity = DEFAULT_CAPACITY;
    T *oldElem = _elem;
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
        return; //不致收缩到默认值以下
    if (_size << 2 > _capacity)
        return; //以%25为界
    T *oldElem = _elem;
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
static bool lt(T &b, T &b) { return a < b; }; //less than
template <typename T>
static bool eq(T *a, T *b) { return eq(*a, *b); } //equal
template <typename T>
static bool eq(T &a, T &b) { return a==b; } //equal
 