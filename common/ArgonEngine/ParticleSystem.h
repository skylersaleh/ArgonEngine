/**
 * @brief A particle system.
 * @file ParticleSystem.h
 * @author Skyler Saleh, Ian Wiggins
 **/

#ifndef ParticleSystem_h
#define ParticleSystem_h
#include "Node.h"
#include "RenderSystem.h"
#include "MeshFormat.h"
#include <ArgonEngine/Matrix.h>
#include "ContainerReflection.h"
namespace Argon
{


    struct Particle{
        Vector4f color=kClearColor;
        Vector4f color_v=kClearColor;
        Vector3f position=Vector3f(0.f,0.f,0.f);
        Vector3f velocity=Vector3f(0.f,0.f,0.f);
        Vector3f rotation_axis=Vector3f(1.f,0.f,0.f);
        float scale=0.4f;
        float scale_v=0.1f;
        float angle=0.f;
        float angular_velocity=0.f;
        float life=-1.f;
    };


    MAKE_TYPE_INFO_Argon(Particle, {
        ADD_VAR_T(color);
        ADD_VAR_T(color_v);
        ADD_VAR_T(position);
        ADD_VAR_DOUBLE(scale);
        ADD_VAR_DOUBLE(scale_v);
        ADD_VAR_T(velocity);
        ADD_VAR_DOUBLE(angle);
        ADD_VAR_T(rotation_axis);
        ADD_VAR_DOUBLE(angular_velocity);
        ADD_VAR_DOUBLE(life);
    });

    struct ParticleSystem : public Node
    {
        std::vector<Particle> particles;

        Particle initial;
        Particle variance;
        Vector3f gravity=Vector3f(0.,0.,-9.8);
        float emission_rate=100.f;
        float allowed_spawn=0.f;

        void reinit(Particle& p){
            allowed_spawn-=1.0;
            p=initial;
            p.color+=variance.color*(random_vector<float,4>());
            p.color_v+=variance.color_v*(random_vector<float,4>());
            p.scale+=variance.scale*((rand()%10000)-5000)/5000.f;
            p.scale_v+=variance.scale_v*((rand()%10000)-5000)/5000.f;
            p.velocity+=variance.velocity*(random_vector<float,3>());
            p.position+=variance.position*(random_vector<float,3>());
            p.rotation_axis+=variance.rotation_axis*(random_vector<float,3>());
            p.rotation_axis=normalize(p.rotation_axis);
            p.angular_velocity+=variance.angular_velocity*((rand()%10000)-5000)/5000.f;
            p.angle*=variance.angle*(((rand()%10000)-5000)/5000.f);
            p.life=3;

        };
        std::shared_ptr<Node> animate(float dt){
            allowed_spawn+=dt*emission_rate;
            for(auto &p : particles){
                p.color+=p.color_v*dt;
                p.position+=p.velocity*dt;
                p.velocity+=gravity*dt;
                p.scale+=p.scale_v*dt;
                p.angle+=p.angular_velocity*dt;
                p.life-=dt;
                if(p.life<0.f&&allowed_spawn>1.f)reinit(p);
            }
            return nullptr;
        }
        MAKE_VISIT(ParticleSystem,{
                       ADD_BASE(Node);
                       ADD_VAR_T(particles);
                       ADD_VAR_T(initial);
                       ADD_VAR_T(variance);
                       ADD_VAR_DOUBLE(emission_rate);
                       ADD_VAR_DOUBLE(allowed_spawn);
                   })
    };

};

#endif
