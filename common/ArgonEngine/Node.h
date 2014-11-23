/**
 * @brief This file declares all the classes used in the Render System.
 * @file Node.h
 * @author Skyler Saleh
 **/
#ifndef ARNode_DEF
#define ARNode_DEF 1

#include "TypeInfo.h"
#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"
#include "Utility.h"
#include "BoundingCube.h"

namespace Argon
{
struct BoundsRenderer;
/**
 * @brief The Node class is the base building block for almost all in game objects.
 * @details
 * While using the Node class is not required for any functionality of the engine,
 * it provides alot of useful functionality, such as handling transformations,
 * and managing the scene graph.
 **/
struct Node: public ReflectionBase{

    Node* parent=NULL;
    StringIntern name;
    Vector3f position=Vector3f(0,0,0);
    Quaternionf rotation=Quaternionf();
    Vector3f anchor=Vector3f(0,0,0);
    Vector3f scale=Vector3f(1,1,1);
    Vector3f dimensions=Vector3f(1,1,1);
    Vector4f color=Vector4f(1,1,1,1);

    bool should_render=true;
    int layer = 0;
    BoundingCube bounds=BoundingCube(Vector3f(-0.5,-0.5,-0.5), Vector3f(1.,1,.1));
    float &red(){return color[0];}
    float &green(){return color[1];}
    float &blue(){return color[2];}
    float &alpha(){return color[3];}

    float &xPosition() {return position[0];}
    float &yPosition() {return position[1];}
    float &zPosition() {return position[2];}

    float &xScale(){return scale[0];}
    float &yScale(){return scale[1];}
    float &zScale(){return scale[2];}

    float &xDimensions() {return dimensions[0];}
    float &yDimensions() {return dimensions[1];}
    float &zDimensions() {return dimensions[2];}

    bool overlaps_screen_coord(Vector2f coord);
    virtual std::shared_ptr<Node> animate(float dt){
        return nullptr;
    }
    virtual void draw(){return;}

    bool get_should_render(){return should_render? parent? parent->get_should_render() : true :false;}
    Vector4f get_color(){return parent? color*parent->get_color():color;}
    int get_layer(){return parent?parent->get_layer()+layer:layer;}
    Node()=default;
    Node(const Node &t):Node(){operator=(t);}
    Node & operator=(const Node &t);

    virtual void visit(Visitor &v) override;
    Matrix4f world_matrix();

    Matrix4f object_matrix();
    Matrix4f get_camera_projection();
    Matrix4f get_camera_view();
    Matrix4f get_camera_inverse_view();
    Matrix4f projection_matrix();
    Matrix4f view_matrix();

    Matrix4f object_matrix_inverse();

    Matrix4f world_matrix_inverse();
    Matrix4f view_matrix_inverse();

    Matrix4f object_normal_matrix();
    Matrix4f world_normal_matrix();
    Matrix4f view_normal_matrix();
    Vector3f screen_to_local(Vector3f p);
    Vector3f world_to_local(Vector3f p);
    Vector3f parent_to_local(Vector3f p);

};
}
#endif
