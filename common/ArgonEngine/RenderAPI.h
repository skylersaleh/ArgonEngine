/**
 * @brief This file contains the declaration of the RenderAPI class.
 * @file RenderAPI.h
 * @author Skyler Saleh
 **/
#ifndef RenderAPI_h
#define RenderAPI_h
#include "RenderSystem.h"
#include "Utility.h"
#include "VirtualResource.h"
namespace Argon{
    /**
     * @brief The RenderAPI class describes the interface between the engine and an external rasterizer
     * such as OpenGL or DirectX.
     * @details
     * All functions provided by this class must be implemented, but additional functions may be provided
     * by the API which can be accessed by a dynamic cast to the subclass.
     *
     * By using the RenderAPI class, Renderers can remain cross platform.
     **/
    struct RenderAPI{
        /**
         * Render a Vertex Array using the current state. Set 'end_vert' to the last vertex to be
         * rendered and 'draw_mode' to the DrawType that describes how the vertices should be treated.
         **/
        virtual void draw_vertex_array(std::shared_ptr<VertexArray> array,int end_vert, int draw_mode)=0;
        ///Perform any required per frame updates and calculations.
        virtual void update_resources(){}
        ///Sets the current alpha blend operation.
        virtual void set_blend(unsigned int blend)=0;
        ///How to back-face cull the vertices.
        virtual void set_cull_face(int face)=0;
        ///Sets the range of the z-buffer.
        virtual void set_depth_range(float near, float far)=0;
        ///Apply specified render-flags
        virtual void set_render_flags(unsigned int render_flags)=0;
        ///Set the color to clear the frame buffer to.
        virtual void set_clear_color(Argon::Vector4f v)=0;
        ///Set the bounds of the viewport being rendered.
        virtual void set_viewport(int x,int y, int w, int h)=0;
        ///Set the uniforms for the shader being rendered. This is wiped after each shader change.
        virtual void set_uniforms(Uniforms ** uniforms, int size)=0;
        ///Set the shader to use when rendering
        virtual void set_shader(Renderable*,VirtualResource& x,Uniforms** uniforms, int size)=0;
        ///Bind the FBO for render textures.
        virtual void bind_render_framebuffer()=0;
        ///Bind the default window framebuffer.
        virtual void bind_default_framebuffer()=0;
        ///Bind a depth texture to the render FBO.
        virtual void bind_depth_texture(VirtualResource t)=0;
        ///Bind a specific level of a color texture to color buffer 'buffer' of the render fbo.
        virtual void bind_color_texture(VirtualResource t,int level, int buffer)=0;
        ///Clear the current FBO
        virtual void clear(bool color, bool depth, bool stencil)=0;
        ///Cache a texture for use. This must be called every frame before a texture is used.
        virtual void cache_texture(VirtualResource tex)=0;
        //Cache an array for use. This must be called every frame before an vertex array is used.
        virtual void cache_array(std::shared_ptr<VertexArray> array)=0;
        ///Cache a material for use. This must be called every frame before a material is used or when the shader is changed.
        virtual void cache_material(Material& state, const VirtualResource&  shader)=0;
    };
}

#endif
