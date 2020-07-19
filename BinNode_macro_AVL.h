/*
 * @Author: your name
 * @Date: 2020-07-18 13:58:40
 * @LastEditTime: 2020-07-18 13:58:41
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \code\BinNode_marcro_AVL
 * .h
 */

#ifndef BINNODE_MACRO_AVL_H
#define BINNODE_MACRO_AVL_H
/*DSA*/ #define HeightUpdated(x) /*高度更新常规条件*/
    /*DSA*/ ((x).height == 1 + __max(stature((x).lc), stature((x).rc)))
#define Balanced(x) (stature((x).lc) == stature((x).rc))     //理想平衡条件
#define BalFac(x) (stature((x).lc) - stature((x).rc))        //平衡因子
#define AvlBalanced(x) ((-2 < BalFac(x)) && (BalFac(x) < 2)) //AVL平衡条件
#endif