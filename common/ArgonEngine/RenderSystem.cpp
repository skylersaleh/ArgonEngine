//Generated by the Argon Build System

//
//  ARRenderSystem.cpp
//  NeonRush
//
//  Created by Skyler Saleh on 5/3/12.
//  Copyright (c) 2012 Argon Software. All rights reserved.
//

#include "RenderSystem.h"
#include "Types.h"
#include "Utility.h"
#include <iostream>
#include "Reflection.h"
#include "Collision.h"
#include "ContainerReflection.h"

namespace Argon {
void VertexArray::recalc_stride(){
    int s = 0;
    for(size_t x = 0; x<attributes.size(); ++x) {
        s += attributes[x].components * kRenderTypeSize[attributes[x].type];
    }

    stride = s;
    for(size_t x = 0; x<attributes.size(); ++x) {
        attributes[x].stride=s;
    }
}
VertexArray::VertexArray() : updates_frequently(false),draw_type(kDrawTriangles),stride(0){
}

bool VertexArray::has_attribute(const StringIntern& s){
    for(size_t x = 0; x<attributes.size(); ++x) {
        if(attributes[x].attribute==s) return true;
    }
    return false;
}
VertexIterator VertexArray::begin(const StringIntern& s){
    for(size_t x = 0; x<attributes.size(); ++x) {
        if(attributes[x].attribute==s) {
            return VertexIterator(data_start() + attributes[x].offset,&attributes[x]);
        }
    }
    return VertexIterator();
}
BoundingCube VertexArray::get_attribute_bounds(const StringIntern & s){
    BoundingCube   b;
    Vector3f       v;
    VertexIterator it = begin(s);
    VertexIterator end_it = end(s);
    while(it!=end_it) {
        it.get(v);
        b.add_point_to_bounds(v);
        ++it;
    }
    return b;

}
void VertexArray::generate_indices()
{
    index_data.resize(vertex_count());
    for (int x = 0; x<vertex_count(); ++x) index_data[x]=x;
}
void VertexArray::generate_indices_quad(){
    //index_data.clear();
    int count= vertex_count()/4;
    index_data.resize(count*6);
    for (int x = 0; x<count; ++x){
        index_data[x*6]=x*4;
        index_data[x*6+1]=x*4+1;
        index_data[x*6+2]=x*4+2;
        index_data[x*6+3]=x*4+2;
        index_data[x*6+4]=x*4+3;
        index_data[x*6+5]=x*4+1;

    }
}

void VertexArray::append_data(const uint8_t* d, int bytes_size){
    int old_size = data.size();
    data.resize(bytes_size + old_size);
    memcpy(&data[old_size], d, bytes_size);
    update_id++;
}
void VertexArray::set_data(const uint8_t* d, int bytes_size){
    data.resize(bytes_size);
    memcpy(&data[0], d, bytes_size);
    update_id++;
}
VertexAttribPair VertexArray::get_attribute(const StringIntern&s)
{
    for (size_t x = 0; x<attributes.size(); ++x) {
        VertexAttribPair& a = attributes[x];
        if (a.attribute==s) return a;
    }
    return VertexAttribPair();
}


Matrix4f Camera::inverse(){
    Matrix4f i;
    Matrix4f m = projection_matrix*matrix;
    m.inverse(i);
    return i;
}

Camera::~Camera(){
    std::map<VirtualResource, Camera*>::iterator it = all_cameras().begin();
    while(it!=all_cameras().end()){
        if(it->second==this)all_cameras().erase(it);
        ++it;
    }
}
void Frustrum::extract_from_matrix(const Matrix4f& m){

    planes[kRight]=Vector3f(m(0,3)-m(0,0), m(1,3)-m(1,0), m(2,3)-m(2,0));
    distances[kRight]=m(3,3)-m(3,0);
    planes[kLeft]=Vector3f(m(0,3)+m(0,0), m(1,3)+m(1,0), m(2,3)+m(2,0));
    distances[kLeft]=m(3,3)+m(3,0);

    planes[kTop]=Vector3f(m(0,3)-m(0,1),m(1,3)-m(1,1),m(2,3)-m(2,1));
    distances[kTop]=m(3,3)-m(3,1);
    planes[kBottom]=Vector3f(m(0,3)+m(0,1),m(1,3)+m(1,1),m(2,3)+m(2,1));
    distances[kBottom] = m(3,3)+m(3,1);

    planes[kFar]=Vector3f(m(0,3)-m(0,2), m(1,3)-m(1,2), m(2,3)-m(2,2));
    distances[kFar] = m(3,3)-m(3,2);
    planes[kNear]=Vector3f(m(0,3)+m(0,2), m(1,3)+m(1,2), m(2,3)+m(2,2));

    for(int i=0;i<6;++i){
        float d = length(planes[i]);
        planes[i]/=d;
        distances[i]/=d;
    }



}

Frustrum::collision_mapping Frustrum::test_cube(const BoundingCube &c)const{
    collision_mapping result = kInside;
    //for each plane do ...
    for(int i=0; i < 6; i++) {

        // is the positive vertex outside?
        if (point_plane_distance(planes[i], distances[i],c.get_positive_vertex(planes[i]))<0.f)
            return kOutside;
        // is the negative vertex outside?
        else if (point_plane_distance(planes[i], distances[i],c.get_negative_vertex(planes[i]))<0.f)
            result =  kCollides;
    }
    return(result);


}

void Renderable::update_transform(){
    last_world_matrix = world_matrix();
    last_normal_matrix = world_normal_matrix();
    last_color = get_color();
    if(get_should_render()){
        render_flags|=kRenderShouldRender;
    }else{
        render_flags&=~kRenderShouldRender;
    }

}








void VertexArray::optimize_vertices(){
    std::map<Buffer, int> needed_indices;
    size_t s = vertex_count();
    for(size_t i=0;i<s;++i){
        Buffer b;
        b.size=stride;
        b.d=vertex(i);
        needed_indices[b]=0;
    }
    std::cout<<"Before: "<<s<<" After: "<<needed_indices.size()<<"\n";
}


Material &Material::operator =(const Material &m){
    uniform=m.uniform;
    shader=m.shader;
    blend = m.blend;
    name=m.name;
    render_flags=m.render_flags;
    return *this;
}


Renderable &Renderable::operator =(const Renderable &r){
    uniform=r.uniform;
    vertex_array = r.vertex_array;
    render_flags = r.render_flags;
    tag = r.tag;
    end_vertex = r.end_vertex;
    cull_face = r.cull_face;
    material = r.material;
    return *this;
}
MAKE_VISIT_IMPL(Frustrum,{
               ADD_VAR(planes);
               ADD_VAR(distances);
           })
MAKE_VISIT_IMPL(Camera, {
    ADD_VAR(matrix);
    ADD_VAR(projection_matrix);
});
MAKE_VISIT_IMPL(VertexArray, {
    ADD_VAR2(updates_frequently, "updates frequently");
    ADD_VAR(data);
    ADD_VAR2(index_data, "faces");
    ADD_VAR(attributes);
    ADD_VAR2(draw_type, "draw_mode");
    t.recalc_stride();
});
MAKE_VISIT_IMPL(Light,{
               ADD_BASE(Node);
               ADD_VAR(distance);
               ADD_VAR(specular_power);
               ADD_VAR(diffuse_power);
           })

MAKE_VISIT_IMPL(Uniforms, {
               ADD_VAR(textures);
               ADD_VAR(mat4);
               ADD_VAR(mat3);
               ADD_VAR(mat2);
               ADD_VAR(f4);
               ADD_VAR(f3);
               ADD_VAR(f2);
               ADD_VAR(f);
           })
MAKE_VISIT_IMPL(Material,{
               ADD_VAR(blend);
               ADD_VAR(render_flags);
               ADD_VAR(shader);
               ADD_VAR(color);

               ADD_VAR2(uniform.textures,"textures");
               ADD_VAR(name);

               if(!v.read_only()) {
                   while(v.has_unhandled_keys()) {
                       std::string s = v.get_unhandled_key();
                       v.set_key(s.c_str());
                       if(s=="transparent"){
                           int x =0;
                           v.handle(x);
                           if(x&&t.blend==kBlendReplace){
                               t.blend=kBlendAlpha;
                           }
                       }else v.handle(t.uniform.get_fvec4(s));
                   }
               }
           })
MAKE_VISIT_IMPL(Renderable,{
               ADD_BASE(Node);
               ADD_VAR(uniform);
               ADD_VAR(render_flags);
               ADD_VAR(tag);
               ADD_VAR(end_vertex);
               ADD_VAR(cull_face);
               ADD_VAR(vertex_array);
               ADD_VAR(material);
           })
};
