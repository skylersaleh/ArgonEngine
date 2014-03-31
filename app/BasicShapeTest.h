
#ifndef __BASIC_SHAPE_TEST__
#define __BASIC_SHAPE_TEST__

#include "ArgonEngine/MeshFormat.h"
#include "ArgonEngine/Node.h"
#include "ArgonEngine/Hardware.h"
#include <ArgonEngine/BasicShape.h>
#include <iostream>
#include "bullet-test.h"
struct BoundingCubeRenderTest : public Argon::Node{
    Argon::BoundingCubeNode node;
    Argon::Input input;
    BoundingCubeRenderTest(){
        name="Bounding Cube Rendering";
        node.set(Argon::BoundingCube(Argon::Vector3f(-5,-5,-5),Argon::Vector3f(10,10,10)));
    }

    std::shared_ptr<Argon::Node> animate(float time){
        return input[' '].value?std::make_shared<BulletTest>():nullptr;
    }
};

struct BasicShapeTest : public Argon::Node {
    std::shared_ptr<Argon::BasicShape> shape;
    Argon::Input input;
    float total_time;
    float state_time;
    int state;
    Argon::Light l;
    BasicShapeTest():shape(new Argon::BasicShape){
        name="BasicShapeTest";
        total_time=0;
        state_time=0;
        state=0;
        shape->set_box();
        l.position=Vector3f(0.,0.,1.);
        l.color=Vector4f(1.,1.,1.,1.);
        l.parent=this;
        l.diffuse_power=20.0;
        l.specular_power=20.0;
        l.distance=10.0;
        shape->scale=Vector3f(5,5.,5.);
        shape->parent=this;
    }
    std::shared_ptr<Argon::Node> animate(float time){
        total_time+=time;
        state_time+=time;

        shape->color[0]=sinf(total_time*3.);
        shape->color[2]=sinf(total_time*3.);

        if(input['S'].value&&state_time>0.2){
            state_time =0.0;
            state++;
            switch (state%6) {
                case 0: shape->set_box();break;
                case 1: shape->set_sphere();break;
                case 2: shape->set_torus();break;
                case 3: shape->set_cylinder();break;
                case 4: shape->set_plane();break;
                case 5: shape->set_ring();break;
            }
        }
        return input[' '].value?std::make_shared<BoundingCubeRenderTest>():nullptr;
    }

};
#endif
