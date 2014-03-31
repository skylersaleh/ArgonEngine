
#ifndef ArgonApp_RayTracingTest_h
#define ArgonApp_RayTracingTest_h

#include <ArgonEngine/Node.h>
#include <ArgonEngine/Hardware.h>
#include <ArgonEngine/Sprite.h>
#include "FontTest.h"

struct RayTracingTest : public Argon::Node {
    Argon::Sprite sprite;
    Argon::Input input;
    RayTracingTest(){
        name="Raytracing Test";
        sprite.set_shader("shader://raytracer.shd");
        sprite.dimensions.set(20,20.,0.);

    }
    std::shared_ptr<Node> animate(float time){
        return input[' '].value?std::make_shared<FontTest>():nullptr;
    }

};
#endif

