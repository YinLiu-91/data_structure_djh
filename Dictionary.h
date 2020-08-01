/*
 * @Author: your name
 * @Date: 2020-08-01 10:41:41
 * @LastEditTime: 2020-08-01 10:41:41
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit#
 * @FilePath: \code\Dictionary.h
 */
#ifndef DICTIONARY_H
#define DICTIONARY_H
template <typename K, typename V>
struct Dictionary
{
    /* data */
    virtual int size() const = 0; //当前词条总数
    virtual bool put(K, V) = 0;   //插入词条(禁止雷同词条时可能失败)
    virtual V *get(K k) = 0;      //读取词条
    virtual bool remove(K k) = 0; //删除词条
};

#endif
