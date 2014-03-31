//Generated by the Argon Build System


#ifndef ArgonEngineApp_ForwardRender_h
#define ArgonEngineApp_ForwardRender_h
#include <ArgonEngine/RenderSystem.h>
#include <vector>
#include <ArgonEngine/RenderAPI.h>
#include <ArgonEngine/VirtualResourceImage.h>
#include <ArgonEngine/Hardware.h>
#include <ArgonEngine/Sprite.h>
#include <ArgonEngine/Thread.h>
#include <ArgonEngine/BasicShape.h>
namespace Argon {
    class ForwardRenderer{
    private:
        Timer timer;
        struct ShaderData{
            VirtualResource full_color;
            VirtualResource full_depth;
        };
    public:
        std::map<VirtualResource, ShaderData> shaders;

        Uniforms renderer_uniforms;
        std::vector<Vector4f> &light_positions=renderer_uniforms.f4["light_positions"];
        std::vector<Vector4f> &light_colors=renderer_uniforms.f4["light_colors"];

        std::vector<Vector4f> &light_radius=renderer_uniforms.f4["light_radius"];
        std::vector<Vector4f> &light_power=renderer_uniforms.f4["light_power"];
        Vector3f& camera_pos=renderer_uniforms.get_fvec3("camera_pos");

        float &frame_time=renderer_uniforms.get_scalar("time");
        Matrix4f& view_matrix=renderer_uniforms.get_mat4("view_matrix");
        Matrix4f& projection_matrix=renderer_uniforms.get_mat4("projection_matrix");
        float &total_lights=renderer_uniforms.get_scalar("total_lights");
        Vector2f& pixel_size=renderer_uniforms.get_fvec2("pixel_size");
        Vector2f& viewport=renderer_uniforms.get_fvec2("viewport");
        VirtualResource fallback_shader;
        VirtualResource& noise_texture=renderer_uniforms.get_texture("noise_texture");

        ForwardRenderer()
        {
            light_positions.resize(32);
            light_colors.resize(32);

            light_power.resize(8);
            light_radius.resize(8);
            total_lights=0;
            noise_texture= "virtual://noise-texture.tex{w=h=128; f=RGBA8;mip=0;}";

            fallback_shader="shader://fallback.shd";

            VirtualResourceImage * noise=noise_texture.get_data<VirtualResourceImage*>();
            for(int x=0;x<128;++x){
                for(int y=0;y<128;++y){
                    Vector4f f(
                               (rand()%1000)*0.001,
                               (rand()%1000)*0.001,
                               (rand()%1000)*0.001,
                               (rand()%1000)*0.001
                               );
                    noise->set_pixel_color(x, y, f);

                }
            }
            noise->update_id_++;

        }
        ShaderData &get_shader_data(VirtualResource& r){
            ShaderData & d = shaders[r];
            if(!d.full_color)
                d.full_color = "append:{shader://default.shd,"+r.get_path_string()+"}";

            if(!d.full_depth)
                d.full_depth = "append:{shader://default.shd,string:{#define DEPTH_ONLY 1 \n},"+r.get_path_string()+"}";

            return d;
        }
        void do_shader(std::shared_ptr<Material> m){

            if(!m->shader) m->shader=fallback_shader;



        }

