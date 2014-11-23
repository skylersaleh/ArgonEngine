#include <config.h>

/**
 * @brief Quaternion class and manipulation.
 * @file Quaternion.h
 * @author Skyler Saleh, Ian Wiggins
 **/

#ifndef ArgonEngine_Quaternion2_h
#define ArgonEngine_Quaternion2_h
#include "Matrix.h"
#include "Vector.h"
#include "HandlePicker.h"
namespace Argon {
    template<typename T>
    struct Quaternion : public VectorBase4<T>{
        typedef VectorBase4<T> base;
        Quaternion(){identity();}
        Quaternion(T x, T y, T z,T w){set_quaternion(x,y,z,w);}
        Quaternion(const T angle, const VectorBase<T, 3> &axis){
            set_angle_axis(angle,axis);
        }
        Quaternion(const VectorBase<T, 4>&c) : base(c){}
        void identity(){base::set(0,0,0,1);}
        Quaternion inverse(){
            const Quaternion q =normalize(*this);
            return normalize(Quaternion(-q[0],-q[1],-q[2],q[3]));
        }
        void zero(){base::set(0,0,0,0);}
        void set_angle_axis(T angle, VectorBase<T, 3> axis);
        void set_euler_xyz(T x, T y,T z);
        void set_quaternion(T x,T y, T z,T w);
        VectorBase<T,3> transform(VectorBase<T,3> p)const{
                    VectorBase<T,3> v3(const_vector());
                    VectorBase<T,3> t = cross(v3,p)*2.f;
                    return p+base::data[3]*t+cross(v3,t);
                }
        Quaternion& operator*=(const Quaternion& o);
        Quaternion operator*(const Quaternion o) const {
            return normalize(Quaternion(*this)*=o);
        }
        VectorBase<T, 4>& vector(){return *this;}
        const VectorBase<T, 4>& const_vector() const {return *this;}
        Quaternion operator-(){
            return Quaternion(-base::operator[](0),-base::operator[](1),-base::operator[](2),base::operator[](3));
        }

    };
    typedef Quaternion<float> Quaternionf;
    typedef Quaternion<double> Quaterniond;
    template<typename T>
    struct TypeInfo<Quaternion<T> >{
        enum {valid=1};
        static const char* get_name(){
            return "Quaternion";
        }
        static void handle(Quaternion<T>& v, Visitor &vis){
            for(int x=0; x<4; ++x) HANDLE_WITH_PICKER(vis,v[x]);
        }

    };
    template<typename T>
    void Quaternion<T>::set_angle_axis(T angle, VectorBase<T, 3> axis){
        axis= Argon::normalize(axis)*sinf(angle*0.5f);

        base::operator[](0)=axis[0];
        base::operator[](1)=axis[1];
        base::operator[](2)=axis[2];
        base::operator[](3)=cosf(angle*0.5f);

        *this=normalize(*this);
    }
    template<typename T>
    void Quaternion<T>::set_euler_xyz(T x, T y,T z){
        Quaternion q1;
        Quaternion q2;
        set_angle_axis( x, VectorBase3<T>(1,0,0));
        q1.set_angle_axis(y, VectorBase3<T>(0,1,0));
        q2.set_angle_axis(z, VectorBase3<T>(0,0,1));
        *this*=Argon::normalize(q1);
        *this*=Argon::normalize(q2);
        *this=Argon::normalize(*this);
    }
    template<typename T>
    void Quaternion<T>::set_quaternion(T x,T y, T z,T w){
        base::set(x, y, z, w);
    }
    template<typename T>
    Quaternion<T>& Quaternion<T>::operator*=(const Quaternion& o){

        T x= base::operator[](0);
        T y= base::operator[](1);
        T z= base::operator[](2);
        T w= base::operator[](3);
        T rqx= o[0];
        T rqy= o[1];
        T rqz= o[2];
        T rqw= o[3];
        base::operator=(Argon::normalize(base(w * rqx + x * rqw + y * rqz - z * rqy,
                                              w * rqy + y * rqw + z * rqx - x * rqz,
                                              w * rqz + z * rqw + x * rqy - y * rqx,
                                              w * rqw - x * rqx - y * rqy - z * rqz)));
        return *this;
    }
    template<typename T>
    static inline Quaternion<T> slerp(const Quaternion<T> &a, const Quaternion<T> &b, float t)
    {

        T flip = 1.0f;

        T cosine = dot(a, b);

        if (cosine<0.0f)
        {
            cosine = -cosine;
            flip = -1.0f;
        }

        if ((1.0f-cosine)<0.000001f)
            return Argon::normalize(Quaternion<T>(a.const_vector() * (1.0f-t) + b.const_vector() * (t*flip)));

        T theta = (T)acos(cosine);
        T sine = (T)sin(theta);
        T beta = (T)sin((1.0-t)*theta) / sine;
        T alpha = (T)sin(t*theta) / sine * flip;

        return Argon::normalize(Quaternion<T>(a.const_vector() * beta + b.const_vector() * alpha));
    }

