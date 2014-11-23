#include <config.h>

/**
 * @brief Matrix classes and manipulations.
 * @file Matrix.h
 * @author Skyler Saleh, Ian Wiggins
 **/

#ifndef ArgonEngine_Matrix2_h
#define ArgonEngine_Matrix2_h

#include "Vector.h"
namespace Argon {
template <typename T, size_t TSize> struct MatrixBase {
    T data[TSize*TSize];
    T* get_data(){
        return data;
    }
    T& operator()(size_t x, size_t y ){
        return data[x+y*TSize];
    }
    const T& operator() (size_t x, size_t y )const{
        return data[x+y*TSize];
    }

    MatrixBase(const MatrixBase & m){
        for(size_t x=0; x<TSize*TSize; ++x) data[x]=m.data[x];
    }
    MatrixBase() {
        for(size_t x=0; x<TSize*TSize; ++x) data[x]=!(x%(TSize+1));
    }

    INLINE const T operator[](const size_t s) const {
        return data[s];
    }
    INLINE T& operator[](size_t s){
        return data[s];
    }
    INLINE VectorBase<T, TSize> row(size_t r) const {
        VectorBase<T,TSize> v;
        for(size_t x =0; x<TSize; ++x) v[x]=operator[](x+r*TSize);
        return v;
    }
    INLINE VectorBase<T, TSize> col(size_t c) const {
        VectorBase<T,TSize> v;
        for(size_t x =0; x<TSize; ++x) v[x]=operator[](x*TSize+c);
        return v;
    }

};
template <typename T> struct MatrixBase4 : public MatrixBase<T, 4>{
    MatrixBase4() : MatrixBase<T, 4>(){}
    template<typename T2>
    MatrixBase4(const MatrixBase<T2,4> & m) : MatrixBase<T,4>(m){}

    MatrixBase4(T m0,T m1,T m2,T m3,T m4,T m5,T m6,T m7,T m8,T m9,T m10,T m11, T m12,T m13,T m14,T m15){
        T *f = this->data;
        f[0]=m0;
        f[1]=m1;
        f[2]=m2;
        f[3]=m3;
        f[4]=m4;
        f[5]=m5;
        f[6]=m6;
        f[7]=m7;
        f[8]=m8;
        f[9]=m9;
        f[10]=m10;
        f[11]=m11;
        f[12]=m12;
        f[13]=m13;
        f[14]=m14;
        f[15]=m15;

    }

    bool inverse(MatrixBase4&inv)
    {
        T *m = this->data;
        T det=m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

        if (det == 0) return false;
        det = 1.0f / det;

        inv[0] = (m[5]  * m[10] * m[15] -
                m[5]  * m[11] * m[14] -
                m[9]  * m[6]  * m[15] +
                m[9]  * m[7]  * m[14] +
                m[13] * m[6]  * m[11] -
                m[13] * m[7]  * m[10])*det;

        inv[4] = (-m[4]  * m[10] * m[15] +
                m[4]  * m[11] * m[14] +
                m[8]  * m[6]  * m[15] -
                m[8]  * m[7]  * m[14] -
                m[12] * m[6]  * m[11] +
                m[12] * m[7]  * m[10])*det;

        inv[8] = (m[4]  * m[9] * m[15] -
                m[4]  * m[11] * m[13] -
                m[8]  * m[5] * m[15] +
                m[8]  * m[7] * m[13] +
                m[12] * m[5] * m[11] -
                m[12] * m[7] * m[9])*det;

        inv[12] = (-m[4]  * m[9] * m[14] +
                m[4]  * m[10] * m[13] +
                m[8]  * m[5] * m[14] -
                m[8]  * m[6] * m[13] -
                m[12] * m[5] * m[10] +
                m[12] * m[6] * m[9])*det;

        inv[1] = (-m[1]  * m[10] * m[15] +
                m[1]  * m[11] * m[14] +
                m[9]  * m[2] * m[15] -
                m[9]  * m[3] * m[14] -
                m[13] * m[2] * m[11] +
                m[13] * m[3] * m[10])*det;

        inv[5] = (m[0]  * m[10] * m[15] -
                m[0]  * m[11] * m[14] -
                m[8]  * m[2] * m[15] +
                m[8]  * m[3] * m[14] +
                m[12] * m[2] * m[11] -
                m[12] * m[3] * m[10])*det;

        inv[9] = (-m[0]  * m[9] * m[15] +
                m[0]  * m[11] * m[13] +
                m[8]  * m[1] * m[15] -
                m[8]  * m[3] * m[13] -
                m[12] * m[1] * m[11] +
                m[12] * m[3] * m[9])*det;

        inv[13] = (m[0]  * m[9] * m[14] -
                m[0]  * m[10] * m[13] -
                m[8]  * m[1] * m[14] +
                m[8]  * m[2] * m[13] +
                m[12] * m[1] * m[10] -
                m[12] * m[2] * m[9])*det;

        inv[2] =(m[1]  * m[6] * m[15] -
                m[1]  * m[7] * m[14] -
                m[5]  * m[2] * m[15] +
                m[5]  * m[3] * m[14] +
                m[13] * m[2] * m[7] -
                m[13] * m[3] * m[6])*det;

        inv[6] = (-m[0]  * m[6] * m[15] +
                m[0]  * m[7] * m[14] +
                m[4]  * m[2] * m[15] -
                m[4]  * m[3] * m[14] -
                m[12] * m[2] * m[7] +
                m[12] * m[3] * m[6])*det;

        inv[10] = (m[0]  * m[5] * m[15] -
                m[0]  * m[7] * m[13] -
                m[4]  * m[1] * m[15] +
                m[4]  * m[3] * m[13] +
                m[12] * m[1] * m[7] -
                m[12] * m[3] * m[5])*det;

        inv[14] = (-m[0]  * m[5] * m[14] +
                m[0]  * m[6] * m[13] +
                m[4]  * m[1] * m[14] -
                m[4]  * m[2] * m[13] -
                m[12] * m[1] * m[6] +
                m[12] * m[2] * m[5])*det;

        inv[3] = (-m[1] * m[6] * m[11] +
                m[1] * m[7] * m[10] +
                m[5] * m[2] * m[11] -
                m[5] * m[3] * m[10] -
                m[9] * m[2] * m[7] +
                m[9] * m[3] * m[6])*det;

        inv[7] = (m[0] * m[6] * m[11] -
                m[0] * m[7] * m[10] -
                m[4] * m[2] * m[11] +
                m[4] * m[3] * m[10] +
                m[8] * m[2] * m[7] -
                m[8] * m[3] * m[6])*det;

        inv[11] = (-m[0] * m[5] * m[11] +
                m[0] * m[7] * m[9] +
                m[4] * m[1] * m[11] -
                m[4] * m[3] * m[9] -
                m[8] * m[1] * m[7] +
                m[8] * m[3] * m[5])*det;

        inv[15] = (m[0] * m[5] * m[10] -
                m[0] * m[6] * m[9] -
                m[4] * m[1] * m[10] +
                m[4] * m[2] * m[9] +
                m[8] * m[1] * m[6] -
                m[8] * m[2] * m[5])*det;


        return true;
    }
};
template <typename T> MatrixBase4<T> transpose(const MatrixBase4<T>& m){
    return MatrixBase4<T>(m[0],m[4],m[8],m[12],m[1],m[5],m[9],m[13],m[2],m[6],m[10],m[14],m[3],m[7],m[11],m[15]);
}
template <typename T> struct MatrixBase3 : public MatrixBase<T, 3>{
    MatrixBase3() : MatrixBase<T, 3>(){}
    template<typename T2>
    MatrixBase3(const MatrixBase<T2,3> & m) : MatrixBase<T,3>(m){}
    MatrixBase3(T m0,T m1,T m2,T m3,T m4,T m5,T m6,T m7,T m8,T m9){
        T *f = this->data;
        f[0]=m0;
        f[1]=m1;
        f[2]=m2;
        f[3]=m3;
        f[4]=m4;
        f[5]=m5;
        f[6]=m6;
        f[7]=m7;
        f[8]=m8;
        f[9]=m9;

    }
};
template <typename T> struct MatrixBase2 : public MatrixBase<T, 2>{
    MatrixBase2() : MatrixBase<T, 2>(){}
    template<typename T2>
    MatrixBase2(const MatrixBase<T2,2> & m) : MatrixBase<T,2>(m){}
    MatrixBase2(T m0,T m1,T m2,T m3){
        T *f = this->data;
        f[0]=m0;
        f[1]=m1;
        f[2]=m2;
        f[3]=m3;

    }
};
typedef MatrixBase4<float> Matrix4f;
typedef MatrixBase3<float> Matrix3f;
typedef MatrixBase2<float> Matrix2f;

typedef MatrixBase4<double> Matrix4d;
typedef MatrixBase3<double> Matrix3d;
typedef MatrixBase2<double> Matrix2d;
template<typename T, size_t size>
struct TypeInfo<MatrixBase<T,size> >{
    enum {valid=1};
    static const char* get_name(){
        return "Matrix";
    }
    static void handle(MatrixBase<T,size>& v, Visitor &vis){
        for(int x=0; x<size*size; ++x) HANDLE_WITH_PICKER(vis,v[x]);
    }

};

#define MAKE_MATRIX_TYPE_INFO(MatrixType,Name,Count)\
    template <typename T>\
    struct TypeInfo<MatrixType<T> >{\
    enum {valid=1};\
    static const char* get_name(){\
    return Name;\
    }\
    static void handle(MatrixType<T>& v,Visitor &vis){\
    for(int x=0; x<Count*Count; ++x) HANDLE_WITH_PICKER(vis,v[x]);\
    }\
    };

MAKE_MATRIX_TYPE_INFO(MatrixBase4,"Matrix4",4)

MAKE_MATRIX_TYPE_INFO(MatrixBase3,"Matrix3",3)

MAKE_MATRIX_TYPE_INFO(MatrixBase2,"Matrix2",2)

template<typename T>struct TranslationMatrixCookie {
    const VectorBase<T, 3> data;
    TranslationMatrixCookie(const VectorBase<T,3> d) : data(d){}
};
template<typename T>struct ScaleMatrixCookie {VectorBase<T, 3> data; ScaleMatrixCookie(const VectorBase<T,3> d) : data(d){}
                                             };

template<typename T,size_t TSize>const TranslationMatrixCookie<T> TranslateMatrix(const VectorBase<T, TSize> v){
    return TranslationMatrixCookie<T>(v);
}

template<typename T>const ScaleMatrixCookie<T> ScaleMatrix(const VectorBase<T, 3> v){
    return ScaleMatrixCookie<T>(v);
}
template<typename T>const ScaleMatrixCookie<T> ScaleMatrix(const VectorBase<T, 2> v){
    return ScaleMatrixCookie<T>(VectorBase<T,3>(v[0],v[1],1.,1.));
}

template <typename T,typename T2>
INLINE const MatrixBase<T,2> operator *(const MatrixBase<T,2> a, const TranslationMatrixCookie<T2> b){
    return MatrixBase<T,2>(
                a[0]+a[0]*b.data[0]+a[2]*b.data[1],
            a[1]+a[1]*b.data[0]+a[3]*b.data[1],
            a[2]+a[0]*b.data[0]+a[2]*b.data[1],
            a[3]+a[1]*b.data[0]+a[3]*b.data[1]
            );
}

template <typename T,typename T2>
INLINE const MatrixBase<T,4> operator *(MatrixBase<T,4> a, const TranslationMatrixCookie<T2> b){
    a[12]+=a[0]*b.data[0]+a[4]*b.data[1]+a[8]*b.data[2];
    a[13]+=a[1]*b.data[0]+a[5]*b.data[1]+a[9]*b.data[2];
    a[14]+=a[2]*b.data[0]+a[6]*b.data[1]+a[10]*b.data[2];
    a[15]+=a[3]*b.data[0]+a[7]*b.data[1]+a[11]*b.data[2];

    /*const T c1=a[0]*b.data[0]+a[1]*b.data[1]+a[2]*b.data[2];
         const T c2=a[4]*b.data[0]+a[5]*b.data[1]+a[6]*b.data[2];
         const T c3=a[8]*b.data[0]+a[9]*b.data[1]+a[10]*b.data[2];
         const T c4=a[12]*b.data[0]+a[13]*b.data[1]+a[14]*b.data[2];*/

    return a;
}
template <typename T,typename T2>
INLINE const MatrixBase<T,3> operator *(const MatrixBase<T,3> a, const TranslationMatrixCookie<T2> b){
    const T c1=a[0]*b.data[0]+a[3]*b.data[1];
    const T c2=a[1]*b.data[0]+a[4]*b.data[1];

    return MatrixBase<T,3>(
                a[0], a[1], a[2],
            a[3], a[4], a[5],
            a[6]+c1, a[7]+c2, a[8]
            );
}
template <typename T,typename T2>
INLINE const MatrixBase<T,4> operator *(const MatrixBase<T,4> a, const ScaleMatrixCookie<T2> b){
    /*return MatrixBase4<T>(
         a[0]*b.data[0], a[4]*b.data[1], a[8]*b.data[2], a[12],
         a[1]*b.data[0], a[5]*b.data[1], a[9]*b.data[2], a[13],
         a[2]*b.data[0], a[6]*b.data[1], a[10]*b.data[2], a[14],
         a[3]*b.data[0], a[7]*b.data[1], a[11]*b.data[2], a[15]
         );*/
    return MatrixBase4<T>(
                a[0]*b.data[0], a[1]*b.data[0], a[2]*b.data[0], a[3]*b.data[0],
            a[4]*b.data[1], a[5]*b.data[1], a[6]*b.data[1], a[7]*b.data[1],
            a[8]*b.data[2], a[9]*b.data[2], a[10]*b.data[2], a[11]*b.data[2],
            a[12], a[13], a[14], a[15]
            );
}
template <typename T,size_t TSize>
INLINE const MatrixBase<T,TSize> operator *(T b,const MatrixBase<T,TSize> a1){
    MatrixBase<T,TSize> a = a1;
    for(int i=0;i<TSize*TSize;++i)a[i]*=b;
    return a;
}
template <typename T,size_t TSize>
INLINE const MatrixBase<T,TSize> operator *(const MatrixBase<T,TSize> a1, T b){
    MatrixBase<T,TSize> a = a1;

    for(int i=0;i<TSize*TSize;++i)a[i]*=b;
    return a;
}

template <typename T>
INLINE const MatrixBase<T,2> operator *(const MatrixBase<T,2> a, const MatrixBase<T, 2> b){

    return MatrixBase<T,2>(
                a[0]*b[0]+a[1]*b[2],
            a[0]*b[1]+a[1]*b[3],
            a[2]*b[0]+a[3]*b[2],
            a[2]*b[1]+a[3]*b[3]
            );
}
template <typename T>
INLINE const MatrixBase<T,3> operator *(const MatrixBase<T,3> a, const MatrixBase<T, 3> b){
    return MatrixBase<T,3>(
                a[0]*b[0]+a[1]*b[3]+a[2]*b[6],
            a[0]*b[1]+a[1]*b[4]+a[2]*b[7],
            a[0]*b[2]+a[1]*b[5]+a[2]*b[8],

            a[3]*b[0]+a[4]*b[3]+a[5]*b[6],
            a[3]*b[1]+a[4]*b[4]+a[5]*b[7],
            a[3]*b[2]+a[4]*b[5]+a[5]*b[8],

            a[6]*b[0]+a[7]*b[3]+a[8]*b[6],
            a[6]*b[1]+a[7]*b[4]+a[8]*b[7],
            a[6]*b[2]+a[7]*b[5]+a[8]*b[8]
            );
}
template <typename T>
INLINE const MatrixBase<T,4> operator *(const MatrixBase<T,4> b, const MatrixBase<T, 4> a){
    return MatrixBase4<T>(
                a[0]*b[0]+a[1]*b[4]+a[2]*b[8]+a[3]*b[12],
            a[0]*b[1]+a[1]*b[5]+a[2]*b[9]+a[3]*b[13],
            a[0]*b[2]+a[1]*b[6]+a[2]*b[10]+a[3]*b[14],
            a[0]*b[3]+a[1]*b[7]+a[2]*b[11]+a[3]*b[15],

            a[4]*b[0]+a[5]*b[4]+a[6]*b[8]+a[7]*b[12],
            a[4]*b[1]+a[5]*b[5]+a[6]*b[9]+a[7]*b[13],
            a[4]*b[2]+a[5]*b[6]+a[6]*b[10]+a[7]*b[14],
            a[4]*b[3]+a[5]*b[7]+a[6]*b[11]+a[7]*b[15],

            a[8]*b[0]+a[9]*b[4]+a[10]*b[8]+a[11]*b[12],
            a[8]*b[1]+a[9]*b[5]+a[10]*b[9]+a[11]*b[13],
            a[8]*b[2]+a[9]*b[6]+a[10]*b[10]+a[11]*b[14],
            a[8]*b[3]+a[9]*b[7]+a[10]*b[11]+a[11]*b[15],

            a[12]*b[0]+a[13]*b[4]+a[14]*b[8]+a[15]*b[12],
            a[12]*b[1]+a[13]*b[5]+a[14]*b[9]+a[15]*b[13],
            a[12]*b[2]+a[13]*b[6]+a[14]*b[10]+a[15]*b[14],
            a[12]*b[3]+a[13]*b[7]+a[14]*b[11]+a[15]*b[15]
            );
}
template <typename T>
INLINE VectorBase3<T> get_position(MatrixBase<T, 4>&m){
    return VectorBase3<T>(m[12],m[13],m[14]);
}
template <typename T>
INLINE VectorBase3<T> get_scale(MatrixBase<T, 4>&m){
    VectorBase3<T> xs(m[0],m[1],m[2]);
    VectorBase3<T> ys(m[4],m[5],m[6]);
    VectorBase3<T> zs(m[8],m[9],m[10]);
    return VectorBase3<T>(length(xs),length(ys),length(zs));
}

template <typename T, size_t TSize>
INLINE MatrixBase<T,TSize> operator *(const MatrixBase<T,TSize>& a, const MatrixBase<T, TSize>& b){
    MatrixBase<T,TSize> m;
    for(size_t x=0; x<TSize*TSize; ++x) {
        m[x]=dot(a.row(x/TSize),b.col(x%TSize));
    }
    return m;
}
template <typename T>
INLINE VectorBase<T, 4> operator *(const MatrixBase<T,4>& a, const VectorBase<T, 4> & b){
    return VectorBase4<T>(
                a[0]*b[0]+a[4]*b[1]+a[8]*b[2]+a[12]*b[3],
            a[1]*b[0]+a[5]*b[1]+a[9]*b[2]+a[13]*b[3],
            a[2]*b[0]+a[6]*b[1]+a[10]*b[2]+a[14]*b[3],
            a[3]*b[0]+a[7]*b[1]+a[11]*b[2]+a[15]*b[3]);
}
template <typename T>
INLINE VectorBase<T, 3> operator *(const MatrixBase<T,3>& a, const VectorBase<T, 3> & b){
    return VectorBase<T, 3>(
                a[0]*b[0]+a[1]*b[1]+a[2]*b[2],
            a[0]*b[4]+a[1]*b[5]+a[2]*b[6],
            a[0]*b[8]+a[1]*b[9]+a[2]*b[10]);
}
template <typename T>
INLINE VectorBase<T, 2> operator *(const MatrixBase<T,2>& a, const VectorBase<T, 2> & b){
    return VectorBase<T, 2>(a[0]*b[0]+a[1]*b[1],a[0]*b[4]+a[1]*b[5]);
}
template <typename T, size_t TSize>
INLINE VectorBase<T, TSize> operator *(const MatrixBase<T,TSize>& a, const VectorBase<T, TSize> & b){
    VectorBase<T,TSize> v;
    for(size_t x=0; x<TSize; ++x) {
        v[x]=dot(a.row(x),b);
    }
    return v;
}

template<typename T>
static const MatrixBase<T, 4> IdentityMatrix(){
    return MatrixBase<T, 4>();
}

template<typename T,typename T1,typename T2,typename T3>
static MatrixBase<T, 4> LookAtMatrix(const VectorBase<T1, 3>& eye_position, const VectorBase<T2, 3>& center,const VectorBase<T3, 3>& up){
    VectorBase<T,3> forward = normalize(center-eye_position);
    VectorBase<T,3> side = normalize(cross(forward, up));
    VectorBase<T,3> upv = cross(side, forward);
    MatrixBase<T, 4> m;
    m[0] = side[0];
    m[4] = side[1];
    m[8] = side[2];
    m[12] = 0.0;

    m[1] = upv[0];
    m[5] = upv[1];
    m[9] = upv[2];
    m[13] = 0.0;

    m[2] = -forward[0];
    m[6] = -forward[1];
    m[10] = -forward[2];
    m[14] = 0.0;

    m[3]=m[7]=m[11]=0.0;
    m[15]= 1.0f;
    return m*TranslateMatrix(eye_position* -1.0f);
}
template<typename T>
static MatrixBase<T, 4> FrustumMatrix(T left, T right, T bottom, T top,
                                      T znear, T zfar)
{

    T a = (right+left)/(right-left);
    T b = (top+bottom)/(top-bottom);
    T c = (zfar+znear)/(znear-zfar);
    T d = (2.0f*zfar*znear)/(znear-zfar);

    MatrixBase<T, 4> m;

    m[0] = (2.0f*znear)/(right-left);
    m[1] = 0;
    m[2] = 0;
    m[3] = 0;

    m[4] = 0;
    m[5] = (2.0f*znear)/(top-bottom);
    m[6] = 0;
    m[7] = 0;

    m[8] = a;
    m[9] = b;
    m[10] = c;
    m[11] = -1;

    m[12] = 0;
    m[13] = 0;
    m[14] = d;
    m[15] = 0;
    return m;
}
template<typename T>
static MatrixBase<T, 4> PerspectiveMatrix(T field_of_view,T aspect,T zNear, T zFar)
{
    T ymax, xmax;
    ymax = zNear * tanf(field_of_view* 3.14159f / 180.0f);

    xmax = ymax * aspect;

    return FrustumMatrix(-xmax, xmax, -ymax, ymax, zNear, zFar);
}

template<typename T>
static MatrixBase<T, 4> OrthoMatrix(T right, T left, T top, T bottom, T near, T far)
{
    MatrixBase<T, 4> matrix;
    matrix[0] = 2.0f/(right-left);
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = -((right+left)/(right-left));

    matrix[4] = 0.0f;
    matrix[5] = 2.0f/(top-bottom);
    matrix[6] = 0.0f;
    matrix[7] = -((top+bottom)/(top-bottom));

    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = -2.0f/(far-near);
    matrix[11] = -((far+near)/(far-near));

    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;
    return matrix;
}
template<typename T>
static INLINE MatrixBase<T, 3> mat3(const MatrixBase<T, 4>& m)
{
    return MatrixBase<T, 3>(m[0],m[1],m[2],m[4],m[5],m[6],m[8],m[9],m[10]);
}

};
template <typename T,typename TType, size_t TSize> T & operator<<(T&stream, Argon::MatrixBase<TType, TSize>&m){
    stream<<"Matrix[\n";
    for(int i = 0;i<TSize*TSize;++i){
        stream<<m.data[i];
        if(i+1!=TSize*TSize)stream<<",";
        if(!((i+1)%TSize))stream<<"\n";
    }
    stream<<"]";
    return stream;

}

#endif
