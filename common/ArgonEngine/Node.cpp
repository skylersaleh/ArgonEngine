/**
 * @brief Implementation of the Node, the base for all renderable objects.
 * @file Node.cpp
 * @author Skyler Saleh, Ian Wiggins
 **/
#include "Node.h"
#include "RenderSystem.h"
#include "BasicShape.h"
#include "HandlePicker.h"

namespace Argon {

bool Node::overlaps_screen_coord(Vector2f coord){
    Matrix4f m = projection_matrix();
    BoundingCube c = bounds.transform(m);
    return c.overlaps_point(coord);
}

Node &Node::operator=(const Node &t){
    parent = t.parent;
    position=t.position;
    rotation=t.rotation;
    anchor=t.anchor;
    scale=t.scale;
    dimensions=t.dimensions;
    color=t.color;
    should_render=t.should_render;
    layer=t.layer;
    bounds = t.bounds;
    return *this;
}

void Node::visit(Visitor &v){
    v.type_name="Node";
    if(v.begin_map()){
        HANDLE_KEY_WITH_PICKER(v,position,"position");
        HANDLE_KEY_WITH_PICKER(v,rotation,"rotation");
        HANDLE_KEY_WITH_PICKER(v,anchor,"anchor");
        HANDLE_KEY_WITH_PICKER(v,scale,"scale");
        HANDLE_KEY_WITH_PICKER(v,dimensions,"dimensions");
        HANDLE_KEY_WITH_PICKER(v,color,"color");
        HANDLE_KEY_WITH_PICKER(v,should_render,"should_render");
        HANDLE_KEY_WITH_PICKER(v,layer,"layer");
        HANDLE_KEY_WITH_PICKER(v,bounds,"bounds");
        v.finish_map();
    }

}

Matrix4f Node::world_matrix(){
    return (parent? parent->world_matrix():IdentityMatrix<float>())*
                TranslateMatrix(position)*
                RotateMatrix(rotation)*
                ScaleMatrix(dimensions*scale)*
                TranslateMatrix(anchor);
}

Matrix4f Node::object_matrix(){
    return IdentityMatrix<float>()*
                TranslateMatrix(position)*
                RotateMatrix(rotation)*
                ScaleMatrix(dimensions*scale)*
                TranslateMatrix(anchor)
                ;

}

Matrix4f Node::get_camera_projection(){
    return Camera::get_main_camera()->projection_matrix;
}

Matrix4f Node::get_camera_view(){
    return Camera::get_main_camera()->matrix;
}
Matrix4f Node::get_camera_inverse_view(){
    return Camera::get_main_camera()->inverse();
}

Matrix4f Node::projection_matrix(){
    Matrix4f world_m= world_matrix();
    return (get_camera_projection()* get_camera_view())*world_m;
}

Matrix4f Node::view_matrix(){
    return (parent? parent->view_matrix(): Camera::get_main_camera()->matrix)*
                TranslateMatrix(position)*
                RotateMatrix(rotation)*
                ScaleMatrix(dimensions*scale)*
                TranslateMatrix(anchor);
}

Matrix4f Node::object_matrix_inverse(){
    return IdentityMatrix<float>()*
                TranslateMatrix(-anchor)*
                ScaleMatrix(1.0f/(dimensions*scale))*
                RotateMatrix(rotation.inverse())*
                TranslateMatrix(-position);
}

Matrix4f Node::world_matrix_inverse(){
    return IdentityMatrix<float>()*
                TranslateMatrix(-anchor)*
                ScaleMatrix(1.0f/(dimensions*scale))*
                RotateMatrix(rotation.inverse())*
                TranslateMatrix(-position)*
                (parent? parent->world_matrix_inverse():IdentityMatrix<float>());

}

Matrix4f Node::view_matrix_inverse(){
    return IdentityMatrix<float>()*
                TranslateMatrix(-anchor)*
                ScaleMatrix(1.0f/(dimensions*scale))*
                RotateMatrix(rotation.inverse())*
                TranslateMatrix(-position)*
                (parent? parent->view_matrix_inverse():Camera::get_main_camera()->inverse());
}

Matrix4f Node::object_normal_matrix(){return transpose(object_matrix_inverse());}

Matrix4f Node::world_normal_matrix(){return transpose(world_matrix_inverse());}

Matrix4f Node::view_normal_matrix(){return transpose(view_matrix_inverse());}

Vector3f Node::screen_to_local(Vector3f p){
    Vector4f pos(p[0],p[1],p[2],1.0);
    pos  = max(pos,Vector4f(-1,-1,-1,-1));
    pos  = min(pos,Vector4f(1,1,1,1));
    Matrix4f m =Camera::get_main_camera()->projection_matrix*Camera::get_main_camera()->matrix;

    Matrix4f i;

    m.inverse(i);
    pos=i*Vector4f(pos);
    pos/=pos[3];
    return Vector3f(pos);
}
    Vector3f Node::world_to_local(Vector3f p){
        Vector4f pos(p[0],p[1],p[2],1.0);
        Matrix4f i =world_matrix_inverse();
        pos=i*pos;
        pos/=pos[3];
        return Vector3f(pos);
    }
    Vector3f Node::parent_to_local(Vector3f p){
        Vector4f pos(p[0],p[1],p[2],1.0);
        pos=object_matrix_inverse()*pos;
        return Vector3f(pos/=pos[3]);
    }

};
