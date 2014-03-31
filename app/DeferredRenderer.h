//Generated by the Argon Build System

//
//  ForwardRender.h
//  ArgonEngineApp
//
//  Created by Skyler Saleh on 3/16/13.
//
//

#ifndef ArgonEngineApp_DeferredRender_h
#define ArgonEngineApp_DeferredRender_h
#include <ArgonEngine/Matrix.h>
#include <ArgonEngine/RenderApi.h>

#include <ArgonEngine/RenderSystem.h>
#include <ArgonEngine/VirtualResourceImage.h>

#include <vector>
#include <sstream>
#include <ArgonEngine/Sprite.h>
#include <ArgonEngine/Hardware.h>

namespace Argon {
    /*class DeferredRenderer{
    private:
        
        VirtualResource fallback_shader;
        
        Matrix4f camera_matrix;
        Matrix4f normal_matrix;
        Matrix4f inverse_matrix;

        Vector4f camera_pos;
        std::map<StringType, Argon::Uniform> render_uniforms;
        VirtualResourceImage *image;
        VirtualResourceImage *image2;
        VirtualResourceImage *image3;
        Input input;
        VirtualResourceImage *depth;

        VirtualResource render_data1;
        VirtualResource render_data2;
        VirtualResource render_data3;

        VirtualResource depth_data;
        
        Renderable point_light_objects;
        Material point_light_material;
        VertexArray point_light_primitive;

        Sprite resolv_sprite;
        Material resolv_material;

        int total_lights;
        float transparency;
        float metalness;
        float smoothness;
    public:
        int viewport_w;
        int viewport_h;
        int current_frame;
        
        
        DeferredRenderer()
        {
            total_lights=0;
            fallback_shader="{Shaders}/fallback-deffered.shd";
            render_uniforms["camera_normal"].set( kUniformFMat4x4, &normal_matrix);
            render_uniforms["camera_inverse"].set(kUniformFMat4x4,&inverse_matrix);
            render_uniforms["camera_matrix"].set(kUniformFMat4x4,&camera_matrix);
            render_data1 = "virtual://render1.tex";
            render_data2 = "virtual://render2.tex";
            render_data3 = "virtual://render3.tex";
            depth_data = "virtual://depth_texture.tex";

            image= render_data1.get_data<VirtualResourceImage*>();
            image->width=image->tex_width=512;
            image->height=image->tex_height=512;
            image->format=kTextureRGBA16|kTextureFramebuffer;
            
            image2= render_data2.get_data<VirtualResourceImage*>();
            image2->width=image2->tex_width=512;
            image2->height=image2->tex_height=512;
            image2->format=kTextureRGBA16|kTextureFramebuffer;
            
            image3= render_data3.get_data<VirtualResourceImage*>();
            image3->width=image3->tex_width=512;
            image3->height=image3->tex_height=512;
            image3->format=kTextureRGBA16|kTextureFramebuffer;
            
            depth= depth_data.get_data<VirtualResourceImage*>();
            depth->width=depth->tex_width=512;
            depth->height=depth->tex_height=512;
            depth->format=kTextureDepth24|kTextureFramebuffer;
            
            resolv_material.textures["data1"]=render_data1;
            resolv_material.textures["data2"]=render_data2;
            resolv_material.textures["data3"]=render_data3;

            resolv_material.textures["depth"]=depth_data;
            //resolv_material.shaders["color"]="{Shaders}/resolve.shd";
            resolv_material.render_flags &= ~(kRenderDepthMask|kRenderDepthTest);

            resolv_sprite.render.cull_face=kCullNone;
            resolv_material.blend = kLinearDodge;
            
            resolv_sprite.render.layer=1;
        
            resolv_sprite.render.material = &resolv_material;
            point_light_primitive.add_attribute<float>(3, "position");
            point_light_primitive.add_attribute<float>(4, "center");

            point_light_primitive.add_attribute<float>(4, "color");

            point_light_primitive.recalc_stride();
            point_light_primitive.draw_type = kDrawTriangles;
            
            point_light_objects.vertex_array = &point_light_primitive;
            point_light_objects.material = & point_light_material;
            point_light_objects.layer = 1;
            point_light_material.textures["data1"]=render_data1;
            point_light_material.textures["data2"]=render_data2;
            point_light_material.textures["data3"]=render_data3;

            point_light_material.textures["depth"]=depth_data;
            point_light_material.shader="{Shaders}/blend_light.shd";
            point_light_objects.cull_face=kCullNone;
            point_light_material.blend = kLinearDodge;
            point_light_material.render_flags &= ~(kRenderDepthMask|kRenderDepthTest);
            point_light_material.uniforms["camera_pos"].set(kUniformFVec4, camera_pos.data);
            point_light_material.uniforms["metalness"].set(kUniformFloat, &metalness);
            point_light_material.uniforms["smoothness"].set(kUniformFloat, &smoothness);
            point_light_material.uniforms["transparency"].set(kUniformFloat, &transparency);



        }
        
        void do_shader(Material*m){
            
            VirtualResource &color_shad =  m->shader;
            if(!color_shad) color_shad=fallback_shader;
            
            
        }     // do_shader
        
        void draw(RenderAPI& api){
            Light* current_light=Light::get_root();
            total_lights=0;
            while(current_light){
                total_lights++;
                current_light = current_light->next_instance;
            }
            camera_matrix = Camera::get_main_camera()->matrix;

            current_light=Light::get_root();
            point_light_primitive.set_size(total_lights*4);
            
            
            point_light_primitive.index_data.resize(total_lights*6);
            VertexIterator it_p = point_light_primitive.begin("position");
            VertexIterator it_lp = point_light_primitive.begin("center");
            VertexIterator it_c = point_light_primitive.begin("color");
            metalness= (input['M'].integral-input['N'].integral)*0.5;
            smoothness= (input['R'].integral-input['E'].integral)*0.5;
            transparency= (input['O'].integral-input['I'].integral)*0.1;
            
           // std::cout<<"metalness: "<<metalness<<"\n";
            //std::cout<<"smoothness "<<smoothness<<"\n";
            //std::cout<<"transparency: "<<transparency<<"\n";


            size_t i =0;
            while(i<total_lights&&current_light&&i<32){
                float sc = current_light->distance;

                Vector4f v= camera_matrix*vect4(current_light->position,1.f);
                Vector3f v1(camera_matrix[0],camera_matrix[4],camera_matrix[8]);
                Vector3f v2(camera_matrix[1],camera_matrix[5],camera_matrix[9]);
                Vector3f v3(camera_matrix[2],camera_matrix[6],camera_matrix[10]);

                Vector3f s;
                float v3v = v[3];

                s[0]=current_light->distance/v3v;
                s[1]= current_light->distance/v3v;
                
                v[3]=current_light->distance;

                Vector4f lp = v;
                lp[3]=current_light->distance/v3v;
                v/=v3v;

                Vector4f lc = vect4(current_light->color, current_light->diffuse_power);
                it_lp.set(lp);

                (++it_lp).set(lp);
                (++it_lp).set(lp);
                (++it_lp).set(lp);
                ++it_lp;

                //s/=v[3];
                
                it_p.set(v+Vector4f(-s[0],-s[1],0.,0.));
                it_c.set(lc);
                
                (++it_p).set(v+Vector4f(s[0],s[1],0.,0.));
                (++it_c).set(lc);

                
                (++it_p).set(v+Vector4f(s[0],-s[1],0.,0.));
                (++it_c).set(lc);

                (++it_p).set(v+Vector4f(-s[0],s[1],0.,0.));
                (++it_c).set(lc);

                ++it_p;
                ++it_c;
                point_light_primitive.index_data[i*6+0]=i*4;
                point_light_primitive.index_data[i*6+1]=i*4+1;
                point_light_primitive.index_data[i*6+2]=i*4+2;
                
                point_light_primitive.index_data[i*6+3]=i*4;
                point_light_primitive.index_data[i*6+4]=i*4+3;
                point_light_primitive.index_data[i*6+5]=i*4+1;
                ++i;

                current_light = current_light->next_instance;
            }
            point_light_primitive.update_id++;
            std::set<Material*> cached_materials;
            
            
            camera_matrix.inverse(inverse_matrix);
            camera_pos=inverse_matrix*Vector4f(0.,0.,1.,1.);
            camera_pos/=camera_pos[3];
            //std::cout<<"camera: "<<camera_pos[0]<<" "<<camera_pos[1]<<" "<<camera_pos[2]<<"\n";
            current_frame++;
            
            normal_matrix=transpose(inverse_matrix);
            
            
            
            api.update_resources();
            api.cache_texture(depth_data);
            api.cache_texture(render_data1);
            api.cache_texture(render_data2);
            api.cache_texture(render_data3);

            image->tex_height=
            image2->tex_height=
            image3->tex_height=
            depth->tex_height=viewport_h;
            
            image->tex_width=
            image2->tex_width=
            image3->tex_width=
            depth->tex_width=viewport_w;
            image->update_id_++;
            image2->update_id_++;
            image3->update_id_++;
            depth->update_id_++;


            api.set_clear_color(Argon::Vector4f(0,0,0,0));
            api.set_depth_range(0.,1.);
            std::vector<Renderable*> targeted_renderables;
            
            api.set_viewport(0,0,viewport_w,viewport_h);
            
            //api.bind_default_framebuffer();
            api.bind_render_framebuffer();
            api.bind_depth_texture(depth_data);
            api.bind_color_texture(render_data1, 0, 0);
            api.bind_color_texture(render_data2, 0, 1);

            GLenum buffers[]={
                GL_COLOR_ATTACHMENT0_EXT,
                GL_COLOR_ATTACHMENT1_EXT,

            };
            glDrawBuffers(2,buffers);
            api.clear(true,true,false);
            Renderable* current=Renderable::get_root();
            api.set_blend(kBlendReplace);

            while (current) {
                int render_flags=current->render_flags&current->material->render_flags;
                if(render_flags&kRenderShouldRender&&current->layer!=1) {
                    api.set_render_flags(render_flags|kRenderOpaqueFlags);
                    if(cached_materials.insert(current->material).second){
                        do_shader(current->material);
                        api.cache_material(*current->material,current->material->shader);
                    }
                    if(!current->is_transparent()&&current->vertex_array){
                        api.cache_array(current->vertex_array);
                        api.set_shader(current,current->material->shader);
                        api.set_cull_face(current->cull_face);
                        api.set_uniforms(render_uniforms);
                        api.draw_vertex_array(current->vertex_array, current->end_vertex,current->vertex_array->draw_type);
                    }
                }
                
                current=current->next_instance;
            }
            api.bind_default_framebuffer();
            GLenum b2[]={GL_COLOR_ATTACHMENT0_EXT};
            glDrawBuffers(1,b2);
            api.set_viewport(0,0,viewport_w,viewport_h);
            api.clear(true,true,false);
            current=Renderable::get_root();
            
            while (current) {
                int render_flags=current->render_flags&current->material->render_flags;
                if(render_flags&kRenderShouldRender&&current->layer==1) {
                    api.set_render_flags(render_flags);
                    if(cached_materials.insert(current->material).second){
                        do_shader(current->material);
                        api.cache_material(*current->material,current->material->shader);
                    }
                    if(current->vertex_array){
                        api.set_blend(current->material->blend);

                        api.cache_array(current->vertex_array);
                        api.set_shader(current,current->material->shader);
                        api.set_cull_face(current->cull_face);
                        api.set_uniforms(render_uniforms);
                        api.draw_vertex_array(current->vertex_array, current->end_vertex,current->vertex_array->draw_type);
                    }
                }
                
                current=current->next_instance;
            }
            
            current=Renderable::root_renderable;
            while(current)
            {
                int render_flags=current->render_flags&current->material->render_flags;
                if(current->is_transparent()&& render_flags&kRenderShouldRender) {
                    api.set_shader(current,current->material->shaders["color"]);
                    api.set_blend(current->material->blend);
                    api.set_cull_face(current->cull_face);
                    api.set_uniforms(render_uniforms);
                    api.draw_vertex_array(current->vertex_array, current->end_vertex,current->vertex_array->draw_type);
                }
                current=current->next_renderable;
                
            }
            
            api.bind_default_framebuffer();
            api.set_viewport(0,0,viewport_w,viewport_h);
            
        }     // draw
    };*/
};

#endif
