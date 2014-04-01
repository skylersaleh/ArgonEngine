//Generated by the Argon Build System

    //
    //  ARMeshFormat.cpp
    //  Neon Rush
    //
    //  Created by Skyler Saleh on 7/28/12.
    //  Copyright (c) 2012 Argon Software. All rights reserved.
    //

#include "MeshFormat.h"
#include <fstream>
#include "Quaternion.h"
#include "Matrix.h"
#include "ContainerReflection.h"
namespace Argon{

    std::shared_ptr<Material> MeshFormatFile::get_material(const StringIntern& s){

        return materials[s];
    }
    void MeshFormatObject::set_mesh_data(MeshFormatObjectData* d,MeshFormatFile* file){
        data=d;

        BoundingCube b;

        Matrix4f m =Matrix4f(d->matrix);

        Quaternionf q;
        q= get_rotation(m);
        q=normalize(q);
        Matrix4f m2 ;
        m2= m*RotateMatrix(q.inverse());
        Matrix3f scale_m;
        Matrix3f rot_m;
        Vector3f s = get_scale(m2);
        Vector3f scale_v=s;

        Matrix4f m3;
        m3=m2;

        Vector3f pos_v=get_position(m3);
        anchor=Vector3f(0,0,0);
        position=pos_v;
        dimensions=scale_v;
        //rotation.setIdentity();
        rotation=q;
        if(d->materials.size())
        material = file->materials[d->materials[0]];
        color = material?material->color:kWhiteColor;

        vertex_array=file->meshes[data->mesh];
        if(file->meshes[data->mesh])
        bounds=vertex_array->get_attribute_bounds(kPositionAttribute);
    }


    void MeshFormatLight::set_mesh_data(MeshFormatLightData* d){
        data=d;
        Light::specular_power= Light::diffuse_power=d->intensity;
        Light::distance=d->distance;
        color=d->color;
        Matrix4f m2=Matrix4f(data->matrix);
        position=get_position(m2);

    }
    void MeshFormat::load_file(VirtualResource p)
    {
        file.read(p);
    };
    void MeshFormat::create_objects(){
        object_data_iterator it = begin_object_data();
        object_data_iterator end = end_object_data();
        std::map<StringIntern, std::shared_ptr<Material> >&materials = file.materials;
        std::map<StringIntern, std::shared_ptr<Node> > object_map;

        while(it!=end){
            std::shared_ptr<MeshFormatObject>o(new MeshFormatObject(&it->second,&file));
            object_map[it->first]=o;
            if(it->second.materials.size()&& &materials[it->second.materials[0]])
                o->material =materials[it->second.materials[0]];


            ++it;
        }
        light_data_iterator it2 = begin_light_data();
        light_data_iterator end2 = end_light_data();

        while(it2!=end2){
            std::shared_ptr<MeshFormatLight> o;
            object_map[it2->first]=o=std::shared_ptr<MeshFormatLight>(new MeshFormatLight(&it2->second));

            ++it2;
        }

        std::map<StringIntern, std::shared_ptr<Node> >::iterator it3 = object_map.begin();
        while (it3!=object_map.end()) {
            std::shared_ptr<Node> n = object_map[get_file().objects[it3->first].parent];
            it3->second->parent=n?&*n:this;

            spawned_nodes[it3->first]=it3->second;
            ++it3;
        }

    }
    std::shared_ptr<MeshFormatObject> MeshFormat::spawn_object(const StringIntern& s){
        MeshFormatObjectData* d = &get_object_data(s);
        if(!d){
            std::cout<<s<<" was not found\n";
            return nullptr;
        }
        std::shared_ptr<MeshFormatObject> o(new MeshFormatObject(d,&file));

        if(d->materials.size())
        o->material=get_material(d->materials[0]);

        o->parent = &*spawned_nodes[d->parent];
        if(!o->parent)o->parent=this;

        return o;
    }
    MAKE_VISIT_IMPL(MeshFormatObjectData, {
                             ADD_VAR(matrix);
                             ADD_VAR(mesh);
                             ADD_VAR(parent);
                             ADD_VAR(materials);
                             ADD_VAR(quaternion);
                             ADD_VAR(scale);
                             ADD_VAR(position);
                         });
    MAKE_VISIT_IMPL(MeshFormatLightData, {
                             ADD_VAR(matrix);
                             ADD_VAR(color);
                             ADD_VAR(parent);
                             ADD_VAR(type);
                             ADD_VAR(intensity);
                             ADD_VAR(distance);
                             ADD_VAR(angle);
                             ADD_VAR(softness);
                    })
    MAKE_VISIT_IMPL(MeshFormatFile, {
                             ADD_VAR2(global_matrix,"matrix");
                             ADD_VAR(materials);
                             ADD_VAR(meshes);
                             ADD_VAR(lights);
                             ADD_VAR(objects);
                         });
    MAKE_VISIT_IMPL(MeshFormatLight,{
                   ADD_BASE(Light);
               });


};