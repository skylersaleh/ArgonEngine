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
template <typename T>
struct Pick_Handle
{
    static void handle(Visitor&v, T& t)
    {
        v.handle_T(t);
    }

    static void handle(Visitor&v, T&t, const char*str)
    {
        v.handle_T(t,str);
    }
};

template <>
struct Pick_Handle<long>
{
    static void handle(Visitor&v, long t)
    {
        v.handle_Long(t);
    }

    static void handle(Visitor &v, long t,const char* str)
    {
        v.handle_Long(t,str);
    }
};

template <>
struct Pick_Handle<double>
{
    static void handle(Visitor&v,double t)
    {
        v.handle_Double(t);
    }

    static void handle(Visitor &v, double t,const char*str)
    {
        v.handle_Double(t,str);
    }
};

template<>
struct Pick_Handle<double&>:public Pick_Handle<double>{};

template<>
struct Pick_Handle<float>:public Pick_Handle<double>{};

template<>
struct Pick_Handle<float&>:public Pick_Handle<double>{};

template<>
struct Pick_Handle<long&>:public Pick_Handle<long>{};

template<>
struct Pick_Handle<int>:public Pick_Handle<long>{};

template<>
struct Pick_Handle<int&>:public Pick_Handle<long>{};

template<>
struct Pick_Handle<unsigned>:public Pick_Handle<long>{};

template<>
struct Pick_Handle<unsigned&>:public Pick_Handle<long>{};

template<>
struct Pick_Handle<short>:public Pick_Handle<long>{};

template<>
struct Pick_Handle<short&>:public Pick_Handle<long>{};

template<>
struct Pick_Handle<unsigned short>:public Pick_Handle<long>{};

template<>
struct Pick_Handle<unsigned short&>:public Pick_Handle<long>{};

template<>
struct Pick_Handle<unsigned long>:public Pick_Handle<long>{};

template<>
struct Pick_Handle<unsigned long&>:public Pick_Handle<long>{};

template<>
struct Pick_Handle<bool>:public Pick_Handle<long>{};

template<>
struct Pick_Handle<bool&>:public Pick_Handle<long>{};

template<>
struct Pick_Handle<char>:public Pick_Handle<long>{};

template<>
struct Pick_Handle<char&>:public Pick_Handle<long>{};

template<>
struct Pick_Handle<unsigned char>:public Pick_Handle<long>{};

template<>
struct Pick_Handle<unsigned char&>:public Pick_Handle<long>{};


#define HANDLE_WITH_PICKER(visitor,item) Pick_Handle<decltype(item)>::handle(visitor,item)

#define HANDLE_KEY_WITH_PICKER(visitor,item,key) Pick_Handle<decltype(item)>::handle(visitor,item,key)

}

#endif // HANDLEPICKER_H
