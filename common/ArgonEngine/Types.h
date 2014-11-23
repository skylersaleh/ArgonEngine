/**
 * @brief this file contains an implementation of several utility classes.
 * @file Types.h
 * @author Skyler Saleh
 **/
#ifndef ARTYPES_DEF
#define ARTYPES_DEF 1
#define DEBUG_LEVEL 0
#define PI_OVER_180 -0.017453292519943
//!@namespace Argon
//!This namespace contains everything within the Argon Engine.
namespace Argon
{
//! @brief This class provides a generalized bounded number.
//! @details This means that it will not overflow, but will instead saturate, and always remain within the specified boundary.
//! It is useful for logic where you want to allow only a fixed range. Such as when scrolling.
//! If you had a min of -5 and a max of 3, its sequence would look like...
//!
//! ...-5,-5,-5,-4,-3,-2,-1,0,1,2,3,3,3,3,3...
//!
//! @tparam T the type of number to specialize.
template<typename T,int min, int max>
class BoundedNumber
{
private:
    T value;
public:

    BoundedNumber(){
        operator=(0);
    };
    BoundedNumber(T d){
        operator=(d);
    }
    BoundedNumber& operator =(T i){
        if(i>max) i=max;
        if(i<min) i=min;
        value=i;
        return *this;
    }
    operator T(){
        return value;
    }
    T operator++(int i){
        T d = value; operator=(value+1); return d;
    }
    T operator--(int i){
        T d = value; operator=(value-1); return d;
    }
    T operator++(){
        return operator=(value+1);
    }
    T operator--(){
        return operator=(value-1);
    }

    T operator+=(T i){
        return operator=(value+i);
    }
    T operator-=(T i){
        return operator=(value-i);
    }
    T operator*=(T i){
        return operator=(value*i);
    }
    T operator/=(T i){
        return operator=(value/i);
    }

    T operator+(T i){
        return value+i;
    }
    T operator-(T i){
        return value-i;
    }
    T operator*(T i){
        return value*i;
    }
    T operator/(T i){
        return value/i;
    }

    bool operator<(T i){
        return value<i;
    }
    bool operator<=(T i){
        return value<=i;
    }
    bool operator>(T i){
        return value>i;
    }
    bool operator>=(T i){
        return value>=i;
    }
    bool operator==(T i){
        return value==i;
    }

    bool operator!(){
        return !value;
    }
};
/**
 * @brief This class provides an implementation of an integer that overflows at specified values.
 * @details This class can be used as a drop in replacement for an integer, but you can specify its overflow/underflow characteristics.
 * For instance you could say I want an int with a minimum value of -5 and a maximum value of 3, and then if you were to repeatably add one,
 * The sequence would look like this:
 *
 * ...0,1,2,3,-5,-4,-3,-2,-1,0,1,2,3,-5,-4,-3,-2,-1,...
 *
 * This is very useful for implementing ring buffers as now they can be implemented in a few lines of code like:
 * \code{.cpp}
 * Node* effects[15];      //These effects are expensive so we only want 15 on screen at once. However we want to get rid of the oldest, when we already have to many.
 * RollingInt<0,14> index;
 * ...
 * effects[index++]->go(); //Advance Ring Buffer and show effect.
 * \endcode
 *
 * @tparam TMin Minimum representable value
 * @tparam Tmax Maximum representable value
 **/
template<signed TMin, signed TMax>
class RollingInt
{
private:
    int d_;
    int range;
public:
    RollingInt(){
        range=TMax-TMin; operator=(0);
    };
    RollingInt(int d){
        range=TMax-TMin; operator=(d);
    };
    RollingInt& operator =(int i){
        if(i>=TMax) {
            int x = (i-TMin)%range;
            i=x+TMin;
        }
        else if(i<TMin) {
            int x = (i-TMin)%range;
            i=TMax+x;
        }
        d_=i;
        return *this;
    }
    operator int(){
        return d_;
    }
    int operator+=(int i){
        return operator=(d_+i);
    }
    int operator-=(int i){
        return operator=(d_-i);
    }
    int operator*=(int i){
        return operator=(d_*i);
    }
    int operator/=(int i){
        return operator=(d_/i);
    }

    int operator++(int i){
        int d = d_; operator=(d_+1); return d;
    }
    int operator--(int i){
        int d = d_; operator=(d_-1); return d;
    }
    int operator++(){
        return operator=(d_+1);
    }
    int operator--(){
        return operator=(d_-1);
    }

    int operator+(int i){
        return d_+i;
    }
    int operator-(int i){
        return d_-i;
    }
    int operator*(int i){
        return d_*i;
    }
    int operator/(int i){
        return d_/i;
    }

    bool operator<(int i){
        return d_<i;
    }
    bool operator<=(int i){
        return d_<=i;
    }
    bool operator>(int i){
        return d_>i;
    }
    bool operator>=(int i){
        return d_>=i;
    }
    bool operator==(int i){
        return d_==i;
    }

    bool operator!(){
        return !d_;
    }
};

}

#endif