    template <typename T>
    struct RotationMatrixCookie {
        RotationMatrixCookie(const Quaternion<T>&d) : data(d.const_vector()){}
        const VectorBase<T, 4>& data;
    };
    template<typename T>
    static INLINE RotationMatrixCookie<T> RotateMatrix(const Quaternion<T>& v){
        return RotationMatrixCookie<T >(v);
    }
    template <typename T1, typename TV>
    const MatrixBase<T1, 4> operator*(const MatrixBase<T1,4>& m, const RotationMatrixCookie<TV> &q){
        const VectorBase<T1,4> fT  = q.data*2.0f;


        const T1 fTx0 = fT[0]*q.data[0];
        const T1 fTx1 = fT[1]*q.data[0];
        const T1 fTx2 = fT[2]*q.data[0];

        const T1 fTy1 = fT[1]*q.data[1];
        const T1 fTy2 = fT[2]*q.data[1];

        const T1 fTz2 = fT[2]*q.data[2];

        const T1 fTw0 = fT[0]*q.data[3];
        const T1 fTw1 = fT[1]*q.data[3];
        const T1 fTw2 = fT[2]*q.data[3];

        const T1 a = 1.0f-(fTy1+fTz2);
        const T1 b = fTx1-fTw2;
        const T1 c = fTx2+fTw1;

        const T1 d =  fTx1+fTw2;
        const T1 e = 1.0f-(fTx0+fTz2);

        const T1 g = fTx2-fTw1;
        const T1 f = fTy2-fTw0;
        const T1 h = fTy2+fTw0;
        const T1 i = 1.0f-(fTx0+fTy1);

        const VectorBase4<T1> m1(m[0],m[1],m[2],m[3]);
        const VectorBase4<T1> m2(m[4],m[5],m[6],m[7]);
        const VectorBase4<T1> m3(m[8],m[9],m[10],m[11]);

        const VectorBase<T1, 4> r1 = m1*a+m2*b+m3*c;
        const VectorBase<T1, 4> r2 = m1*d+m2*e+m3*f;
        const VectorBase<T1, 4> r3 = m1*g+m2*h+m3*i;
        return MatrixBase4<T1>(
                               r1[0],r1[1],r1[2],r1[3],
                               r2[0],r2[1],r2[2],r2[3],
                               r3[0],r3[1],r3[2],r3[3],
                               m[12],m[13],m[14],m[15]
                               );


    }
    template<typename T>
    INLINE Quaternion<T> get_rotation(MatrixBase<T, 4>&m){
        VectorBase3<T> scale_val = get_scale(m);
        float xr = atan2f(m[9]/scale_val[2], m[10]/scale_val[2]);
        float yr = -asin(m[8]/scale_val[2]);
        float zr = atan2f(m[4]/scale_val[1], m[0]/scale_val[0]);
        Quaternion<float> q;
        Quaternion<float> q1;
        Quaternion<float> q2;

        q.set_euler_xyz(xr, 0, 0);
        q1.set_euler_xyz(0, yr, 0);
        q2.set_euler_xyz(0, 0, zr);

        return normalize(q2*q1*q);
    }

};

#endif
