/**
 * @brief This file contains the declaration of the Bounding Cube class.
 * @file BoundingCube.h
 * @author Skyler Saleh, Ian Wiggins
 **/

#ifndef Neon_Rush_ARBoundingBox_h
#define Neon_Rush_ARBoundingBox_h
#include "Vector.h"
#include "Matrix.h"
#include "TypeInfo.h"

namespace Argon {
/**
 * @brief A run of the mill bounding cube class.
 * @details An AABB is used for various things in the engine as an efficient collision primitive,
 * such as the broad phase of collision detection or oclussion culling.
 **/
struct BoundingCube : public ReflectionBase{
    Vector3f origin; ///The origin point of the bounding cube.
    Vector3f size;   ///The size of the bounding cube.
    BoundingCube() : origin(0,0,0),size(-1,0,0){}
    BoundingCube(Vector3f origin, Vector3f size) : origin(origin),size(size){}

    bool operator==(const BoundingCube &b) const {
        return origin==b.origin&&size==b.size;
    }
    bool operator!=(const BoundingCube &b) const {
        return !operator==(b);
    }
    ///Returns true if the bounding cube has been assigned a valid range.
    bool is_valid(){return size[0]!=-1.;}
    ///Returns true if the bounds overlaps the point
    bool overlaps_point(Vector3f point) const;
    ///Returns true if the bounds overlaps the point
    bool overlaps_point(Vector2f point) const;

    ///Extend the bounding cube by adding a point to it.
    void add_point_to_bounds(const VectorBase<float, 3>& point);
    ///Get the closeset vertex of the cube in a specific direction
    Vector3f get_positive_vertex(Vector3f normal)const;
    ///Get the furthest vertex of the cube in a specific direction
    Vector3f get_negative_vertex(Vector3f normal)const;
    ///Returns a new bounding cube that contains this bounding cube when it is transformed by a matrix.
    BoundingCube transform(const Matrix4f &m);
    MAKE_VISIT_HEAD(BoundingCube)
};

};


#endif
