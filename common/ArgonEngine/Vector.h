#include <config.h>
/**
 * @brief This file includes the Argon::Vector classes and provides an implementation of most common Vector Ops. It is important to remember
 * that any function using operator overloading is component wise. Dot and cross are implemented as functions.
 *
 * @file Vector.h
 * @author Skyler Saleh, Ian Wiggins
 **/

#ifndef Vector2_h
#define Vector2_h
#include "TemplateUtilities.h"
#include <cmath>
#include "TypeInfo.h"
#include "HandlePicker.h"
//!Quickly define vector operators.
#define DEFINE_VEC_OPERATOR(op) \
    template <typename T, size_t TSize> \
    INLINE VectorBase<T, TSize> operator op (const T& a, const VectorBase<T, TSize> b){ \
    VectorBase<T, TSize> v; \
    for(size_t x=0; x<TSize; ++x) v[x]=a op b[x]; \
    return v; \
    } \
    template <typename T, size_t TSize> \
    INLINE VectorBase<T, TSize> operator op (const VectorBase<T, TSize> a, const T&b){ \
    VectorBase<T, TSize> v; \
    for(size_t x=0; x<TSize; ++x) v[x]=a[x] op b; \
    return v; \
    } \
    template <typename T, size_t TSize,typename T2> \
    INLINE VectorBase<typename TypePromotion<T,T2>::type, TSize> operator op (const VectorBase<T, TSize>& a, const VectorBase<T2, TSize> b){ \
    VectorBase<typename TypePromotion<T,T2>::type, TSize> v; \
    for(size_t x=0; x<TSize; ++x) v[x]=a[x] op b[x]; \
    return v; \
    }
