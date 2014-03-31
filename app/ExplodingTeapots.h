
#ifndef __EXPLODING_TEAPOTS__
#define __EXPLODING_TEAPOTS__

#include "ArgonEngine/MeshFormat.h"
#include "ArgonEngine/Node.h"
#include "ArgonEngine/Hardware.h"
#include "RayMarchingTest.h"

struct ExplodingTeapots : public Argon::Node {
    Argon::Input input;
    Argon::MeshFormat model;
    Argon::ParticleSystem system;
    std::vector<std::shared_ptr<Argon::Node> > shapes;
    Argon::Light l;
    ExplodingTeapots(){
        name= "ExplodingTeapots";
        l.position.set(1.,1.,1.);
        l.color.set(1.,1.,1.,1.);
        l.parent=this;

        l.diffuse_power=5.0;
        l.specular_power=5.0;
        l.distance=10.0;

        model.load_file("resource://teapot.ahf");
        int particles =600;
        system.variance.color=Argon::kWhiteColor;
        system.variance.color_v.set_all(1.);
        system.variance.rotation_axis.set_all(10.);
        system.variance.angular_velocity=2.5;
        system.variance.scale_v=0.1;
        system.emission_rate = particles/3.;

        system.initial.scale=0.24;
        system.initial.color.set_all(1);
        system.initial.velocity.set(0.5,0.5,2);
        system.variance.velocity.set_all(5.);
        system.particles.resize(particles);

        shapes.resize(particles);
        for(auto& s : shapes){s=model.spawn_object("teapot");s->parent=this;}
    }
    std::shared_ptr<Argon::Node> animate(float time){
        system.animate(time*(input['f'].integral-input['s'].integral+1.));
        size_t i=0;
        for(auto &p:system.particles){
            shapes[i]->position=p.position;
            shapes[i]->scale.set_all(p.scale);
            shapes[i]->color=p.color;

            shapes[i]->rotation.set_angle_axis(p.angle,p.rotation_axis);
            ++i;
        }

        return input[' '].value?std::make_shared<RayMarchingTest>():nullptr;
    }

};
#endif
