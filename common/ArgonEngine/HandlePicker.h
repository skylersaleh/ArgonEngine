/**
 * @brief This file contains a template to assist with picking the proper handle function when generating TypeInfo.
 * @file HandlePicker.h
 * @author Ian Wiggins
 **/
#ifndef HANDLEPICKER_H
#define HANDLEPICKER_H

#include "TypeInfo.h"

namespace Argon
{
template <typename T,bool keyed>
struct Pick_Handle
{
    static void handle(Visitor&v, T&t)
    {
        v.handle_T(t);
    }
};

template <typename T>
struct Pick_Handle<T,true>
{
    static void handle(Visitor &v, T &t,const char*str)
    {
        v.handle_T(t,str);
    }
};

template <>
struct Pick_Handle<long,false>
{
    static void handle(Visitor&v, long t)
    {
        v.handle_Long(t);
    }
};

template <>
struct Pick_Handle<long,true>
{
    static void handle(Visitor&v,long t,const char*str)
    {
        v.handle_Long(t,str);
    }
};

template <>
struct Pick_Handle<double,false>
{
    static void handle(Visitor&v,double t)
    {
        v.handle_Double(t);
    }
};

template <>
struct Pick_Handle<double,true>
{
    static void handle(Visitor&v,double t,const char*str)
    {
        v.handle_Double(t,str);
    }
};

template <bool b>
struct Pick_Handle<float,b>:public Pick_Handle<double,b>{};

template <bool b>
struct Pick_Handle<int,b>:public Pick_Handle<long,b>{};

template <bool b>
struct Pick_Handle<unsigned,b>:public Pick_Handle<long,b>{};

template <bool b>
struct Pick_Handle<short,b>:public Pick_Handle<long,b>{};

template <bool b>
struct Pick_Handle<unsigned short,b>:public Pick_Handle<long,b>{};

template <bool b>
struct Pick_Handle<unsigned long,b>:public Pick_Handle<long,b>{};

template <typename T,bool b>
struct Pick_Handle<T&,b>:public Pick_Handle<T,b>{};

#define HANDLE_WITH_PICKER(visitor,item) Pick_Handle<decltype(item),false>::handle(visitor,item)

#define HANDLE_KEY_WITH_PICKER(visitor,item,key) Pick_Handle<decltype(item),true>::handle(visitor,item,key)

}

#endif // HANDLEPICKER_H
