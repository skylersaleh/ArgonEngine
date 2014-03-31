

#ifndef __TEAPOT_TEST__
#define __TEAPOT_TEST__

#include <ArgonEngine/MeshFormat.h>
#include <ArgonEngine/Node.h>
#include <ArgonEngine/Hardware.h>
#include <ArgonEngine/Sprite.h>
#include <iostream>
#include "ArgonAppBase.h"

struct TeapotTest : public Argon::Node {
    Argon::MeshFormat teapot;
    Argon::Input input;
    std::shared_ptr<Argon::Node> lamp;
    TeapotTest(){
        name="TeapotTest";
        teapot.load_file("resource://teapot.ahf");
        teapot.create_objects();
        lamp = teapot.spawned_nodes["Lamp"];
        std::shared_ptr<Argon::Material> m =teapot.get_material(teapot.get_object_data("teapot").materials[0]);
        m->uniform.get_texture("texture")="resource://metal.png";
        m->shader="shader://metal.shd";
        m->color=Vector4f(0.2,0.2,0.2,1.);

    }
    std::shared_ptr<Node> animate(float time){
        Argon::Vector4f pos(input[Argon::kInputIDMouseX].value,
                     input[Argon::kInputIDMouseY].value,0.8+input[Argon::kInputIDUp].integral*0.1-input[Argon::kInputIDDown].integral*0.1,1.);
        Argon::Matrix4f i=Argon::Camera::get_main_camera()->matrix.inverse();
        pos = i*pos;
        lamp->position=(pos/pos[3]).head<3>();
        return input[' '].value?std::make_shared<ArgonAppBase>():nullptr;

    }

};
#endif
