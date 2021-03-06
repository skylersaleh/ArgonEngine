//Generated by the Argon Build System

//
//  Collision.h
//  ArgonEngine
//
//  Created by Skyler Saleh on 9/21/12.
//  Copyright (c) 2012 Argon Software. All rights reserved.
//

#ifndef __ArgonEngine__Collision__
#define __ArgonEngine__Collision__

#include "Vector.h"
namespace Argon {
//Returns the point closest to "point" on the line containing point "lineA" and point "lineB"
//If the line is a ray clamp the point that defines the origin.
//If the line is a segment clamp both points(default).
template <typename T, size_t TComps>
VectorBase<T,TComps> closest_point_on_line(const VectorBase<T, TComps>& lineA,const VectorBase<T, TComps>& lineB,const VectorBase<T, TComps>&point, bool clamp_a=true,bool clamp_b=true)
{
    VectorBase<T,TComps> ap=point-lineA;
    VectorBase<T,TComps> ab=lineB-lineA;
    T delta=dot(ap,ab)/dot(ab, ab);
    if(clamp_a&& delta < 0.0) delta=0.0;
    if(clamp_b&& delta>1.0) delta=1.0;
    return lineA+ab*delta;
}
    template <typename T>
    bool ray_intersects_triangle(
                                 const VectorBase<T, 3>& v0,
                                 const VectorBase<T, 3>& v1,
                                 const VectorBase<T, 3>& v2,
                                 const VectorBase<T, 3>& pos,
                                 const VectorBase<T, 3>&dir)
    {
        typedef const VectorBase<T, 3> vect_type;

        //Shift origin
        vect_type e1= v1-v0;
        vect_type e2= v2-v0;
        vect_type s = pos-v0;

        vect_type h = cross(dir,e2);
        vect_type q = cross(s,e1);

        const T f = 1.0/dot(e1,h);

        //Calculate Point on plane
        const T u = f*dot(s,h);
        const T v = f*dot(dir,q);
        const T t = f*dot(e2,q);

        //Check if point is in triangle and ray
        return t>0.0 && u+v<f && u>0.0 && v>0.0  ;
    }
    template <typename T>
    T point_plane_distance(VectorBase<T, 3> plane, T distance, VectorBase<T, 3> point){
        return dot(plane,point)+distance;
    }


};
#endif /* defined(__ArgonEngine__Collision__) */
