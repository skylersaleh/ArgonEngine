
#ifndef __STREAM_VERTEX_TEST__
#define __STREAM_VERTEX_TEST__

#include <ArgonEngine/Node.h>
#include <ArgonEngine/Hardware.h>
#include <ArgonEngine/Sprite.h>
#include <ArgonEngine/RenderSystem.h>
#include <iostream>
#include "TeapotTest.h"

struct StreamVertexArrayTest : public Argon::Node {
    Argon::Sprite sprite;
    std::shared_ptr<Argon::VertexArray> array;
    Argon::Input input;
    float total_time=0;
    StreamVertexArrayTest(){
        name = "StreamedVertexArray";
        array.reset(new Argon::VertexArray);
        sprite.texture="resource://test.png";
        array->updates_frequently=true;
        array->add_attribute<float>(2, "texture_coord");
        array->add_attribute<float>(2, "position");

        array->set_size(102);
        array->draw_type = Argon::kDrawTriangleStrip;
        array->generate_indices();
        Argon::VertexIterator tex_it = array->begin("texture_coord");
        Argon::VertexIterator pos_it = array->begin("position");
        for(int i=0;i<51;++i){
            *(tex_it++)=Argon::Vector2f((float)i/50.0,1.0);
            *(tex_it++)=Argon::Vector2f((float)i/50.0,0.0);

            *(pos_it++)=Argon::Vector2f((float)i/50.0-0.5,-0.5);
            *(pos_it++)=Argon::Vector2f((float)i/50.0-0.5,0.5);
        }
        array->update_id++;
        sprite.vertex_array = array;
        sprite.dimensions=Vector3f(10,10.,10.);
        sprite.parent=this;
    }
    std::shared_ptr<Node> animate(float time){
        color.head<3>()=Argon::hsv_to_rgb(Argon::Vector3f(total_time*0.2,1,1));
        total_time+=time*2.;
        Argon::VertexIterator pos_it = array->begin("position");
        for(int i=0;i<51;++i){
            float x_offset = sinf(i*0.1f+total_time)*0.1;
            float y_offset = cosf(i*0.1f+total_time)*0.1;
            *(pos_it++)=Argon::Vector2f((float)i/50.0-0.5+x_offset,-0.5+y_offset);
            *(pos_it++)=Argon::Vector2f((float)i/50.0-0.5+x_offset,0.5+y_offset);
        }
        array->update_id++;
        return input[' '].value?std::make_shared<TeapotTest>():nullptr;
    }

};
#endif