        // do_shader
        static bool sort_primitives(Renderable* a, Renderable* b){
            if(a->layer!=b->layer)return a->layer<b->layer;
            if(a->material!=b->material)return a->material<b->material;
            return a->vertex_array<b->vertex_array;
        }
        std::vector<std::shared_ptr<BoundingCubeNode>> bounds;
        void draw(RenderAPI& api){

            viewport=Vector2f(Argon::Screen::size[0],Argon::Screen::size[1] );
            pixel_size=Vector2f(1./Argon::Screen::size[0],1./Argon::Screen::size[1]);
            if(!Camera::get_main_camera())return;
            api.update_resources();
            Light* current_light=Light::get_root();
            int x=0;
            while(current_light&&x<32) {
                Vector4f p=current_light->world_matrix()*Vector4f(0,0,0,1.);
                light_positions[x].head<3>()= Array3f(p.head<3>())/p[3];
                light_positions[x][3]=current_light->distance;
                light_colors[x].head<3>()= (current_light->color).head<3>();
                light_colors[x][3]=current_light->specular_power*0.5f;
                ++x;

                current_light=current_light->next_instance;

            }
            while(x%4){
                light_positions[x]=Vector4f(INFINITY,INFINITY,INFINITY,0.1);
                light_colors[x]=kBlackColor;
                ++x;
            }
            total_lights=x;
            //total_lights=32;
            //total_lights=(x+3)/4;
            view_matrix= Camera::get_main_camera()->matrix;
            projection_matrix=Camera::get_main_camera()->projection_matrix;

            Matrix4f proj_view =projection_matrix*view_matrix;
            Matrix4f inv_c=view_matrix.inverse();

            Frustrum f;
            f.extract_from_matrix(proj_view);

            renderer_uniforms.get_mat4("view_normal")=inv_c.transpose();
            Vector4f v1 =inv_c*Vector4f(0.,0.,0.,1.);
            camera_pos=v1.head<3>()/v1[3];

            frame_time+=timer.delta_time();

            api.set_clear_color(Camera::get_main_camera()->background_color);
            api.set_depth_range(0.,1.);

            std::vector<Renderable*> all_visible;
            BoundsRenderer* b_it=BoundsRenderer::get_root();
            size_t bounds_count=0;
            while(b_it){
                if(b_it->parent){
                    BoundingCube c = b_it->parent->bounds.transform(b_it->parent->world_matrix().matrix());
                    if(bounds_count>=bounds.size()){
                        std::shared_ptr<BoundingCubeNode> n=std::make_shared<BoundingCubeNode>();;
                        bounds.push_back(n);
                    }
                    bounds[bounds_count]->set(c);
                    bounds[bounds_count]->color=b_it->color;
                    bounds[bounds_count]->layer = b_it->parent->layer;
                    bounds[bounds_count]->render_flags&=~(kRenderDepthMask|kRenderDepthTest);
                    ++bounds_count;
                }
                b_it=b_it->next_instance;
            }
            bounds.resize(bounds_count);
            Renderable*current = Renderable::get_root();
            while(current){
                if(current->material&&current->vertex_array){
                    current->update_transform();
                    BoundingCube c = current->bounds.transform(current->last_world_matrix);
                    int render_flags=current->render_flags&current->material->render_flags;
                    if(render_flags&kRenderShouldRender&&!f.should_cull(c)){
                        do_shader(current->material);
                        api.cache_array(current->vertex_array);
                        all_visible.insert(std::lower_bound(all_visible.begin(), all_visible.end(), current, sort_primitives),current);
                    }
                }
                current=current->next_instance;
            }
            std::vector<Renderable*>::iterator it;
            glDepthFunc(GL_LEQUAL);
            api.bind_default_framebuffer();
            api.set_viewport(0,0,viewport[0],viewport[1]);
            api.clear(true,true,false);
            it = all_visible.begin();
            api.set_blend(kBlendReplace);
            while (it!=all_visible.end()) {
                Renderable* current = *it;
                if((current->get_layer()&kRenderLayerMask)==0&&!current->is_transparent()) {
                    ShaderData& cs = get_shader_data(current->material->shader);
                    Uniforms* unif[]={&(current->uniform),&(current->material->uniform),&renderer_uniforms};

                    api.set_shader(current,cs.full_color,unif,3);
                    int render_flags=current->render_flags&current->material->render_flags;
                    api.set_render_flags(render_flags|kRenderOpaqueFlags|kRenderDepthMask);
                    api.set_cull_face(current->cull_face);
                    api.draw_vertex_array(current->vertex_array, current->end_vertex,current->vertex_array->draw_type);
                }
                ++it;
            }

            it = all_visible.begin();
            while (it!=all_visible.end()) {
                Renderable* current = *it;
                if((current->get_layer()&kRenderLayerMask)==0&& current->is_transparent()) {
                    int render_flags=current->render_flags&current->material->render_flags;
                    ShaderData& cs = get_shader_data(current->material->shader);
                    api.set_render_flags(render_flags&(~kRenderDepthMask));
                    Uniforms* unif[]={&(current->uniform),&(current->material->uniform),&renderer_uniforms};
                    api.set_shader(current,cs.full_color,unif,3);
                    api.set_blend(current->material->blend);
                    api.set_cull_face(current->cull_face);
                    api.draw_vertex_array(current->vertex_array, current->end_vertex,current->vertex_array->draw_type);
                }
                ++it;
            }
            view_matrix = Camera::get_main_camera2D()->matrix;
            projection_matrix=Camera::get_main_camera2D()->projection_matrix;
            it = all_visible.begin();
            while (it!=all_visible.end()) {
                Renderable* current = *it;
                if((current->get_layer()&kRenderLayerMask)>0) {
                    int render_flags=current->render_flags&current->material->render_flags;
                    ShaderData& cs = get_shader_data(current->material->shader);
                    api.set_render_flags(render_flags&~(kRenderDepthMask|kRenderDepthTest));
                    Uniforms* unif[]={&(current->uniform),&(current->material->uniform),&renderer_uniforms};
                    api.set_shader(current,cs.full_color,unif,3);
                    api.set_blend(current->material->blend);
                    api.set_cull_face(current->cull_face);
                    api.draw_vertex_array(current->vertex_array, current->end_vertex,current->vertex_array->draw_type);
                }
                ++it;
            }
            api.set_render_flags(kRenderDefault);
            api.bind_default_framebuffer();
            api.set_viewport(0,0,viewport[0],viewport[1]);
        }     // draw
    };
};

#endif