namespace Argon {


//To ease making type infos:
#define MAKE_VECTOR_TYPE_INFO(VectorType,Name,Count)\
    template <typename T>\
    struct TypeInfo<VectorType<T> >{\
    enum {valid=1};\
    static const char* get_name(){\
    return Name;\
}\
    static void handle(VectorType<T>& v, Visitor &vis){\
    for(int x=0; x<Count; ++x) HANDLE_WITH_PICKER(vis,v[x]);\
}\
};

/**
     * @brief The base implementation of all Vectors. All operations using operators are the component wise variation.
     * @tparam T The variable type to use for the vector.
     * @tparam TSize The number of components for the vector.
     **/
template <typename T, size_t TSize> class VectorBase {
public:
    T data[TSize];            //!< The data array.
    typedef T value_type;     //!< The variable type for the vector.
    enum {size=TSize};
    //!Initialize the Vector with zeros.
    VectorBase(){
        for(size_t s=0; s<TSize; ++s) data[s]=0;
    }
    //!Initialize all of the components in the vector to value d.
    VectorBase(const T& d){
        for(size_t s=0; s<TSize; ++s) data[s]=d;
    }
    //! Initialize the vector with another vector. If the other vector is smaller, the remaining components are assigned to zero.
    template<typename T2, size_t TSize2>
    VectorBase(const VectorBase<T2, TSize2>& v){
        for(size_t s=0; s<TSize; ++s)
            data[s]= s<TSize2 ? v[s] : 0;
    }
    //! Get a pointer to the raw data.
    T* get_data(){
        return data;
    }
    void set_all(T v){for(size_t i=0;i<TSize;++i)data[i]=v;}
    template<typename T2, size_t TSize2>
    VectorBase &operator=(const VectorBase<T2, TSize2>& v){
        size_t si = TSize< TSize2 ? TSize : TSize2; for(size_t s=0; s<si; ++s) data[s]=v[s]; return *this;
    }

    const T& operator[](size_t s) const {
        return data[s];
    }
    T& operator[](size_t s){
        return data[s];
    }
    template<typename T2>
    VectorBase& operator+=(const VectorBase<T2,TSize>& v){
        for(size_t x=0; x<TSize; ++x) data[x]+= v[x]; return *this;
    }
    template<typename T2>
    VectorBase& operator-=(const VectorBase<T2,TSize>& v){
        for(size_t x=0; x<TSize; ++x) data[x]-= v[x]; return *this;
    }
    template<typename T2>
    VectorBase& operator*=(const VectorBase<T2,TSize>& v){
        for(size_t x=0; x<TSize; ++x) data[x]*= v[x]; return *this;
    }
    template<typename T2>
    VectorBase& operator/=(const VectorBase<T2,TSize>& v){
        for(size_t x=0; x<TSize; ++x) data[x]/= v[x]; return *this;
    }

    VectorBase& operator+=(const T& v){
        for(size_t x=0; x<TSize; ++x) data[x]+= v; return *this;
    }
    VectorBase& operator-=(const T& v){
        for(size_t x=0; x<TSize; ++x) data[x]-= v; return *this;
    }
    VectorBase& operator*=(const T& v){
        for(size_t x=0; x<TSize; ++x) data[x]*= v; return *this;
    }
    VectorBase& operator/=(const T& v){
        for(size_t x=0; x<TSize; ++x) data[x]/= v; return *this;
    }
    template<typename T2> bool operator==(const VectorBase<T2,TSize> & b) const {
        for(size_t x=0; x<TSize; ++x) if(data[x]!=b[x]) return false;
        return true;
    }
    template<typename T2> bool operator!=(const VectorBase<T2,TSize> & b) const {
        for(size_t x=0; x<TSize; ++x) if(data[x]!=b[x]) return true;
        return false;
    }
    VectorBase operator+(const T& v) const {
        return VectorBase(*this)+=v;
    }
    VectorBase operator-(const T& v) const {
        return VectorBase(*this)-=v;
    }
    VectorBase operator-() const {
        return VectorBase(*this)*-1.0;
    }
    VectorBase operator*(const T& v) const {
        return VectorBase(*this)*=v;
    }
    VectorBase operator/(const T& v) const {
        return VectorBase(*this)/=v;
    }
    //! Performs a parallel add.
    T add_components() const {
        T v=data[0];
        for(size_t x=1; x<TSize; ++x) v+=data[x];
        return v;
    }

};
//!@brief Template for Vectors of 2 components, provides some convenience functions.
//!@tparam T Variable type of components.
template <typename T> struct VectorBase2 : public VectorBase<T, 2>{
    VectorBase2(){}
    VectorBase2(const T x, const T y){
        set(x,y);
    }
    void set(const T x, const T y){
        VectorBase<T, 2>::data[0]=x; VectorBase<T, 2>::data[1]=y;
    }
    template<typename T2, size_t TSize2>
    VectorBase2 &operator=(const VectorBase<T2, TSize2>& v){
        size_t si = 2< TSize2 ? 2 : TSize2; for(size_t s=0; s<si; ++s) this->data[s]=v[s]; return *this;
    }
    template<typename T2, size_t TSize2>
    VectorBase2(const VectorBase<T2, TSize2>& v){
        size_t si = 2< TSize2 ? 2 : TSize2; for(size_t s=0; s<si; ++s) this->data[s]=v[s];
    }
};
//!@brief Template for Vectors of 3 components, provides some convenience functions.
//!@tparam T Variable type of components.
template <typename T> struct VectorBase3 : public VectorBase<T, 3>{
    VectorBase3(){}
    template<typename T2, size_t TSize2>
    explicit VectorBase3(const VectorBase<T2, TSize2>& v){
        size_t si = 3< TSize2 ? 3 : TSize2; for(size_t s=0; s<si; ++s) this->data[s]=v[s];
    }
    template<typename T2>
    VectorBase3(const VectorBase<T2, 3>& v){
        for(size_t s=0; s<3; ++s) this->data[s]=v[s];
    }
    VectorBase3(const T x, const T y,const T z){
        set(x,y,z);
    }
    void set(const T x, const T y,const T z){
        this->data[0]=x; this->data[1]=y; this->data[2]=z;
    }
    template<typename T2, size_t TSize2>
    VectorBase3 &operator=(const VectorBase<T2, TSize2>& v){
        size_t si = 3< TSize2 ? 3 : TSize2; for(size_t s=0; s<si; ++s) this->data[s]=v[s]; return *this;
    }
};
//!@brief Template for Vectors of 4 components, provides some convenience functions.
//!@tparam T Variable type of components.
template <typename T> struct VectorBase4 : public VectorBase<T, 4>{
    VectorBase4(){}
    template<typename T2, size_t TSize2>
    VectorBase4(const VectorBase<T2, TSize2>& v){
        size_t si = 4< TSize2 ? 4 : TSize2; for(size_t s=0; s<si; ++s) this->data[s]=v[s];
    }
    VectorBase4(const T x, const T y,const T z,const T w){
        set(x,y,z,w);
    }
    void set(const T x, const T y,const T z,const T w){
        this->data[0]=x; this->data[1]=y; this->data[2]=z; this->data[3]=w;
    }
    template<typename T2, size_t TSize2>
    VectorBase4 &operator=(const VectorBase<T2, TSize2>& v){
        size_t si = 4< TSize2 ? 4 : TSize2; for(size_t s=0; s<si; ++s) this->data[s]=v[s]; return *this;
    }
};
typedef VectorBase4<double> Vector4d;     //!< Vector of 4 doubles
typedef VectorBase3<double> Vector3d;     //!< Vector of 3 doubles
typedef VectorBase2<double> Vector2d;     //!< Vector of 2 doubles

typedef VectorBase4<float> Vector4f;     //!< Vector of 4 floats
typedef VectorBase3<float> Vector3f;     //!< Vector of 3 floats
typedef VectorBase2<float> Vector2f;     //!< Vector of 2 floats

typedef VectorBase4<int> Vector4i;     //!< Vector of 4 ints
typedef VectorBase3<int> Vector3i;     //!< Vector of 3 ints
typedef VectorBase2<int> Vector2i;     //!< Vector of 2 ints

typedef VectorBase4<signed char> Vector4c;     //!< Vector of 4 chars
typedef VectorBase3<signed char> Vector3c;     //!< Vector of 3 chars
typedef VectorBase2<signed char> Vector2c;     //!< Vector of 2 chars

typedef VectorBase4<short> Vector4s;     //!< Vector of 4 shorts
typedef VectorBase3<short> Vector3s;     //!< Vector of 3 shorts
typedef VectorBase2<short> Vector2s;     //!< Vector of 2 shorts

typedef VectorBase4<unsigned int> Vector4ui;     //!< Vector of 4 unsigned ints
typedef VectorBase3<unsigned int> Vector3ui;     //!< Vector of 3 unsigned ints
typedef VectorBase2<unsigned int> Vector2ui;     //!< Vector of 2 unsigned ints

typedef VectorBase4<unsigned char> Vector4uc;     //!< Vector of 4 unsigned chars
typedef VectorBase3<unsigned char> Vector3uc;     //!< Vector of 3 unsigned chars
typedef VectorBase2<unsigned char> Vector2uc;     //!< Vector of 2 unsigned chars

typedef VectorBase4<unsigned short> Vector4us;     //!< Vector of 4 unsigned shorts
typedef VectorBase3<unsigned short> Vector3us;     //!< Vector of 3 unsigned shorts
typedef VectorBase2<unsigned short> Vector2us;     //!< Vector of 2 unsigned shorts
template<typename T, size_t size>
struct TypeInfo<VectorBase<T,size> >{
    enum {valid=1};
    static const char* get_name(){
        return "Vector";
    }
    static void handle(VectorBase<T,size>& v, Visitor &vis){
        for(int x=0; x<size; ++x) vis.handle_T(v[x]);
    }

};
template <size_t size>
struct TypeInfo<VectorBase<long,size> >{
    enum {valid=1};
    static const char* get_name(){
        return "Vector";
    }
    static void handle(VectorBase<long,size>& v,Visitor &vis){
        for(int x=0; x<size; ++x) vis.handle_Long(v[x]);
    }
};
template <size_t size>
struct TypeInfo<VectorBase<double,size> >{
    enum {valid=1};
    static const char* get_name(){
        return "Vector";
    }
    static void handle(VectorBase<double,size>& v,Visitor &vis){
        for(int x=0; x<size; ++x) vis.handle_Double(v[x]);
    }
};

MAKE_VECTOR_TYPE_INFO(VectorBase2,"Vector2",2)

MAKE_VECTOR_TYPE_INFO(VectorBase3,"Vector3",3)

MAKE_VECTOR_TYPE_INFO(VectorBase4,"Vector4",4)

template<typename T,size_t TSize>
VectorBase<T,TSize> max(const VectorBase<T,TSize> &a, const VectorBase<T,TSize> &b){
    VectorBase<T,TSize> c;
    for(int i=0;i<TSize;++i)c[i]=a[i]<b[i]?b[i]:a[i];
    return c;
}
template<typename T,size_t TSize>
VectorBase<T,TSize> min(const VectorBase<T,TSize> &a, const VectorBase<T,TSize> &b){
    VectorBase<T,TSize> c;
    for(int i=0;i<TSize;++i)c[i]=a[i]<b[i]?a[i]:b[i];
    return c;
}
template<typename T,size_t TSize>
VectorBase<T,TSize> abs(const VectorBase<T,TSize> &a){
    VectorBase<T,TSize> c;
    for(int i=0;i<TSize;++i)c[i]=std::abs(a[i]);
    return c;
}
template<typename T,size_t TSize>
VectorBase<T,TSize> mod(const VectorBase<T,TSize> &a,const VectorBase<T,TSize> &b){
    VectorBase<T,TSize> c;
    for(int i=0;i<TSize;++i)c[i]=std::fmod(a[i], b[i]);;
    return c;
}
DEFINE_VEC_OPERATOR(*);     //!Define all operators for component wise multiplication.
DEFINE_VEC_OPERATOR(/);     //!Define all operators for component wise division.
DEFINE_VEC_OPERATOR(+);     //!Define all operators for component wise addition.
DEFINE_VEC_OPERATOR(-);     //!Define all operators for component wise subtraction.

//! Performs the dot product of Vectors a and b.
template<typename T1, typename T2, size_t TSize>
typename TypePromotion<T1,T2>::type dot(const VectorBase<T1,TSize> & a, const VectorBase<T2,TSize> & b){
    return (a*b).add_components();
}
//! Returns the squared length of a vector. This is more efficient than the length function.
template<typename T, size_t TSize> T length_squared(const VectorBase<T,TSize> & a){
    return dot(a,a);
}
//! Returns the length(magnitude) of a vector.
template<typename T, size_t TSize> T length(const VectorBase<T,TSize> & a){
    return std::sqrt(length_squared(a));
}
//! Returns the magnitude of distance between two vectors.
template<typename T,typename T2, size_t TSize> T distance(const VectorBase<T,TSize> & a,const VectorBase<T2,TSize> & b){
    return length<T,TSize>(a-b);
}
//! Returns a vector of unit length that is in the same direction as vector a.
template<typename T, size_t TSize> VectorBase<T,TSize> normalize(const VectorBase<T,TSize> & a){
    T l = length<T,TSize>(a);
    if(l>0.0001)return a/l;
    return a;
}

const static Vector4f kRedColor(1,0,0,1);               //!< Holds a Vector representation of the color red.
const static Vector4f kGreenColor(0,1,0,1);             //!< Holds a Vector representation of the color green.
const static Vector4f kBlueColor(0,0,1,1);              //!< Holds a Vector representation of the color blue.

const static Vector4f kYellowColor(1,1,0,1);            //!< Holds a Vector representation of the color yellow.
const static Vector4f kMagentaColor(1,0,1,1);           //!< Holds a Vector representation of the color magenta.
const static Vector4f kCyanColor(0,1,1,1);              //!< Holds a Vector representation of the color cyan.

const static Vector4f kClearColor(0,0,0,0);             //!< Holds a Vector representation of a color that is completely transparent.
const static Vector4f kWhiteColor(1,1,1,1);             //!< Holds a Vector representation of the color white.
const static Vector4f kGrayColor(0.5f,0.5f,0.5f,1);     //!< Holds a Vector representation of the color gray.
const static Vector4f kBlackColor(0,0,0,1);             //!< Holds a Vector representation of the color black.

const static Vector4f kBrownColor(0.62f,0.41f,0.21f,1);      //!< Holds a Vector representation of the color brown.
const static Vector4f kPurpleColor(0.5f,0,0.5f,1);      //!< Holds a Vector representation of the color purple.
const static Vector4f kOrangeColor(1,0.5f,0,1);         //!< Holds a Vector representation of the color orange.

//! Performs the cross product of two 3 component vectors.

template<typename T, size_t comps>
VectorBase<T,comps> random_vector(){
    VectorBase<T,comps> v;
    for(size_t s =0;s<comps;++s)v[s]=(rand()%1000)/500.f-1.0;
    return v;
}

template<typename T>
static inline VectorBase<T, 3> cross(const VectorBase<T,  3> &op1,const VectorBase<T,  3> &op2)
{
    VectorBase<T, 3> v;
    v[0] = op1[1]*op2[2]-op1[2]*op2[1];
    v[1] = op1[2]*op2[0]-op1[0]*op2[2];
    v[2] = op1[0]*op2[1]-op1[1]*op2[0];

    return v;
}
static inline Vector3f hsv_to_rgb(Vector3f hsv){


    float v = hsv[2];
    hsv[0]=hsv[0]-floor(hsv[0]);
    float i = floor(hsv[0] * 6.);
    float f = hsv[0] * 6. - i;
    float p = hsv[2] * (1 - hsv[1]);
    float q = hsv[2] * (1 - hsv[1] * f);
    float t = hsv[2] * (1 - hsv[1] * (1. - f));

    if(i==0.)return Vector3f(v,t,p);
    else if(i==1) return Vector3f(q,v,p);
    else if(i==2) return Vector3f(p,v,t);
    else if(i==3) return Vector3f(p,q,v);
    else if(i==4) return Vector3f(t,p,v);

    return Vector3f(v,p,q);

}
//! Used to change a 3D vector into a 4D one.
template <typename T> VectorBase<T, 4> vect4(VectorBase<T,3> v, T w){
    return VectorBase4<T>(v[0],v[1],v[2],w);
}


};

#endif
